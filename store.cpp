#include "store.h"
#include "./ui_store.h"
#include <fstream>
#include <iostream>
#include "mysql++/mysql++.h"
#include <QFileDialog>
#include <QMessageBox>
#include <string>
#include "boost/algorithm/string.hpp"

using namespace mysqlpp;
extern Connection conn;

static void split(const char *s, std::vector<std::string> &strs, char delim = ',') {
    if(s == nullptr) {
        return;
    }
    const char *head, *tail;
    head = tail = s;
    while(*head != '\0') {
        while(*head != '\0' && *head == delim) {
            head++;
        }
        tail = head;
        while(*tail != '\0' && *tail != delim) {
            tail++;
        }
        if(head != tail) {
            strs.push_back(std::string(head, tail));
            head = tail;
        } else {
            break;
        }
    }
}

store::store(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::store)
{
    ui->setupUi(this);
    loadFile=false;
    this->setWindowTitle("图书入库");
    QObject::connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(btnOK()));
    QObject::connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(btnReject()));
    QObject::connect(ui->selectFile,SIGNAL(clicked()),this,SLOT(load()));
}

store::~store()
{
    delete ui;
}

void store::load(){
       fileName = QFileDialog::getOpenFileName(this,tr("选择文件"),"",tr("Text (*.*)"));
       if(fileName.isEmpty()){
              loadFile=false;
              return;
       }
       ui->textBrowser->setText(fileName);
       loadFile=true;
}

void store::btnOK(){
    if(loadFile){
        loadFile=false;
        std::ifstream file(fileName.toStdString());
        if(!file.is_open()){
            QMessageBox::warning(NULL,"","文件打开失败",QMessageBox::Ok);
            return;
        }else{
            char buf[200];
            while(file.getline(buf,200)){
                std::string line(buf);
                if(line.length()<=3){
                    QMessageBox::information(NULL,"","文本格式错误",QMessageBox::Ok);
                    return;
                }
                line=line.substr(1,line.length()-2);
                boost::trim(line);
                std::vector<std::string> lineQuery;
                split(line.c_str(),lineQuery,',');
                for(std::vector<std::string>::iterator it=lineQuery.begin();it!=lineQuery.end();it++){
                    boost::trim(*it);
                }
                memset(buf,0,sizeof(buf));
                std::string checkStock("select bno from book where bno='"+lineQuery[0]+"';");
                Query q=conn.query(checkStock);
                StoreQueryResult res=q.store();
                if(res.empty()){
                    std::string storeBook="insert into book(bno,category,title,press,year,author,price,total,stock)\
                         values('"+lineQuery[0]+"','"+lineQuery[1]+"','"+lineQuery[2]+"','"+lineQuery[3]\
                         +"',"+lineQuery[4]+",'"+lineQuery[5]+"',"+lineQuery[6]+","+lineQuery[7]+","+lineQuery[8]+");";
                    q.clear();
                    q=conn.query(storeBook);
                    q.execute();
                }else{
                    std::string checkNum("select stock from book where bno='"+lineQuery[0]+"';");
                    q=conn.query(checkNum);
                    res=q.store();
                    std::string stkStr;
                    res[0][0].to_string(stkStr);
                    int stkNum=atoi(stkStr.c_str())+atoi(lineQuery[8].c_str());
                    stkStr=std::to_string(stkNum);
                    std::string updateStock="update book set stock="+stkStr+" where bno='"+lineQuery[0]+"';";
                    q.clear();
                    q=conn.query(updateStock);
                    q.execute();

                    checkNum="select total from book where bno='"+lineQuery[0]+"';";
                    q=conn.query(checkNum);
                    res=q.store();
                    res[0][0].to_string(stkStr);
                    stkNum=atoi(stkStr.c_str())+atoi(lineQuery[8].c_str());
                    stkStr=std::to_string(stkNum);
                    std::string updateTotal="update book set total="+stkStr+" where bno='"+lineQuery[0]+"';";
                    q.clear();
                    q=conn.query(updateTotal);
                    q.execute();
                }
            }
        }
    }else{
        std::string no=ui->no->text().toStdString(),
                category=ui->category->text().toStdString(),
                publisher=ui->publisher->text().toStdString(),
                name=ui->name->text().toStdString(),
                author=ui->author->text().toStdString(),
                year=ui->year->text().toStdString(),stockNum=ui->stocknum->text().toStdString(),
                price=ui->price->text().toStdString(),stk=ui->stk->text().toStdString();
        std::string checkStock("select bno from book where bno='"+no+"';");
        Query q=conn.query(checkStock);
        StoreQueryResult res=q.store();
        if(res.empty()){
            std::string storeBook="insert into book(bno,category,title,press,year,author,price,total,stock)\
                 values('"+no+"','"+category+"','"+name+"','"+publisher\
                 +"',"+year+",'"+author+"',"+price+","+stockNum+","+stk+");";
            q.clear();
            q=conn.query(storeBook);
            q.execute();
        }else{
            std::string checkNum("select stock from book where bno='"+no+"';");
            q=conn.query(checkNum);
            res=q.store();
            std::string stkStr;
            res[0][0].to_string(stkStr);
            int stkNum=atoi(stkStr.c_str())+atoi(stockNum.c_str());
            stkStr=std::to_string(stkNum);
            std::string updateStock="update book set stock="+stkStr+" where bno='"+no+"';";
            q.clear();
            q=conn.query(updateStock);
            q.execute();

            checkNum="select total from book where bno='"+no+"';";
            q=conn.query(checkNum);
            res=q.store();
            res[0][0].to_string(stkStr);
            stkNum=atoi(stkStr.c_str())+atoi(stockNum.c_str());
            stkStr=std::to_string(stkNum);
            std::string updateTotal="update book set total="+stkStr+" where bno='"+no+"';";
            q.clear();
            q=conn.query(updateTotal);
            q.execute();
        }
    }
    QMessageBox::information(NULL,"","入库成功",QMessageBox::Ok);
}

void store::btnReject(){
    loadFile=false;
    this->close();
}
