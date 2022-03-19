#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QWidget>
#include <QTableView>
#include <QMessageBox>
#include <QStandardItemModel>


namespace Ui {
class transact;
}

class transact : public QWidget
{
    Q_OBJECT

public:
    explicit transact(QWidget *parent = nullptr);
    ~transact();
    QStandardItemModel *model;

public slots:
    void search();
    void ret();
    void borrow();

private:
    Ui::transact *ui;
};

#endif // TRANSACTION_H
