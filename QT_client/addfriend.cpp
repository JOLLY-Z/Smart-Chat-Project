#include "addfriend.h"
#include "ui_addfriend.h"
#include"my_tcpsocket.h"
#include<QMessageBox>
#include<cellmain.h>
addfriend::addfriend(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addfriend)
{
    ui->setupUi(this);
    setWindowTitle("添加好友");

}

QString addfriend::returnStr()
{
    return ui->lineEdit->text();
}

addfriend::~addfriend()
{
    delete ui;
}



void addfriend::on_pushButton_clicked()
{

    my_TcpSocket *m=my_TcpSocket::instance();
    m->writeToServer('|'+ui->lineEdit->text()+":");
    //发送信号需要;

}


void addfriend::on_pushButton_2_clicked()
{
    this->close();
}

