#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QDebug>
#include <QClipboard>
#include <QEventLoop>
#include <QMimeData>
#include <QPlainTextEdit>
#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <queue>
#include "finddialog.h"
#include "settingdialog.h"

//#define CTRL_A 1
//#define CTRL_B 2
//#define CTRL_C 3
//#define CTRL_D 4
#define CTRL_E 5
#define CTRL_F 6
//#define CTRL_G 7
//#define CTRL_H 8
//#define CTRL_I 9
//#define CTRL_J 10
//#define CTRL_K 11
#define CTRL_L 12
//#define CTRL_M 13
//#define CTRL_N 14
//#define CTRL_O 15
//#define CTRL_P 16
//#define CTRL_Q 17
#define CTRL_R 18
//#define CTRL_S 19
//#define CTRL_T 20
//#define CTRL_U 21
//#define CTRL_V 22
//#define CTRL_W 23
//#define CTRL_X 24
//#define CTRL_Y 25
//#define CTRL_Z 26
#define ESC 27
//#define SPACE 32

class TextEdit : public QPlainTextEdit {
  Q_OBJECT
public:
  TextEdit(QWidget *parent);
  ~TextEdit();
  void keyPressEvent(QKeyEvent *e);//
  void init(void);
  int cols(void);
  int rows(void);
  void kill(void);
  void pos(int r, int c);
  int read(char *ibuf, int readsize);
  void write(char *s);
  void printText(QString text);
  void parseAnsi(int attribute, QListIterator<QString> &i,
                           QTextCharFormat &textCharFormat);//
  void room(int n);
  void quitEventLoop(void);

 // textedit3
  void writeEnter(void);
  void enterKeyFunction(void);
  void escapeKeyFunction(void);
  int lastBlockNumber();
  void manageBlockCount(int newBlockCount);
  void deleteLastLine();
  void deleteFirstLine();
  void setTextEditFont(void);

   // textedit3
  void setTextEditSettings(void);
  void initialOutputWindow(void);
  void writeTilda(char *s);
  void writeMsg(char *s);
  void writeOutput(void);
  void enterInsertMode(void);
  void enterCommandMode(void);
  void enterExMode(void);
  void enterAfterCmdMode(void);
  bool checkArrow(int key);

  // textedit2
  bool quitNeatvi(void);

  void openFunction(void);
  void readFunction(void);
  void newFileFunction(void);
  bool saveFunction(void);
  void saveAsFunction(void);

  void undoFunction(void);
  void redoFunction(void);

  void findReplaceFunction(void);
  void exeCommand(const QString &command);

  void copyFunction(void);
  void pasteFunction(void);
  void pasteToTextEdit(QString t);

  void refreshFunction(void);
  void previousFunction(void);
  void nextFunction(void);

  void settingFunction(void);
  void alarmNotSave(char *c);
  void resizeEvent(QResizeEvent *e);
  void wheelEvent(QWheelEvent *e);
  void mousePressEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);
  void mouseDoubleClickEvent(QMouseEvent *e);
  void contextMenuEvent(QContextMenuEvent *e);

  void mouseMoveEvent(QMouseEvent *e);
  void dragEnterEvent(QDragEnterEvent *e);
  void dragLeaveEvent(QDragLeaveEvent *e);
  void dragMoveEvent(QDragMoveEvent *e);
  void dropEvent(QDropEvent *e);

  bool getGuiEvent(){ return guiEvent; }
private:      //  0         1        2
  enum Mode { NormalMode, ExMode, InsertMode, AfterCmdMode };
  QWidget * outputWindow;
  QPlainTextEdit *cmdTextEdit;
  QRegularExpression const ansiExpression;
  QRegularExpressionMatch ansiMatch;
  QTextCharFormat  const defaultTextCharFormat;
  std::queue<char> input;
  QString const setDir = QString(QChar(0x202D)); //0x202D
  QEventLoop eventLoop;
  QClipboard *clipboard;
  const QMimeData *mimeData;
  QMimeData *clipText;
  FindDialog *findDialog;
  SettingDialog *settingDialog;
  Mode mode;
  int columnsCount;
  int rowsCount;
  bool wait;
  bool enter;
  bool paste = false;
  bool guiEvent = false;
signals:
  void inputReady(void);
  void newStatusMessage(const QString &message);
};

#endif // TEXTEDIT_H
