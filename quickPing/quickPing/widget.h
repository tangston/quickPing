#ifndef WIDGET_H
#define WIDGET_H
#include <QString>
#include <QWidget>
#include <QString>
#include "myping.h"
#include <QMessageBox>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void Widget::autoScroll();
    QString Widget::getIP2() ;

private slots:
    void on_startButton_clicked();

    void on_exitButton_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
