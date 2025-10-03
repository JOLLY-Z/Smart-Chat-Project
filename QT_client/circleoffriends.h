#ifndef CIRCLEOFFRIENDS_H
#define CIRCLEOFFRIENDS_H

#include <QWidget>
#include"friendworksitem.h"
#include"pushworks.h"
#include<QListWidget>
#include<QListWidgetItem>

namespace Ui {
class circleOfFriends;
}

class circleOfFriends : public QWidget
{
    Q_OBJECT

public:
    explicit circleOfFriends(QWidget *parent = nullptr);
    ~circleOfFriends();
    void initfriends();

private slots:
    void on_toolButton_clicked();
    void updateCil();

private:
    Ui::circleOfFriends *ui;

    QList<friendWorksItem*> L;
    QList<QListWidgetItem*> L1;
    pushWorks *pushworks;

};

#endif // CIRCLEOFFRIENDS_H
