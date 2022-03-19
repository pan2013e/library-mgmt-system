#ifndef STORE_H
#define STORE_H

#include <QDialog>

namespace Ui {
class store;
}

class store : public QDialog
{
    Q_OBJECT

public:
    explicit store(QWidget *parent = nullptr);
    ~store();

public slots:
    void btnOK();
    void btnReject();
    void load();

private:
    Ui::store *ui;
    bool loadFile;
    QString fileName;
};

#endif // STORE_H
