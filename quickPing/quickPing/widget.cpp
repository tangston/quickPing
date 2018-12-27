#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_startButton_clicked()
{
    myPing mp =myPing();
    // QString ip="10.1.11.70";
    QString ip=ui->ipEdit->toPlainText();//获取文本框的函数
    if(ip==NULL){
        QMessageBox box;
        box.setText(tr("please type your message"));
        box.exec();
    }
    else{
        QStringList list=ip.split(".");
        QString str=list[3];

        int m=str.toInt( );
        for(int i=m;i<  m+10;++i)
        {
             mp.ipCount(ip,i);
            if( mp.ping(ip) ){
                ui->listWidget->addItem(ip+":            on ");
                qDebug()<<"ping yes";
            }
            else {
                ui->listWidget->addItem(ip+":            offline");
                qDebug()<<"ping no";
            }
        }
    }

}
void Widget::autoScroll()//自动滚屏
{

}

void Widget::on_exitButton_clicked()
{
    ::exit(0);
}
