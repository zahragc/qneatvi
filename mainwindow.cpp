#include "mainwindow.h"
#include "toolbar.h"
#include <QStatusBar>
#define TAB "&nbsp;&nbsp;&nbsp;&nbsp;"
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  textEdit = new TextEdit(this);
  setCentralWidget(textEdit);
  setWindowTitle("qneatvi");
  this->moveWindowToCenter();
  createToolBar();
  createStatusBar();
}

MainWindow::~MainWindow() { delete textEdit; }

TextEdit *MainWindow::getTextEdit(void){
    return textEdit;
}

void MainWindow::moveWindowToCenter(void) {
  QDesktopWidget *desktopInfo = QApplication::desktop();
  // QRect screenGeometry = desktopInfo->screenGeometry();
  int screenWidth = desktopInfo->width();
  int screenHeight = desktopInfo->height();
  resize(int(screenWidth/1.7), int(screenHeight/1.7));
  move(screenWidth / 4, screenHeight / 10);
}
void MainWindow::minimize(void){
    this->setWindowState(Qt::WindowMinimized);
}

void MainWindow::createStatusBar(void){
    modeLabel = new QLabel("Normal mode");
    fileNameLabel = new QLabel("");
    statusBar()->addWidget(modeLabel);
    statusBar()->addPermanentWidget(fileNameLabel);
    connect(textEdit, &TextEdit::newStatusMessage, this->modeLabel, &QLabel::setText);
}

void MainWindow::createToolBar(void) {
    toolbar = new ToolBar(this);
    addToolBar(toolbar);
      connect(toolbar, &ToolBar::openSignal, this->textEdit , &TextEdit::openFunction);
      connect(toolbar, &ToolBar::readSignal, this->textEdit , &TextEdit::readFunction);
      connect(toolbar, &ToolBar::newFileSignal, this->textEdit , &TextEdit::newFileFunction);
      connect(toolbar, &ToolBar::saveSignal, this->textEdit , &TextEdit::saveFunction);
      connect(toolbar, &ToolBar::saveAsSignal, this->textEdit , &TextEdit::saveAsFunction);

      connect(toolbar, &ToolBar::undoSignal, this->textEdit , &TextEdit::undoFunction);
      connect(toolbar, &ToolBar::redoSignal, this->textEdit , &TextEdit::redoFunction);

      connect(toolbar, &ToolBar::copySignal, this->textEdit , &TextEdit::copyFunction);
      connect(toolbar, &ToolBar::pasteSignal, this->textEdit , &TextEdit::pasteFunction);
      connect(textEdit, &QPlainTextEdit::selectionChanged, this, &MainWindow::checkTextEditSelection);

      connect(toolbar, &ToolBar::searchSignal, this->textEdit, &TextEdit::findReplaceFunction);
      connect(toolbar, &ToolBar::previousSignal, this->textEdit, &TextEdit::previousFunction);
      connect(toolbar, &ToolBar::nextSignal, this->textEdit, &TextEdit::nextFunction);

      connect(toolbar, &ToolBar::refreshSignal, this->textEdit, &TextEdit::refreshFunction);
      connect(toolbar, &ToolBar::settingSignal, this->textEdit, &TextEdit::settingFunction);
}
void MainWindow::checkTextEditSelection(void){
    if(textEdit->textCursor().selectedText() == "")
        toolbar->selectionIsReady(false);
    else
        toolbar->selectionIsReady(true);
}

void MainWindow::debug(char *s) { qDebug() << s; }

void MainWindow::debug(int n) { qDebug() << n; }

void MainWindow::closeEvent(QCloseEvent *e) {
    e->ignore();
    textEdit->quitNeatvi();
}
