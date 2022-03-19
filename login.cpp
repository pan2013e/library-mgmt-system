#include "login.h"
#include "./ui_login.h"
#include "mysql++/mysql++.h"
#include <QString>
#include "mgmt.h"
#include "search.h"

using namespace mysqlpp;
extern Connection conn;

std::string mgName;

login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("图书管理系统"));
    ui->password->setEchoMode(QLineEdit::Password);
    isLogin=false;
    QObject::connect(ui->login_2,SIGNAL(clicked()),this,SLOT(Verification()));
    QObject::connect(ui->search,SIGNAL(clicked()),this,SLOT(jmpSEA()));
}

login::~login()
{
    delete ui;
}

void login::Verification(){
    QString name=ui->userName->text();
    QString password=ui->password->text();
    if(name==""){
        QMessageBox::warning(NULL,"错误","请输入用户名",QMessageBox::Ok);
        return;
    }
    if(password==""){
        QMessageBox::warning(NULL,"错误","请输入密码",QMessageBox::Ok);
        return;
    }
    std::string sql;
    sql="select passwd from manager where name='"+name.toStdString()+"';";
    Query q=conn.query(sql);
    StoreQueryResult res=q.store();
    if(!res){
        QString err("Error: ");
        err.append(q.error());
        QMessageBox::warning(NULL,"错误",err,QMessageBox::Ok);
    }else if(res.empty()){
        QMessageBox::warning(NULL,"错误","用户名不存在",QMessageBox::Ok);
    }else{
        if(res[0].at(0)==password.toStdString()){
            mgName=name.toStdString();
            //QMessageBox::information(NULL,"提示","登陆成功",QMessageBox::Ok);
            this->close();
            isLogin=true;
            mgmt *m=new mgmt(0);
            m->show();
        }else{
            QMessageBox::warning(NULL,"错误","密码错误",QMessageBox::Ok);
        }
    }
}


bool login::logStats(){
    return isLogin;
}

void login::jmpSEA(){
    search *s=new search(0);
    this->close();
    s->show();
}
