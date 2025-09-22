#include "deletefriend.h"
#include "ui_deletefriend.h"
#include<my_tcpsocket.h>
#include<QMessageBox>
deletefriend::deletefriend(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::deletefriend)
{
    ui->setupUi(this);
    setWindowTitle("删除好友");
}

deletefriend::~deletefriend()
{

    delete ui;
}
//向客户端发送需要删除好友的账号
void deletefriend::on_pushButton_clicked()
{
    my_TcpSocket *m=my_TcpSocket::instance();
    m->writeToServer('*'+ui->lineEdit->text()+':');
    QMessageBox::information(this,"提示","删除成功！");
}


void deletefriend::on_pushButton_2_clicked()
{
    this->close();
}

