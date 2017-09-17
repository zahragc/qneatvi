#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include "neatvi/vi.h"

class FindDialog : public QDialog
{
    Q_OBJECT
public:
    FindDialog(QWidget * parent = 0);
private:
    QLabel * findLabel;
    QLabel * replaceLabel;
    QLineEdit * findLineEdit;
    QLineEdit * replaceLineEidit;
    QCheckBox * fromStartCheckBox;
    QCheckBox * wholeWordCheckBox;
    QCheckBox * caseCheckBox;
    QGroupBox * directionGroupBox;
    QRadioButton * upRadioButton;
    QRadioButton * downRadioButton;
    QLabel * goToThe;
    QPushButton * start;
    QPushButton * end;
    QPushButton * findButton;
    QPushButton * replaceButton;
    QPushButton * replaceAllButton;
    QPushButton * closeButton;

    QFormLayout *formLayout;
    QVBoxLayout * groupBoxVLayout;
    QVBoxLayout * buttonsVLayout;
    QGridLayout * mainGridLayout;

    void createDialog(void);
    void isText(const QString &text);
    void findCommandSend(void);
    void replaceCommandSend(void);
    void replaceAllCommandSend(void);
    void getInfo(void);
    void caseSensitiveFunction(void);
    void goToStart(void);
    void goToEnd(void);

    bool wholeWord;
    bool caseSensitive;
    bool fromStart;
    bool isUp;

    QString findString;
    QString replaceString;
    QString findCommand;
    QString findAndReplaceCommand;
    QString findAndReplaceAllCommand;
    QString caseCommand;

    QString const caseSensitiveCommand = "\x1B:set noic\n";
    QString const ignoreCaseSensitiveCommand = "\x1B:set ic\n";

signals:
    void commandReady(const QString & command);
};

#endif // FINDDIALOG_H
