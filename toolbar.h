#ifndef TOOLBAR_H
#define TOOLBAR_H
#include <QToolBar>
#include <QMenu>
#include <QAction>
#include <QToolButton>
#include <QClipboard>
#include <QMimeData>
#include <QGuiApplication>
#include "helpwidget.h"
class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    ToolBar(QWidget *parent = 0);
private slots:
  void showAboutQt(void);
  void showAboutNeatvi(void);
  void showHelp(void);
public slots:
  void clipboardIsReady(const bool& a);
  void selectionIsReady(const bool& a);

private:
  void createActions(void);
  void setConnections(void);
  void checkClipboard(void);
  QClipboard *clipboard;

  QAction * openAction;
  QAction * readAction;
  QAction * newFileAction;
  QAction * saveAction;
  QAction * saveAsAction;
  QAction * undoAction;
  QAction * redoAction;
  QAction * copyAction;
  QAction * pasteAction;
  QAction * searchAction;
  QAction * previousAction;
  QAction * nextAction;
  QAction * refreshAction;
  QAction * settingAction;

  QToolButton * helpButton;
  QMenu * helpMenu;
  QAction * aboutQtAction;
  QAction * aboutNeatviAction;
  QAction * helpAction;

  HelpWidget * helpWidget;
signals:
  void openSignal(void);
  void readSignal(void);
  void newFileSignal(void);
  void saveSignal(void);
  void saveAsSignal(void);

  void undoSignal(void);
  void redoSignal(void);

  void copySignal(void);
  void pasteSignal(void);

  void searchSignal(void);
  void previousSignal(void);
  void nextSignal(void);

  void refreshSignal(void);
  void settingSignal(void);
};

#endif // TOOLBAR_H
