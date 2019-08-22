#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMainWindow>
#include <qlabel.h>
#include <QClipboard>
#include "textedit.h"
#include "toolbar.h"
#include "finddialog.h"
class MainWindow : public QMainWindow {
  Q_OBJECT

private:
    QLabel * modeLabel;
    QLabel * fileNameLabel;
    ToolBar * toolbar;
    TextEdit *textEdit;
public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void moveWindowToCenter(void);
  void createToolBar(void);
  void createStatusBar(void);
  void debug(char *s);
  void debug(int n);
  void checkTextEditSelection(void);
  void minimize(void);
  void closeEvent(QCloseEvent *e);
  TextEdit* getTextEdit(void);
};

#endif // MAINWINDOW_H
