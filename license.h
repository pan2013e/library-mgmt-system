#ifndef LICENSE_H
#define LICENSE_H

#include <QDialog>
#include <QObject>
#include <QMessageBox>
#include <QComboBox>

namespace Ui {
class license;
}

class license : public QDialog
{
    Q_OBJECT

public:
    explicit license(QWidget *parent = nullptr);
    ~license();

public slots:
    void btnOK();
    void btnReject();
    void textSetDisable();

private:
    Ui::license *ui;
};

#endif // LICENSE_H
