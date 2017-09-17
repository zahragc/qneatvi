#include "textedit.h"
#include "neatvi/vi.h"
#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
bool TextEdit::quitNeatvi(void){
    guiEvent = true;
    if(mode != NormalMode)
        input.push(ESC);
    exeCommand(":q\n");
    return true;
}

void TextEdit::openFunction(void) {
    guiEvent = true;
  QString fileName =
      QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath());
  if (fileName != "")
    exeCommand("\x1B:e " + fileName + "\n");
}

void TextEdit::readFunction(void) {
  QString fileName =
      QFileDialog::getOpenFileName(this, tr("Read File"), QDir::homePath());
  if (fileName != "")
    exeCommand("\x1B:r " + fileName + "\n");
}

void TextEdit::newFileFunction(void){
    guiEvent = true;
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Make New File"), QDir::homePath() + "/unnamed");
    if (fileName != "")
      exeCommand("\x1B:e " + fileName + "\n");
}

bool TextEdit::saveFunction(void) {
  QString path = ex_path();
  if (path == "") {
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Save File"), QDir::homePath() + "/unnamed");
    if (fileName != "")
      exeCommand("\x1B:w! " + fileName + "\n");
    else
        return false;
  } else {
    exeCommand("\x1B:w\n");
  }
  return true;
}

void TextEdit::saveAsFunction(void) {
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save File As"), QDir::homePath() + "/unnamed");
  if (fileName != "")
    exeCommand("\x1B:w! " + fileName + "\n");
}

void TextEdit::undoFunction(void) {
  if (mode != NormalMode)
    input.push(ESC);
  input.push('u');
  if(wait) emit inputReady();
}

void TextEdit::redoFunction(void) {
  if (mode != NormalMode)
    input.push(ESC);
  input.push(CTRL_R);
  if(wait) emit inputReady();
}

void TextEdit::copyFunction(void) {
  clipboard = QApplication::clipboard();
  clipboard->clear();
  clipText = new QMimeData;
  QString c = this->textCursor().selectedText().remove(setDir);
  // replace Unicode Character 'PARAGRAPH SEPARATOR' with \n
  c = c.replace(QChar(0x2029), QChar(10));
  clipText->setText(c);
  clipboard->setMimeData(clipText);
  // clipboard->setText(this->textCursor().selectedText().remove(setDir));
}

void TextEdit::pasteFunction(void) {
  QString t;
  clipboard = QApplication::clipboard();
  mimeData = clipboard->mimeData();
  if (!mimeData->hasText())
      return;
    t = mimeData->text();
    bool changeKmap = false;
    if (mode == TextEdit::NormalMode) {
        if(xkmap > 0){
            changeKmap = true;
            exeCommand(":cm=en\n");
        }
      input.push('a');
      if(wait) emit inputReady();
      pasteToTextEdit(t);
      // input.push(ESC);
      // if(wait) emit inputReady();
    } else {
        if(xkmap > 0){
            changeKmap = true;
            input.push(CTRL_E);
            if(wait) emit inputReady();
        }
      pasteToTextEdit(t);
    }
    if(changeKmap){
        changeKmap = false;
        input.push(CTRL_F);
        if(wait) emit inputReady();
    }
}

void TextEdit::pasteToTextEdit(QString text) {
  QByteArray a = text.toUtf8();
  int length = a.length();
  for (int i = 0; i < length; i++){
    input.push(a[i]);
  }
  paste = true;
  if(wait) emit inputReady();
}

void TextEdit::refreshFunction(void) {
  if (mode != TextEdit::NormalMode)
    input.push(ESC);

  input.push(CTRL_L);
  if (wait)
    if(wait) emit inputReady();
}

void TextEdit::findReplaceFunction(void) {
    findDialog->show();
}

void TextEdit::previousFunction(void) {
  if (mode != TextEdit::NormalMode) {
    input.push(ESC);
  }
  input.push('?');
  input.push('\n');
  if(wait) emit inputReady();
}

void TextEdit::nextFunction(void) {
  if (mode != TextEdit::NormalMode) {
    input.push(ESC);
  }
  input.push('/');
  input.push('\n');
  if(wait) emit inputReady();
}

void TextEdit::exeCommand(const QString &command) {
  QChar c;
  this->activateWindow();
  QByteArray a = command.toUtf8();
  for (int i = 0; i < a.length(); i++)
    input.push(a[i]);
  if(wait) emit inputReady();
}

void TextEdit::settingFunction(void){

    settingDialog->show();
}

void TextEdit::alarmNotSave(char *c){
    if(!guiEvent)
        return;
    guiEvent = false;
    QString command = c;
    command = ":" + command + "\n";
    QMessageBox msgBox;
    msgBox.setWindowTitle(" ");
     msgBox.setText("<b>The document has been modified.</b>");
     msgBox.setInformativeText("Do you want to save your changes?");
     msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
     msgBox.setDefaultButton(QMessageBox::Save);
     int ret = msgBox.exec();
     switch (ret) {
         case QMessageBox::Save:
             // Save was clicked
             if(saveFunction())
                exeCommand(command);
             break;
         case QMessageBox::Discard:
             exeCommand(command.insert(2,'!'));
             // Don't Save was clicked
             break;
         case QMessageBox::Cancel:
             // Cancel was clicked
             break;
         default:
             // should never be reached
             break;
       }
}
//***************************************************//

void TextEdit::mousePressEvent(QMouseEvent *e) {
  if (mode != TextEdit::NormalMode)
    e->ignore();
  else
    QPlainTextEdit::mousePressEvent(e);
}

void TextEdit::mouseDoubleClickEvent(QMouseEvent *e) {
  if (mode != TextEdit::NormalMode)
    e->ignore();
  else
    QPlainTextEdit::mouseDoubleClickEvent(e);
}

void TextEdit::mouseReleaseEvent(QMouseEvent *e) {
    if(e->button()==Qt::MiddleButton){
        e->ignore();
        return;
    }
  if (mode != TextEdit::NormalMode)
    e->ignore();
  else
    QPlainTextEdit::mouseReleaseEvent(e);
}

void TextEdit::mouseMoveEvent(QMouseEvent *e) {
  if (mode != TextEdit::NormalMode)
    e->ignore();
  else
    QPlainTextEdit::mouseMoveEvent(e);
}

void TextEdit::resizeEvent(QResizeEvent *e) { QPlainTextEdit::resizeEvent(e); }
void TextEdit::wheelEvent(QWheelEvent *e) {
    if (mode == TextEdit::NormalMode) {
      if(e->angleDelta().y()>0){
          input.push('k');
          input.push('k');
          input.push('k');
      }
      else{
          input.push('j');
          input.push('j');
          input.push('j');
      }
      if(wait) emit inputReady();
    }
    QPlainTextEdit::wheelEvent(e);
}

void TextEdit::contextMenuEvent(QContextMenuEvent *e) { e->ignore(); }
void TextEdit::dragEnterEvent(QDragEnterEvent *e) { e->ignore(); }
void TextEdit::dragLeaveEvent(QDragLeaveEvent *e) { e->ignore(); }
void TextEdit::dragMoveEvent(QDragMoveEvent *e) { e->ignore(); }
void TextEdit::dropEvent(QDropEvent *e) { e->ignore(); }
