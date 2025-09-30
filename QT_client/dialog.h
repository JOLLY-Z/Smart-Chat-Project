#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QString>
#include<QHostAddress>
#include<QMessageBox>
#include<QByteArray>
#include<QTcpSocket>
#include"my_tcpsocket.h"
#include<QByteArray>
#include<QCloseEvent>
#include"chatdb.h"
 #include"video_call/start_videocall.h"
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void onReadyRead();//自定义一个接收服务端过来的消息的槽函数
    void closeEvent(QCloseEvent *event);//由于main函数中使用的是死循环，所以点击X事件要强制退出
    static bool isconnect;
private slots:
    void on_btn_login_clicked();

    void on_btn_exit_clicked();

    void on_pushButton_clicked();


private:
    Ui::Dialog *ui;
    my_TcpSocket *m_t;
    QByteArray ba;

};



#endif // DIALOG_H
