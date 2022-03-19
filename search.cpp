#include "search.h"
#include "./ui_search.h"
#include "mysql++/mysql++.h"
#include <vector>
#include <stdio.h>

#define MAX_RESULTS 50

using namespace mysqlpp;
extern Connection conn;


search::search(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::search)
{
    ui->setupUi(this);
    this->setWindowTitle("图书查询");
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
    QObject::connect(ui->searchButton,SIGNAL(clicked()),this,SLOT(query()));
}

search::~search()
{
    delete ui;
}


void search::query(){
    std::string sql="select * from book where ";
    std::string sort="order by ";
    std::map<std::string,std::string> where;
    model->removeRows(0,model->rowCount());
    QString cat,name,pub,year_low,year_high,author,price_low,price_high;
    cat=ui->category->text();
    name=ui->name->text();
    pub=ui->publisher->text();
    year_low=ui->yearlow->text();
    year_high=ui->yearhigh->text();
    author=ui->author->text();
    price_low=ui->pricelow->text();
    price_high=ui->pricehigh->text();
    if(cat.size()){
        where.insert(std::pair<std::string,std::string>("category=",cat.toStdString()));
    }
    if(name.size()){
        where.insert(std::pair<std::string,std::string>("title=",name.toStdString()));
    }
    if(pub.size()){
        where.insert(std::pair<std::string,std::string>("press=",pub.toStdString()));
    }
    if(year_low.size()){
        where.insert(std::pair<std::string,std::string>("year>=",year_low.toStdString()));
    }
    if(year_high.size()){
        where.insert(std::pair<std::string,std::string>("year<=",year_high.toStdString()));
    }
    if(author.size()){
        where.insert(std::pair<std::string,std::string>("author=",author.toStdString()));
    }
    if(price_low.size()){
        where.insert(std::pair<std::string,std::string>("price>=",price_low.toStdString()));
    }
    if(price_high.size()){
        where.insert(std::pair<std::string,std::string>("price<=",price_high.toStdString()));
    }
    size_t cnt=0;
    for(std::map<std::string,std::string>::iterator it=where.begin();it!=where.end();it++){
        sql+=it->first+"'"+it->second+"'"+" ";
        cnt++;
        if(cnt<where.size()){
            sql+="and ";
        }
    }
    sort+="bno asc";
    sql+=sort;
    sql+=";";
    if(where.size()==0) sql="select * from book order by bno asc;";
    //QMessageBox::information(NULL,"",QString::fromStdString(sql),QMessageBox::Ok);
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
                char buf[100];
                sprintf(buf,"%s",res[i][j].c_str());
                std::string temp=buf;
                list << new QStandardItem(QString::fromStdString(temp));
            }
            model->insertRow(i, list);
        }
    }
}
