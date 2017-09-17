#ifndef HELPWIDGET_H
#define HELPWIDGET_H
#include <QTabWidget>
#include <QScrollArea>
#include <QLabel>

class HelpWidget : public QTabWidget
{
public:
    HelpWidget(QWidget * parent = 0);
private:
    void setTabText(void);
    QScrollArea  confArea;
    QScrollArea  comArea;
    QScrollArea  optAria;
    QScrollArea  markArea;

    QLabel configuration ;
    QLabel commands ;
    QLabel options ;
    QLabel marksAndBuffers;
};

#endif // HELPWIDGET_H
