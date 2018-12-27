#include "widget.h"
#include <QApplication>
#include "myping.h"
#include <qdebug.h>

void ipCount(QString &ip,int index);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}

