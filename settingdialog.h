#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QSlider>
#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
class SettingDialog : public QDialog
{
public:
    SettingDialog(QWidget * parent = nullptr);
    QSettings * getSettings(void);
private:
    void initialTextEditSetting(void);
    void createDialog(void);
    void initialDialog(void);
    void accept(void);
    void reject(void);
    QLabel * blinkLabel;
    QLabel * onLabel;
    QSlider * blinkSlider;
    QCheckBox * popupCheckBox;
    QPushButton * okButton;
    QPushButton * cancelButton;
    QHBoxLayout * hLayout1;
    QHBoxLayout * hLayout2;
    QVBoxLayout * vLayout;
    QSpacerItem * spacer;
    QSpacerItem * spacerHeight;
    QSettings * settings;
};

#endif // SETTING_H
