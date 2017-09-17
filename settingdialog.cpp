#include "settingdialog.h"

SettingDialog::SettingDialog(QWidget * parent) : QDialog(parent)
{
    setWindowTitle(tr("Settings"));
    setMaximumHeight(height());
    setMaximumWidth(width());
    setModal(true);
    initialTextEditSetting();
    createDialog();
    initialDialog();
}

QSettings * SettingDialog::getSettings(void){
    return settings;
}

void SettingDialog::createDialog(void){
    blinkSlider = new QSlider();
    blinkSlider->setOrientation(Qt::Horizontal);
    blinkSlider->setMinimum(0);
    blinkSlider->setMaximum(1);
    blinkSlider->setFixedWidth(31);
    blinkLabel = new QLabel("Text cursor blink:  Off");
    onLabel = new QLabel("On");

    popupCheckBox = new QCheckBox("Show cmd output in new window.    ");

    spacerHeight = new QSpacerItem(5 , 30);

    spacer = new QSpacerItem(width() , 10);
    okButton = new QPushButton("Ok");
    okButton->setDefault(true);
    cancelButton = new QPushButton("Cancel");

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(blinkLabel);
    hLayout1->addWidget(blinkSlider);
    hLayout1->addWidget(onLabel);

    hLayout2 = new QHBoxLayout;
    hLayout2->addItem(spacer);
    hLayout2->addWidget(cancelButton);
    hLayout2->addWidget(okButton);

    vLayout = new QVBoxLayout;
    vLayout->addLayout(hLayout1);
    vLayout->addWidget(popupCheckBox);
    vLayout->addItem(spacerHeight);
    vLayout->addLayout(hLayout2);
    setLayout(vLayout);
}

void SettingDialog::initialTextEditSetting(void){
    settings = new QSettings;
    if(!settings->contains("cursorBlink")){
        settings->setValue("cursorBlink", 1);
    }
    if(!settings->contains("popupCmdOut")){
        settings->setValue("popupCmdOut", false);
    }
}

void SettingDialog::accept(){
    settings->setValue("cursorBlink", blinkSlider->value());
    settings->setValue("popupCmdOut", popupCheckBox->isChecked());
    QDialog::accept();
}

void SettingDialog::reject(){
    initialDialog();
    QDialog::reject();
}

void SettingDialog::initialDialog(){
    popupCheckBox->setChecked(settings->value("popupCmdOut").toBool());
    blinkSlider->setSliderPosition(settings->value("cursorBlink").toInt());
}

