#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>

namespace Ui {
class search;
}

class search : public QWidget
{
    Q_OBJECT

public:
    explicit search(QWidget *parent = nullptr);
    ~search();
    QStandardItemModel *model;

public slots:
    void query();

private:
    Ui::search *ui;

};

#endif // SEARCH_H
