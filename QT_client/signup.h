#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include<QDialog>
#include<QCloseEvent>
#include"my_tcpsocket.h"
namespace Ui {
class Signup;
}

class Signup : public QWidget
{
    Q_OBJECT

public:
    explicit Signup(QWidget *parent = nullptr);
    ~Signup();

private slots:
    void on_pushButton_2_clicked();
     void closeEvent(QCloseEvent *event);//由于main函数中使用的是死循环，所以点击X事件要强制退出

    void on_pushButton_clicked();

private:
    Ui::Signup *ui;
    my_TcpSocket *m_sign=my_TcpSocket::instance();

};

#endif // SIGNUP_H
