#include "signup.h"
#include "ui_signup.h"
#include<QPixmap>
#include<QGraphicsDropShadowEffect>
#include<QMessageBox>
#include<QString>
#include"dialog.h"
Signup::Signup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Signup)
{
    ui->setupUi(this);
    //设置图片
    setWindowTitle("注册");
    QPixmap *pix = new QPixmap(":/girl1.png");
    QSize sz = ui->label->size();
    ui->label->setPixmap(pix->scaled(sz));
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(-3, 0);
    shadow->setColor(QColor("#888888"));
    shadow->setBlurRadius(30);
    ui->label->setGraphicsEffect(shadow);

    //密码设置为不可见
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    connect(m_sign->socket,&QTcpSocket::disconnected,[=](){
        //与客户端断开连接
        QMessageBox::warning(this,"提示","与服务端断开连接，请重启后注册");
        exit(0);
    });

}

Signup::~Signup()
{
    delete ui;
}

void Signup::on_pushButton_2_clicked()
{
    this->close();
}

void Signup::closeEvent(QCloseEvent *event)
{
    this->close();
}

void Signup::on_pushButton_clicked()
{
    if(Dialog::isconnect==false){
        QMessageBox::warning(this,"提示","当前未与服务端建立连接，请连接后重试!");
    }
    else{
    QString str1=ui->lineEdit_2->text();
    QString str2=ui->lineEdit_3->text();
    qDebug()<<str1<<" "<<str2;

    if(str1!=str2){
        QMessageBox::warning(this,"提示","密码不一致，请重新输入！");
    }else{
        m_sign->writeToServer("|"+ui->lineEdit->text()+":"+ui->lineEdit_2->text()+'|');
        QMessageBox::information(this,"提示","注册成功，请返回登陆界面登陆");
    }
    }


}

