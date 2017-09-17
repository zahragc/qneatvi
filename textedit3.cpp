#include "textedit.h"
#include "neatvi/vi.h"
#include <QApplication>

bool TextEdit::checkArrow(int key){
    char arrow;
    if(mode != TextEdit::NormalMode && mode != TextEdit::InsertMode)
        return false;
    switch (key) {
    case Qt::Key_Up:
        arrow = 'k';
        break;
    case Qt::Key_Down:
        arrow = 'j';
        break;
    case Qt::Key_Right:
        arrow = 'l';
        break;
    case Qt::Key_Left:
        arrow = 'h';
        break;
    default:
        return false;
    }
    if(mode == TextEdit::InsertMode){
        input.push(ESC);
    }
    input.push(arrow);
    if(wait) emit inputReady();
    return true;
}

void TextEdit::enterKeyFunction(void){
    enter = true;
    input.push('\n');
}

void TextEdit::escapeKeyFunction(void){
    /*if(enter){
        manageBlockCount(this->document()->blockCount());
        enter = false;
    }*/
    input.push(ESC);
}

void TextEdit::writeEnter() {
  enter = true;
  this->moveCursor(QTextCursor::EndOfBlock);
  this->printText("\n");
}

void TextEdit::manageBlockCount(int newBlockCount) {
  int diff = newBlockCount - rowsCount;
  if (diff > 0)
    while (diff--)
      this->deleteLastLine();
}

int TextEdit::lastBlockNumber() {
  QTextCursor end = this->textCursor();
  end.movePosition(QTextCursor::End);
  return end.blockNumber();
}

void TextEdit::deleteLastLine(void) {
  QTextCursor storeCursorPos = this->textCursor();
  this->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  this->moveCursor(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
  //this->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
  this->textCursor().removeSelectedText();
  this->textCursor().deletePreviousChar();
  this->setTextCursor(storeCursorPos);
}

void TextEdit::deleteFirstLine(void) {
  QTextCursor storeCursorPos = this->textCursor();
  this->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
  this->moveCursor(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
  this->textCursor().removeSelectedText();
  this->textCursor().deleteChar();
  this->setTextCursor(storeCursorPos);
}

//##########################################
void TextEdit::setTextEditSettings(void){
    qApp->setCursorFlashTime(settingDialog->getSettings()->value("cursorBlink").toInt() * 1000);
}

void TextEdit::initialOutputWindow(void){
    outputWindow = new QWidget();
    outputWindow->setWindowTitle("Output");
    cmdTextEdit = new QPlainTextEdit();
    cmdTextEdit->setReadOnly(true);
    QPushButton * clearOutputWindow = new QPushButton;
    clearOutputWindow->setText("clear");
    QVBoxLayout * layout = new QVBoxLayout;

    layout->addWidget(clearOutputWindow);
    layout->addWidget(cmdTextEdit);
    outputWindow->setLayout(layout);
    connect(clearOutputWindow, &QPushButton::clicked, cmdTextEdit, &QPlainTextEdit::clear);
}

void TextEdit::enterInsertMode(void){
    mode = TextEdit::InsertMode;
    emit newStatusMessage("Insert mode");
}

void TextEdit::enterCommandMode(void){
    if(enter){
            manageBlockCount(this->document()->blockCount());
            enter = false;
    }
    mode = TextEdit::NormalMode;
    emit newStatusMessage("Normal mode");
}

void TextEdit::enterExMode(void){
    mode = TextEdit::ExMode;
    emit newStatusMessage("*Normal mode");
}

void TextEdit::enterAfterCmdMode(void){
    mode = TextEdit::AfterCmdMode;
    emit newStatusMessage("+Normal mode");
}
void TextEdit::writeTilda(char * s) {
    //useless
  QString t = s;
  QTextCursor cursor = this->textCursor();
  cursor.insertText(t);
  setTextCursor(cursor);
  //this->insertPlainText(s);
}

void TextEdit::writeMsg(char * s) {
  QString t = s;
  QTextCharFormat opt;
  opt.setForeground(Qt::red);
  QTextCursor cursor = this->textCursor();
  cursor.setCharFormat(opt);
  cursor.insertText(t);
  setTextCursor(cursor);
}
void TextEdit::writeOutput(void) {
    QFile file("/tmp/outFile.txt");
          if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
              return;
          }
          bool s = settingDialog->getSettings()->value("popupCmdOut").toBool();
          if(s){
              QByteArray line = file.readLine();
              if(line.isEmpty())
                  return;
              outputWindow->show();
              QTextCursor c = cmdTextEdit->textCursor();
              c.movePosition(QTextCursor::Start);
              cmdTextEdit->setTextCursor(c);
              cmdTextEdit->insertPlainText("*******************\n");
              cmdTextEdit->insertPlainText(line);
              while (!file.atEnd()) {
                  line = file.readLine();
                  cmdTextEdit->insertPlainText(line);
              }
          }
          else{
              while (!file.atEnd()) {
                  QByteArray line = file.readLine();
                  this->insertPlainText(line);
              }
          }
}
