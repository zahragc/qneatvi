#include "textedit.h"
#include "xterm256torgb.h"
#include <QtMath>
#include <QApplication>
TextEdit::TextEdit(QWidget *parent) : QPlainTextEdit(parent) ,ansiExpression("\33\\[[\\d;]*m"){
  this->installEventFilter(this);
  setUndoRedoEnabled(false);
  this->setLineWrapMode(QPlainTextEdit::NoWrap);
  QTextOption topt = this->document()->defaultTextOption();
  topt.setTextDirection(Qt::LeftToRight);
  //topt.setAlignment(Qt::AlignLeft);
  this->document()->setDefaultTextOption(topt);
  findDialog = new FindDialog(this);
  settingDialog = new SettingDialog (this);
  initialOutputWindow();
  mode = TextEdit::NormalMode;
  setTextEditFont();
  setTextEditSettings();
  connect(this, &TextEdit::inputReady, this, &TextEdit::quitEventLoop);
  connect(settingDialog, &SettingDialog::accepted, this, &TextEdit::setTextEditSettings);
  //connect(this->document(), &QTextDocument::blockCountChanged, this, &TextEdit::manageBlockCount);
  connect(findDialog, &FindDialog::commandReady, this, &TextEdit::exeCommand);
}
void TextEdit::quitEventLoop(void) { eventLoop.quit(); }

TextEdit::~TextEdit(){}

void TextEdit::init(void){
  columnsCount = this->cols();
  rowsCount = this->rows(); // number : 0 - rowsCount-1
  QFontMetrics fontProperty(this->font());
  setCursorWidth(fontProperty.averageCharWidth());
  //this->document()->setDefaultCursorMoveStyle(Qt::VisualMoveStyle);
}

int TextEdit::cols(void) {
  QFontMetricsF fontProperty(this->font());
  double width = this->contentsRect().width();
  double avgCharWidth = fontProperty.averageCharWidth();
  return qFloor(width / avgCharWidth) - 2;
}

int TextEdit::rows(void) {
  QFontMetricsF fontProperty(this->font());
  double height = this->contentsRect().height();
  double rowHeight = fontProperty.lineSpacing() + 2;
  return qFloor(height / rowHeight);
}

void TextEdit::room(int n) {
  //qDebug() << "room : " << n;
  QTextCursor cursor = this->textCursor();
  if (n < 0) // "\33[%dM" delete first line //Scroll display up one line.
  {
    n = -1 * n;
    while (n-- > 0) {
      cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
      cursor.removeSelectedText();
      cursor.deleteChar();
    }
  } else if (n > 0) //\33[%dL add line
  {
    int pos = cursor.position();
    while (n-- > 0) {
      cursor.insertText("\n");
    }
    cursor.setPosition(pos, QTextCursor::MoveAnchor);
  }
  this->setTextCursor(cursor);
  manageBlockCount(this->document()->blockCount());
}

int TextEdit::read(char *ibuf, int readsize) {
  if (!input.size()) {
    wait = true;
    paste = false;
    eventLoop.exec();
  }
  wait = false;
  /*
  while(!(input.size())) {
      qApp->processEvents();
  }
  */
  int i;
  for (i = 0; i < readsize; ++i) {
    if (input.size()) {
       *ibuf = input.front();
       input.pop();
    } else
      break;
  }
  return i;
}


void TextEdit::pos(int r, int c) {
  //qDebug() << "pos" << r << "         " << c;
  if (r > rowsCount - 1) {
    return;
  }
  QTextCursor cursor = this->textCursor();
  // not change row just change column  \r\33[%dC or D
  if (r < 0) {
    cursor.movePosition(QTextCursor::StartOfLine);
    if (c > 0)
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, c + 1);
    if (c < 0)
      cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor,  -1 * c + 1);
  } else {
    // change row and  column  \r"\33[%d;%dH", r + 1, c + 1);
    bool a;
    cursor.movePosition(QTextCursor::Start);
    //while (r-- > 0) {
    for(int i = 0; i < r; i++){
      a = cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, 1);
      if (!a) {
        cursor.movePosition(QTextCursor::EndOfBlock);
        cursor.insertText("\n");
        // manageBlockCount();
      }
    }
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, c ? c + 1 : c);
  }
  this->setTextCursor(cursor);
}

void TextEdit::kill(void) {
  QTextCursor cursor = this->textCursor();
  cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
  //qDebug() << "KILL:" << cursor.selectedText();
  cursor.removeSelectedText();
  this->setTextCursor(cursor);
}

void TextEdit::write(char *s) {
  this->printText(s);
  // this->manageBlockCount();
}

void TextEdit::keyPressEvent(QKeyEvent *event) {
  //qDebug()<<"//////////keyPressEvent///////////"<<event->text()[0].unicode();
    if (event->text()[0].unicode() != 0) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        enterKeyFunction();
    else{
      QByteArray a = event->text().toUtf8();
      int len = a.length();
      for(int i = 0; i < len; i++)
        input.push(a[i]);
    }
    if(wait) emit inputReady();
  } else
      checkArrow(event->key());
}

void TextEdit::setTextEditFont(void){
    //QFont currentFont("Monospace");
    QFont currentFont("vazir code");
   //currentFont.setFixedPitch(true);
    //QFont currentFont("tahoma");
    currentFont.setPointSize(14);
    this->setFont(currentFont);
}

void TextEdit::printText(QString text){
  QTextCharFormat textCharFormat = defaultTextCharFormat;
  QTextCursor cursor = this->textCursor();
  ansiMatch = ansiExpression.match(text);
  int offset = ansiMatch.capturedStart();
  QString t;
  // start from 0 , with ofsett string . if -1 all
  t = setDir + text.mid(0, offset);
  cursor.setCharFormat(defaultTextCharFormat);
  cursor.insertText(t);
  while (offset >= 0) {
    int previousOffset = ansiMatch.capturedEnd();
    QString matchEscapeSeq = ansiMatch.captured();
    matchEscapeSeq = matchEscapeSeq.remove("\33[");
    matchEscapeSeq = matchEscapeSeq.remove("m");
    QStringList list = matchEscapeSeq.split(";");
    QListIterator<QString> i(list);
    while (i.hasNext()) {
      bool ok = false;
      int attribute = i.next().toInt(&ok); // 0 = nothing
      Q_ASSERT(ok);
      this->parseAnsi(attribute, i, textCharFormat);
    }
    ansiMatch = ansiExpression.match(text, previousOffset);
    offset = ansiMatch.capturedStart();
    if (offset < 0) {
      t = text.mid(previousOffset);
      cursor.insertText(t, textCharFormat);
    } else {
      t = text.mid(previousOffset, offset - previousOffset);
      cursor.insertText(t, textCharFormat);
    }
  }
  cursor.setCharFormat(defaultTextCharFormat);
  this->setTextCursor(cursor);
  if(paste)
    qApp->processEvents();
  //this->repaint();
}

#define ALPHA 230
void TextEdit::parseAnsi(
    int attribute, QListIterator<QString> &i, QTextCharFormat &textCharFormat) {
  switch (attribute) {
  case 0: { // Normal/Default (reset all attributes)
    textCharFormat = defaultTextCharFormat;
    break;
  }
  case 1: { // Bold/Bright (bold or increased intensity)
    textCharFormat.setFontWeight(QFont::Bold);
    break;
  }
  case 2: { // Dim/Faint (decreased intensity)
    textCharFormat.setFontWeight(QFont::Light);
    QColor color = textCharFormat.foreground().color();
    if (color == Qt::black)
      textCharFormat.setForeground(Qt::darkGray);
    else
      textCharFormat.setForeground(color.darker());
    break;
  }
  case 3: { // Italicized (italic on) //standout
    textCharFormat.setFontItalic(true);
    break;
  }
  case 4: { // Underscore (single underlined)
    textCharFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
    textCharFormat.setFontUnderline(true);
    break;
  }
  case 5: { // Blink (slow, appears as Bold)
    textCharFormat.setFontWeight(QFont::ExtraBold); // ‌Bold
    break;
  }
  case 6: { // Blink (rapid, appears as very Bold)
    textCharFormat.setFontWeight(QFont::Black);
    break;
  }
  case 7: { // Reverse/Inverse (swap foreground and background)
    QBrush foregroundBrush = textCharFormat.foreground();
    textCharFormat.setForeground(textCharFormat.background());
    textCharFormat.setBackground(foregroundBrush);
    break;
  }
  case 8: { // Concealed/Hidden/Invisible (usefull for passwords)
    textCharFormat.setForeground(textCharFormat.background());
    break;
  }
  case 9: { // Crossed-out characters
    textCharFormat.setFontStrikeOut(true);
    break;
  }
  case 10: { // Primary (default) font
    textCharFormat.setFont(defaultTextCharFormat.font());
    break;
  }
  case 30:
  case 31:
  case 32:
  case 33:
  case 34:
  case 35:
  case 36:
  case 37: {
    int colorIndex = attribute - 30;
    QColor color;
    if (QFont::Normal < textCharFormat.fontWeight()) {//Bold Text is lighter
      switch (colorIndex) {
      case 0: { // black
        color = Qt::darkGray;
        break;
      }
      case 1: {
        color = Qt::red;
        break;
      }
      case 2: {
        color = Qt::green;
        break;
      }
      case 3: {
        color = Qt::yellow;
        break;
      }
      case 4: {
        color = Qt::blue;
        break;
      }
      case 5: {
        color = Qt::magenta;
        break;
      }
      case 6: {
        color = Qt::cyan;
        break;
      }
      case 7: { // Light gray
        // color = Qt::white;
        color = Qt::gray;
        break;
      }
      default: { Q_ASSERT(false); }
      }
    } else {//Normal Text is darker
      switch (colorIndex) {
      case 0: {
        color = Qt::black;
        break;
      }
      case 1: {
        color = Qt::red;
        color.setAlpha(ALPHA);
        break;
      }

      case 2: {
        color = Qt::darkGreen;
        color.setAlpha(ALPHA);
        break;
      }
      case 3: {
        color = Qt::darkYellow;
        color.setAlpha(ALPHA);
        break;
      }
      case 4: {
        color = Qt::blue;
        color.setAlpha(ALPHA);
        break;
      }
      case 5: {
        color = Qt::magenta;
        color.setAlpha(ALPHA);
        break;
      }
      case 6: {
        color = Qt::darkCyan;
        color.setAlpha(ALPHA);
        break;
      }
      case 7: { // Light gray
        // color = Qt::lightGray;
        color = Qt::darkGray;
        break;
      }
      default: { Q_ASSERT(false); }
      }
    }
    textCharFormat.setForeground(color);
    break;
  }

  case 39: {
    textCharFormat.setForeground(defaultTextCharFormat.foreground());
    break;
  }
  case 40:
  case 41:
  case 42:
  case 43:
  case 44:
  case 45:
  case 46:
  case 47: {
    int colorIndex = attribute - 40;
    QColor color;
    switch (colorIndex) {
    case 0: {
      color = Qt::darkGray;
      break;
    }
    case 1: {
      color = Qt::red;
      break;
    }
    case 2: {
      color = Qt::green;
      break;
    }
    case 3: {
      color = Qt::yellow;
      break;
    }
    case 4: {
      color = Qt::blue;
      break;
    }
    case 5: {
      color = Qt::magenta;
      break;
    }
    case 6: {
      color = Qt::cyan;
      break;
    }
    case 7: {
      color = Qt::lightGray;
      break;
    }
    default:
      Q_ASSERT(false);
    }
    textCharFormat.setBackground(color);
    break;
  }
  case 49: {
    textCharFormat.setBackground(defaultTextCharFormat.background());
    break;
  }
  case 38:
  case 48: {
    if (i.hasNext()) {
      bool ok = false;
      int selector = i.next().toInt(&ok);
      Q_ASSERT(ok);
      QColor color;
      if(selector == 5) {
        if (!i.hasNext())
          break;
        int xterm256 = i.next().toInt(&ok);
        Q_ASSERT(ok);
          color.setRgb(Xterm256ToRgb::getRed(xterm256),
                       Xterm256ToRgb::getGreen(xterm256),
                       Xterm256ToRgb::getBlue(xterm256));
          if(attribute == 38)
              textCharFormat.setForeground(color);
          else
              textCharFormat.setBackground(color);
        }
    }
    break;
  }
  }
}
