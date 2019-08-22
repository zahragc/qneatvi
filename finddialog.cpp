#include "finddialog.h"
FindDialog::FindDialog(QWidget * parent) : QDialog(parent){
    setWindowTitle(tr("Find/Replace"));
    setMaximumHeight(height());
    createDialog();
}

void FindDialog::findCommandSend(void){
    getInfo();
    caseSensitiveFunction();
    findCommand = "";
    findCommand = isUp ? "\x1B?" : "\x1B/";
    findCommand += wholeWord ? "\\<" + findString + "\\>" : findString;
    findCommand += "\n";
    emit commandReady(caseCommand + findCommand);
}

void FindDialog::replaceCommandSend(void){
    getInfo();
    caseSensitiveFunction();
    findCommand = "";
    findCommand += wholeWord ? "\\<" + findString + "\\>" : findString;
    findAndReplaceCommand = "\x1B:.,$s/" + findCommand + "/" + replaceString + "\n";
    emit commandReady(caseCommand + findAndReplaceCommand/* + "\x1B/" +findCommand+"\n"*/);
}

void FindDialog::replaceAllCommandSend(void){
    getInfo();
    caseSensitiveFunction();
    findCommand = "";
    //findCommand += isUp ? "\x1B?" : "\x1B/";
    findCommand += wholeWord ? "\\<" + findString + "\\>" : findString;
    findAndReplaceAllCommand = "\x1B:%s/" + findCommand + "/" + replaceString + "/g\n";
    emit commandReady(caseCommand + findAndReplaceAllCommand);
}

void FindDialog::caseSensitiveFunction(void){
    caseCommand = "";
    if(caseSensitive && xic == 1)
        caseCommand = caseSensitiveCommand;
    else if(!caseSensitive && xic == 0)
        caseCommand = ignoreCaseSensitiveCommand;
}

void FindDialog::getInfo(void){
    caseSensitive = caseCheckBox->isChecked();
    wholeWord = wholeWordCheckBox->isChecked();
    fromStart = fromStartCheckBox->isChecked();
    isUp = upRadioButton->isChecked();
    findString = findLineEdit->text();
    replaceString = replaceLineEidit->text();
}

void FindDialog::isText(const QString &text){
    if(text == ""){
        findButton->setEnabled(false);
        replaceButton->setEnabled(false);
        replaceAllButton->setEnabled(false);
    }else{
        findButton->setEnabled(true);
        replaceButton->setEnabled(true);
        replaceAllButton->setEnabled(true);
    }
}

void FindDialog::goToStart(void){
    emit commandReady("\x1B 1G");
    downRadioButton->setChecked(true);
}

void FindDialog::goToEnd(void){
    emit commandReady("\x1B G$");
    upRadioButton->setChecked(true);
}

void FindDialog::createDialog(void){
    findLabel = new QLabel(tr("Sear&ch for:"));
    findLineEdit = new QLineEdit();
    findLabel->setBuddy(findLineEdit);
    replaceLabel = new QLabel(tr("&Replace with:"));
    replaceLineEidit = new QLineEdit();
    replaceLabel->setBuddy(replaceLineEidit);
    formLayout = new QFormLayout;
    formLayout->addRow(findLabel, findLineEdit);
    formLayout->addRow(replaceLabel, replaceLineEidit);

    fromStartCheckBox = new QCheckBox(tr("Search from &start"));
    wholeWordCheckBox = new QCheckBox(tr("Whole words &only"));
    caseCheckBox = new QCheckBox(tr("Case sensiti&ve"));
    goToThe = new QLabel(tr("Go to the:"));;
    start = new QPushButton("Start");
    end = new QPushButton("End");

    directionGroupBox = new QGroupBox(tr("Direction"));
    upRadioButton = new QRadioButton(tr("&Up"));
    upRadioButton->setChecked(true);
    downRadioButton = new QRadioButton(tr("&Down"));
    groupBoxVLayout = new QVBoxLayout;
    groupBoxVLayout->addWidget(upRadioButton);
    groupBoxVLayout->addWidget(downRadioButton);
    directionGroupBox->setLayout(groupBoxVLayout);

    findButton = new QPushButton(tr("Find"));
    findButton->setDefault(true);
    replaceButton = new QPushButton(tr("Replace"));
    replaceAllButton = new QPushButton(tr("Replace all"));
    closeButton = new QPushButton(tr("Close"));
    findButton->setEnabled(false);
    replaceButton->setEnabled(false);
    replaceAllButton->setEnabled(false);

    connect(findButton, &QPushButton::clicked, this, &FindDialog::findCommandSend);
    connect(replaceButton, &QPushButton::clicked, this, &FindDialog::replaceCommandSend);
    connect(replaceAllButton, &QPushButton::clicked, this, &FindDialog::replaceAllCommandSend);
    connect(findLineEdit, &QLineEdit::textChanged, this, &FindDialog::isText);
    connect(closeButton, &QPushButton::clicked, this, &FindDialog::close);
    connect(start, &QPushButton::clicked, this, &FindDialog::goToStart);
    connect(end, &QPushButton::clicked, this, &FindDialog::goToEnd);

    buttonsVLayout = new QVBoxLayout;
    buttonsVLayout->setMargin(0);
    buttonsVLayout->addWidget(findButton);
    buttonsVLayout->addWidget(replaceButton);
    buttonsVLayout->addWidget(replaceAllButton);
    buttonsVLayout->addWidget(closeButton);
    findButton->setMaximumWidth(200);
    replaceButton->setMaximumWidth(200);
    replaceAllButton->setMaximumWidth(200);
    closeButton->setMaximumWidth(200);

    mainGridLayout = new QGridLayout(this);
    mainGridLayout->addLayout(formLayout,0,0,2,3);
    mainGridLayout->addWidget(wholeWordCheckBox,2,0,1,2);
    mainGridLayout->addWidget(caseCheckBox,3,0,1,2);
    mainGridLayout->addWidget(goToThe,4,0,1,2);
    mainGridLayout->addWidget(start,5,0,1,1);
    mainGridLayout->addWidget(end,5,1,1,1);
    mainGridLayout->addWidget(directionGroupBox,2,2,3,1);
    mainGridLayout->addLayout(buttonsVLayout,0,3,4,1);
    setLayout(mainGridLayout);
}
