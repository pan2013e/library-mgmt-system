#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
    class login;
}
QT_END_NAMESPACE

class login : public QWidget{
    Q_OBJECT

public:
    explicit login(QWidget *parent=0);
    ~login();
    bool logStats();

public slots:
    void Verification();
    void jmpSEA();

private:
    Ui::login *ui;
    bool isLogin;
};

#endif // LOGIN_H
