#ifndef CELLMAIN_H
#define CELLMAIN_H

#include <QWidget>
#include<QString>
#include"friendsitem.h"
#include"chatwindows.h"
#include<my_tcpsocket.h>
#include<QList>
#include<QStringList>
#include<QMap>
#include<QCloseEvent>
#include"addfriend.h"
#include<QMouseEvent>
#include<QAction>
#include<QMenu>
#include"chatdb.h"
#include<QSqlQuery>
#include<QTimer>
#include"circleoffriends.h"
#include<memory>
namespace Ui {
class CellMain;
}

class CellMain : public QWidget
{
    Q_OBJECT

public:
    explicit CellMain(QWidget *parent = nullptr);
    ~CellMain();
    void getStrUser(QString str);
    void addfriendnow(QString strid);

    //重写鼠标右键点击事件
    void mousePressEvent(QMouseEvent *event) override;
    void createGroup();
    static QString username;


    void findheader();

public slots:
    void handleReadyRead(); // 新增的槽函数声明,用于去接受好友列表的消息，加载客户端的好友列表
    void handleFriendItemClicked();//用于获得点击item对象转换到相应的聊天页面去
    void handleMessage_to_text();
    void closeEvent(QCloseEvent *event);

signals:
    void handleMessage();
    void updateIsRuning();



private slots:
    void on_addFriend_triggered(QAction *arg1);

    void on_addFriend_clicked();

    void on_deleteFriend_clicked();

    void on_applyFriend_clicked();

    void on_friendsitem_clicked();

private:
    Ui::CellMain *ui;
    QString strUser;
    my_TcpSocket *c_mysocket;
    int  index=0;//每一个好友列表聊天框的索引
    QList<chatWindows *> chatWindowsList; // 用于存储 chatWindows 指针的列表
    QList<FriendsItem *> friendsItemList; // 用于存储 FriendsItem 指针的列表
    QMap<QString,QString> userAndMessage;
    addfriend a;
    int index_ai;

    QByteArray image_data;

    QTimer *timer=nullptr;

    std::unique_ptr<circleOfFriends> cir;


};




#endif // CELLMAIN_H
