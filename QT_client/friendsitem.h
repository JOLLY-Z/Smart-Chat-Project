#ifndef FRIENDSITEM_H
#define FRIENDSITEM_H

#include <QWidget>
#include<QMouseEvent>
#include<QString>
namespace Ui {
class FriendsItem;
}

class FriendsItem : public QWidget
{
    Q_OBJECT

public:
    explicit FriendsItem(QWidget *parent = nullptr);
    ~FriendsItem();
    //设置好友头像
    void setHeadImage(QString imgpath);
    void setId(QString str);
    void setheader(QPixmap pixmap);
    QString strname;
    //自定义被点击事件
    void updateIsRuning();

    QPixmap convertToGrayscale(const QPixmap &pixmap);
signals:
    void clicked();//自定义信号
protected:
    void mousePressEvent(QMouseEvent *event) override{
        if(event->button()==Qt::LeftButton){
            emit clicked();
        }
        QWidget::mousePressEvent(event);
    }



private:
    Ui::FriendsItem *ui;
    QPixmap pixmap_light;
    QPixmap pixmap_gray;
};

#endif // FRIENDSITEM_H
