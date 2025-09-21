#include "checkpassword.h"
#include "ui_checkpassword.h"
#include<my_tcpsocket.h>
#include<QMessageBox>
checkpassword::checkpassword(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::checkpassword)
{
    ui->setupUi(this);
    setWindowTitle("修改密码");
}

checkpassword::~checkpassword()
{

    delete ui;
}
//发送需要修改的密码
void checkpassword::on_pushButton_clicked()
{
    my_TcpSocket *m=my_TcpSocket::instance();
    if(ui->lineEdit->text()==ui->lineEdit_2->text()){
        m->writeToServer('&'+ui->lineEdit->text()+':');
        QMessageBox::information(this,"提示","修改密码成功！");
    }else {
        QMessageBox::warning(this,"提示","前后密码不一致");
    }
}


void checkpassword::on_pushButton_2_clicked()
{
    this->close();
}

