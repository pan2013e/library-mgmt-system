#include "transact.h"
#include "./ui_transact.h"
#include "mysql++/mysql++.h"
#include <QStandardItemModel>
#include <stdio.h>

using namespace mysqlpp;
extern Connection conn;

#define MAX_RESULTS 50
extern std::string mgName;

transact::transact(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::transact)
{
    ui->setupUi(this);
    this->setWindowTitle("借/还书");
    model=new QStandardItemModel();
    model->setColumnCount(9);
    model->setHeaderData(0,Qt::Horizontal,"书号");
    model->setHeaderData(1,Qt::Horizontal,"类别");
    model->setHeaderData(2,Qt::Horizontal,"书名");
    model->setHeaderData(3,Qt::Horizontal,"出版社");
    model->setHeaderData(4,Qt::Horizontal,"年份");
    model->setHeaderData(5,Qt::Horizontal,"作者");
    model->setHeaderData(6,Qt::Horizontal,"价格");
    model->setHeaderData(7,Qt::Horizontal,"总藏书量");
    model->setHeaderData(8,Qt::Horizontal,"库存");
    ui->result->setModel(model);
    ui->result->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
//    ui->result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->result->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QObject::connect(ui->queryButton,SIGNAL(clicked()),this,SLOT(search()));
    QObject::connect(ui->borrow,SIGNAL(clicked()),this,SLOT(borrow()));
    QObject::connect(ui->ret,SIGNAL(clicked()),this,SLOT(ret()));
}

transact::~transact()
{
    delete ui;
}

void transact::search(){
    model->removeRows(0,model->rowCount());
    QString cno=ui->cardno->text();
    if(cno==""){
        QMessageBox::warning(NULL,"","借书证号为空！",QMessageBox::Ok);
        return;
    }
    std::string sql="select bno,category,title,press,author,year,price,total,stock from book natural join borrow ";
    std::string where="where cno="+cno.toStdString();
    sql+=where+" and return_date is null;";
    Query q=conn.query(sql);
    StoreQueryResult res=q.store();
    if(!res){
        QString err("Error: ");
        err.append(q.error());
        QMessageBox::warning(NULL,"错误",err,QMessageBox::Ok);
    }else if(res.empty()){
        QMessageBox::information(NULL,"错误","查询结果为空",QMessageBox::Ok);
    }else{
        for(size_t i=0;i<res.num_rows();i++){
            if(i==MAX_RESULTS) break;
            QList<QStandardItem*> list;
            for(size_t j=0;j<res[i].size();j++){
                if(res[i].size()==5) continue;
                char buf[100];
                sprintf(buf,"%s",res[i][j].c_str());
                std::string temp=buf;
                list << new QStandardItem(QString::fromStdString(temp));
            }
            model->insertRow(i, list);
        }
    }
}

void transact::ret(){
    QString cno=ui->cardno->text();
    QString bno=ui->bookno->text();
    if(cno==""||bno==""){
        QMessageBox::warning(NULL,"","借书证号或书号为空！",QMessageBox::Ok);
        return;
    }
    std::string check="select * from borrow where cno='"+cno.toStdString()+"' and bno='"+bno.toStdString()+\
            "' and return_date is null;";
    Query q=conn.query(check);
    StoreQueryResult res=q.store();
    if(!res){
        QString err("Error: ");
        err.append(q.error());
        QMessageBox::warning(NULL,"错误",err,QMessageBox::Ok);
    }else if(res.empty()){
        QMessageBox::information(NULL,"错误","无相应借书记录",QMessageBox::Ok);
    }else{
        std::string checkNum("select stock from book where bno='"+bno.toStdString()+"';");
        q=conn.query(checkNum);
        res=q.store();
        std::string stkStr;
        res[0][0].to_string(stkStr);
        int stkNum=atoi(stkStr.c_str())+1;
        stkStr=std::to_string(stkNum);
        std::string updateStock="update book set stock="+stkStr+" where bno='"+bno.toStdString()+"';";
        q=conn.query(updateStock);
        q.execute();
        std::string getTime="select NOW();";
        q=conn.query(getTime);
        StoreQueryResult res_t=q.store();
        std::string ret_timeStamp;
        res_t[0][0].to_string(ret_timeStamp);
        std::string updateRec="update borrow set return_date='"+ret_timeStamp+\
                "' where bno='"+bno.toStdString()+"' and cno='"+cno.toStdString()+"';";
        q=conn.query(updateRec);
        q.execute();
        QMessageBox::information(NULL,"","还书成功",QMessageBox::Ok);
    }
}

void transact::borrow(){
    QString cno=ui->cardno->text();
    QString bno=ui->bookno->text();
    std::string check="select cno from card where cno='"+cno.toStdString()+"';";
    Query q=conn.query(check);
    StoreQueryResult res=q.store();
    if(!res){
        QString err("Error: ");
        err.append(q.error());
        QMessageBox::warning(NULL,"错误",err,QMessageBox::Ok);
        return;
    }else if(res.empty()){
        QMessageBox::information(NULL,"错误","无相应借书证号",QMessageBox::Ok);
        return;
    }
    check="select stock from book where bno='"+bno.toStdString()+"';";
    q=conn.query(check);
    res=q.store();
    if(!res){
        QString err("Error: ");
        err.append(q.error());
        QMessageBox::warning(NULL,"错误",err,QMessageBox::Ok);
    }else if(res.empty()){
        QMessageBox::information(NULL,"错误","查无此书",QMessageBox::Ok);
    }else{
        char buf[100];
        sprintf(buf,"%s",res[0][0].c_str());
        if(atoi(buf)){
            std::string checkNum("select stock from book where bno='"+bno.toStdString()+"';");
            q=conn.query(checkNum);
            res=q.store();
            std::string stkStr;
            res[0][0].to_string(stkStr);
            int stkNum=atoi(stkStr.c_str())-1;
            stkStr=std::to_string(stkNum);
            std::string updateStock="update book set stock="+stkStr+" where bno='"+bno.toStdString()+"';";
            q=conn.query(updateStock);
            q.execute();
            std::string getTime="select NOW();";
            q=conn.query(getTime);
            StoreQueryResult res_t=q.store();
            std::string borrow_timeStamp,ret_timeStamp;
            res_t[0][0].to_string(borrow_timeStamp);
            std::string getID="select mgID from manager where name='"+mgName+"';";
            q=conn.query(getID);
            res=q.store();
            std::string mgID;
            res[0][0].to_string(mgID);
            std::string insertRec="insert into borrow(cno,bno,borrow_date,mgID) values('"+cno.toStdString()+"','"+bno.toStdString()+"','"+
                  borrow_timeStamp+"','"+mgID+"');";
            q=conn.query(insertRec);
            q.execute();
            QMessageBox::information(NULL,"","借书成功",QMessageBox::Ok);
        }else{
            std::string qret="select return_date from borrow where bno='"+bno.toStdString()+"' order by return_date asc;";
            Query retcheck=conn.query(qret);
            StoreQueryResult retd=retcheck.store();
            if(!retd){
                QString err("Error: ");
                err.append(retcheck.error());
                QMessageBox::warning(NULL,"错误",err,QMessageBox::Ok);
            }else if(retd.empty()){
                QMessageBox::warning(NULL,"错误","记录错误，请联系管理员",QMessageBox::Ok);
            }else{
                char buf[100];
                sprintf(buf,"%s",retd[0][0].c_str());
                std::string temp=buf;
                QMessageBox::information(NULL,"","库存为0，最近归还时间："+QString::fromStdString(temp),QMessageBox::Ok);
            }
        }
    }
}
