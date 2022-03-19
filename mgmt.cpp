#include "mgmt.h"
#include "./ui_mgmt.h"
#include "mysql++/mysql++.h"
#include "license.h"
#include "store.h"
#include "search.h"
#include "transact.h"

using namespace mysqlpp;
extern Connection conn;

mgmt::mgmt(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mgmt)
{
    ui->setupUi(this);
    this->setWindowTitle("图书管理系统");
    QObject::connect(ui->exitSys,SIGNAL(clicked()),this,SLOT(jmpEXIT()));
    QObject::connect(ui->licenseMgmt,SIGNAL(clicked()),this,SLOT(jmpLIC_MGMT()));
    QObject::connect(ui->storeBook,SIGNAL(clicked()),this,SLOT(jmpSTORE()));
    QObject::connect(ui->queryBook,SIGNAL(clicked()),this,SLOT(jmpSEARCH()));
    QObject::connect(ui->lend,SIGNAL(clicked()),this,SLOT(jmpTRANS()));
}

mgmt::~mgmt()
{
    delete ui;
}

void mgmt::jmpEXIT(){
    conn.disconnect();
    exit(0);
}

void mgmt::jmpLIC_MGMT(){
    license *l=new license(0);
    l->show();
}

void mgmt::jmpSTORE(){
    store *s=new store(0);
    s->show();
}

void mgmt::jmpSEARCH(){
    search *se=new search(0);
    se->show();
}


void mgmt::jmpTRANS(){
    transact *t=new transact(0);
    t->show();
}
