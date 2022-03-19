#include "mainwindow.h"
#include "login.h"
#include "mysql++/mysql++.h"
#include <QApplication>

using namespace mysqlpp;
Connection conn(false);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    login l;
    bool connStat=conn.connect("LibraryMgmt","localhost","root","Pzy12345");
    if(!connStat){
        QString err="Error: ";
        err.append(conn.error());
        QMessageBox::warning(NULL,"MySQL",err,QMessageBox::Ok);
        exit(-1);
    }
    l.show();
    return a.exec();
}
