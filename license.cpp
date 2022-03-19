#include "license.h"
#include "./ui_license.h"
#include "mysql++/mysql++.h"
#include <QMessageBox>

using namespace mysqlpp;
extern Connection conn;

license::license(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::license)
{
    ui->setupUi(this);
    this->setWindowTitle("借书证管理");
    QObject::connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(btnOK()));
    QObject::connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(btnReject()));
    //QObject::connect(ui->comboBox,SIGNAL(TextChanged()),this,SLOT(textSetDisable()));
}

license::~license()
{
    delete ui;
}

void license::btnOK(){
    std::string sql;
    std::string type;
    if(ui->type->currentText()=="教师") type="teacher";
    else if(ui->type->currentText()=="学生") type="student";
    else type="others";
    if(ui->comboBox->currentText().toStdString()=="添加"){
        sql="insert into card(cno,name,dept,type) values('"+ui->ID->text().toStdString()
                +"','"+ui->name->text().toStdString()+"','"+ui->dept->text().toStdString()+"','"
                +type+"');";
        //QMessageBox::information(NULL,"",QString::fromStdString(sql),QMessageBox::Ok);
    }else{
        sql="delete from card where cno='"+ui->ID->text().toStdString()+"';";
    }

    Query q=conn.query(sql);
    q.execute();
    QMessageBox::information(NULL,"","操作成功",QMessageBox::Ok);
}

void license::btnReject(){
    this->close();
}

void license::textSetDisable(){
    if(ui->comboBox->currentText().toStdString()=="删除"){
        ui->name->setDisabled(true);
        ui->dept->setDisabled(true);
    }
}
