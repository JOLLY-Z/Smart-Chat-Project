#include "dialog.h"
#include "ui_dialog.h"

#include<QDebug>
#include<QMessageBox>
#include<cellmain.h>
#include<QString>
#include<QByteArray>
#include<string>
#include<QGraphicsDropShadowEffect>
#include<QPixmap>
#include<QSize>
#include"signup.h"

//connetion连接最好连接在构造函数中，不要在次函数中
int flag=0;
bool Dialog::isconnect=false;
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    //把 UI 设计文件（.ui）里的界面元素加载并设置到当前的窗口部件里。
    ui->setupUi(this);

    //从资源文件中加载名为blue.png的图片。
    QPixmap *pix=new QPixmap(":/blue.png");
    QSize sz=ui->label_image->size();
    //按照label_image尺寸对图像进行缩放
    ui->label_image->setPixmap(pix->scaled(sz));
    //此代码创建了一个QGraphicsDropShadowEffect对象shadow，并把它的父对象设定为this。
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //这里把阴影的偏移量设为(-3, 0)，也就是阴影会向左偏移 3 个单位。
    shadow->setOffset(-3, 0);
    //此代码把阴影颜色设为#888888，即灰色。
    shadow->setColor(QColor("#888888"));
    //这里把阴影的模糊半径设为 30，数值越大，阴影就越模糊。
    shadow->setBlurRadius(30);
    //最后把阴影效果应用到ui->label_image标签控件上。
    ui->label_image->setGraphicsEffect(shadow);

    //创建TCP实例
    m_t=my_TcpSocket::instance();
    setWindowTitle("服务器连接中....");

    //QTcpSocket::connected 信号会在 QTcpSocket 对象成功连接到目标服务器时发射
    //这里的socket是一个QTcpSocket实例
    connect(m_t->socket,&QTcpSocket::connected,this,[=](){
        flag=1;
        setWindowTitle("登陆页面：服务器连接成功");
        isconnect=true;
    });
    connect(m_t->socket,&QTcpSocket::disconnected,this,[=](){
        flag=0;
        setWindowTitle("登陆页面：服务器连接失败");
        isconnect=false;
    });

    m_t->connectToServer();

    // setWindowTitle("服务器连接中....");

    // //QTcpSocket::connected 信号会在 QTcpSocket 对象成功连接到目标服务器时发射
    // //这里的socket是一个QTcpSocket实例
    // connect(m_t->socket,&QTcpSocket::connected,this,[=](){
    //     flag=1;
    //     setWindowTitle("登陆页面：服务器连接成功");
    //     isconnect=true;
    // });
    // connect(m_t->socket,&QTcpSocket::disconnected,this,[=](){
    //     flag=0;
    //     setWindowTitle("登陆页面：服务器连接失败");
    //     isconnect=false;
    // });

    //在 QTcpSocket 对象接收到新的数据，并且可以读取时，这个信号就会被发射出来
    connect(m_t->socket, &QTcpSocket::readyRead, this, &Dialog::onReadyRead);

    //设置登录窗口的密码加密
    ui->le_userPassword->setEchoMode(QLineEdit::Password);
}

Dialog::~Dialog()
{

    delete ui;
}

void Dialog::on_btn_login_clicked()
{

    // if(flag==0){
    //     QMessageBox::warning(this,"提示","等待服务器启动中...");
    // }else
    // {

         m_t->writeToServer(ui->le_userName->text()+":");
         m_t->writeToServer(ui->le_userPassword->text()+'|');

    // }

}

void Dialog::onReadyRead() {
    qDebug() << "数据到达";
    QByteArray ba = m_t->recvFromServer();
    QString str(ba);
    qDebug() << "str: " << str;
    if (str == "yes") {
        start_videoCall::myUsername=ui->le_userName->text();
        //一个类的所有不同实例共享同一个类的静态成员变量
        //CellMain::username是静态成员变量，先提前设置好username
        CellMain c;
        c.getStrUser(ui->le_userName->text());
        CellMain::username=ui->le_userName->text();

        chatdb::instance().insertheader(ui->le_userName->text());

        //QDialog类的setResult()方法用于设置对话框的返回结果。
        //这个结果通常用于表示用户在对话框中做出的选择，例如 “确定”、“取消” 等。
        setResult(1);
        hide();
        // 当验证成功以后断开此连接，避免跳入主界面时该信号槽还会触发
        disconnect(m_t->socket, &QTcpSocket::readyRead, this, &Dialog::onReadyRead);
    } else {
        QMessageBox::warning(this, "提示", "账号或密码错误！");
        ui->le_userName->clear();
        ui->le_userPassword->clear();
    }
}

void Dialog::closeEvent(QCloseEvent *event)
{
    exit(0);
}

//退出
void Dialog::on_btn_exit_clicked()
{
    exit(0);
}
//注册功能
void Dialog::on_pushButton_clicked()
{

    setResult(0);
    //进入登陆页面前只能隐藏此登陆页面，不能调用this.close()进行关闭，不然会触发closeEvent事件，我的closeEvent事件是退出进程
    //会导致整个进程都退出！

    hide();

}



