#include "toolbar.h"
#include <QMessageBox>
ToolBar::ToolBar(QWidget *parent): QToolBar (parent)
{
    setMovable(false);
    createActions();
    setConnections();
}
void ToolBar::clipboardIsReady(const bool& a){
    pasteAction->setEnabled(a);
}

void ToolBar::selectionIsReady(const bool& a){
    copyAction->setEnabled(a);
}

void ToolBar::checkClipboard(void){
    clipboard = QGuiApplication::clipboard();
    clipboardIsReady(clipboard->mimeData()->hasText());
}

void ToolBar::setConnections(void){
    connect(aboutQtAction, &QAction::triggered, this, &ToolBar::showAboutQt);
    connect(aboutNeatviAction, &QAction::triggered, this, &ToolBar::showAboutNeatvi);
    connect(helpAction, &QAction::triggered, this, &ToolBar::showHelp);

    connect(openAction, &QAction::triggered, this, &ToolBar::openSignal);
    connect(readAction, &QAction::triggered, this, &ToolBar::readSignal);
    connect(newFileAction, &QAction::triggered, this, &ToolBar::newFileSignal);
    connect(saveAction, &QAction::triggered, this, &ToolBar::saveSignal);
    connect(saveAsAction, &QAction::triggered, this, &ToolBar::saveAsSignal);

    connect(undoAction, &QAction::triggered, this, &ToolBar::undoSignal);
    connect(redoAction, &QAction::triggered, this, &ToolBar::redoSignal);

    connect(copyAction, &QAction::triggered, this, &ToolBar::copySignal);
    connect(pasteAction, &QAction::triggered, this, &ToolBar::pasteSignal);

    connect(searchAction, &QAction::triggered, this, &ToolBar::searchSignal);
    connect(previousAction, &QAction::triggered, this, &ToolBar::previousSignal);
    connect(nextAction, &QAction::triggered, this, &ToolBar::nextSignal);

    connect(refreshAction, &QAction::triggered, this, &ToolBar::refreshSignal);
    connect(settingAction, &QAction::triggered, this, &ToolBar::settingSignal);

    connect(clipboard, &QClipboard::dataChanged, this, &ToolBar::checkClipboard);
}

void ToolBar::showAboutQt(void){
    QMessageBox::aboutQt(this,"About Qt");
}

void ToolBar::showAboutNeatvi(void){
    QMessageBox::about(this, "About Neatvi",
                       "<h3>About Neatvi</h3>"
                       "Neatvi is a vi/ex editor. It can edit bidirectional UTF-8 text. "
                       "<br>See <a href=http://rudi.ir>rudi.ir</a> for more information.");
}

void ToolBar::showHelp(void){
    helpWidget = new HelpWidget();
    helpWidget->show();
}

void ToolBar::createActions(void){
    QPixmap open(":/icons/open.png");
    QPixmap read(":/icons/read.png");
    QPixmap newFile(":/icons/new.png");
    QPixmap save(":/icons/save.png");
    QPixmap saveAs(":/icons/saveAs.png");

    QPixmap undo(":/icons/undo.png");
    QPixmap redo(":/icons/redo.png");

    QPixmap copy(":/icons/copy.png");
    QPixmap paste(":/icons/paste.png");

    QPixmap search(":/icons/search.png");
    QPixmap previous(":/icons/previous.png");
    QPixmap next(":/icons/next.png");

    QPixmap refresh(":/icons/refresh.png");
    QPixmap setting(":/icons/setting.png");
    QPixmap help(":/icons/help.png");

  //--------------
  openAction = addAction(QIcon(open), "Open file");
  readAction = addAction(QIcon(read), "Read file");
  newFileAction = addAction(QIcon(newFile), "Make file");
  saveAction = addAction(QIcon(save), "Save current file");
  saveAsAction = addAction(QIcon(saveAs), "Save current file As ... ");
  addSeparator();
  //--------------
  undoAction = addAction(QIcon(undo), "Undo");
  redoAction = addAction(QIcon(redo), "Redo");
  addSeparator();
  //---------------
  copyAction = addAction(QIcon(copy), "Copy to clipboard");
  pasteAction = addAction(QIcon(paste), "Paste from clipboard");
  copyAction->setEnabled(false);
  checkClipboard();
  addSeparator();
  //----------------
  searchAction = addAction(QIcon(search), "Find/Replace");
  previousAction = addAction(QIcon(previous), "Find previous");
  nextAction = addAction(QIcon(next), "Find next");
  addSeparator();
  //---------------
  refreshAction = addAction(QIcon(refresh), "Refresh screen");
  settingAction = addAction(QIcon(setting), "Settings");

  helpMenu = new QMenu;
  helpAction = helpMenu->addAction("Help");
  helpMenu->addSeparator();
  aboutNeatviAction = helpMenu->addAction("About Neatvi");
  aboutQtAction = helpMenu->addAction("About Qt");

  helpButton = new QToolButton;
  helpButton->setToolTip("Help");
  helpButton->setIcon(QIcon(help));
  helpButton->setMenu(helpMenu);
  helpButton->setAutoRaise(true);
  helpButton->setPopupMode(QToolButton::InstantPopup);
  addWidget(helpButton);
}
