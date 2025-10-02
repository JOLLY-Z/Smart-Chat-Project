#ifndef FRIENDWORKSITEM_H
#define FRIENDWORKSITEM_H

#include <QWidget>
#include<QStandardItemModel>


#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QWidget>
#include<QMainWindow>
namespace Ui {
class friendWorksItem;
}

class friendWorksItem : public QWidget
{
    Q_OBJECT

public:
    explicit friendWorksItem(QWidget *parent = nullptr);
    ~friendWorksItem();
    void initUi(QString username,QString Copywriting,int like,QString content,QImage image1,QImage image2,QImage image3);
    int id;
private slots:
    void on_toolButton_clicked();

    void on_toolButton_3_clicked();
    void on_toolButton_2_clicked();

signals:
    void updateUi();

private:
    Ui::friendWorksItem *ui;
    bool flag=false;
    bool dianzan=false;
    QString Copywriting;
    QStandardItemModel *listModel;
    int like;


};

#endif // FRIENDWORKSITEM_H
