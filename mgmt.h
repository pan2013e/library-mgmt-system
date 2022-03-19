#ifndef MGMT_H
#define MGMT_H

#include <QWidget>
#include <QCheckBox>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
    class mgmt;
}
QT_END_NAMESPACE

class mgmt : public QWidget{
    Q_OBJECT

public:
    explicit mgmt(QWidget *parent=0);
    ~mgmt();

public slots:
    void jmpEXIT();
    void jmpLIC_MGMT();
    void jmpSTORE();
    void jmpSEARCH();
    void jmpTRANS();

private:
    Ui::mgmt *ui;

};
#endif // MGMT_H
