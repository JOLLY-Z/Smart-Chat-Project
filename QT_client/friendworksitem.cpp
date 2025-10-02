#include "friendworksitem.h"
#include "ui_friendworksitem.h"
#include<QLineEdit>
#include"cellmain.h"
#include<QMessageBox>

#include <QIcon>
#include <QGraphicsOpacityEffect>

friendWorksItem::friendWorksItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::friendWorksItem)
{
    ui->setupUi(this);

    ui->verticalSpacer = new QSpacerItem(
        20, 40, // 初始宽度和高度（可忽略，由布局自动管理）
        QSizePolicy::Minimum,   // 水平策略
        QSizePolicy::Expanding  // 垂直策略
        );

    // 将弹簧添加到布局中
    ui->verticalLayout->addItem(ui->verticalSpacer);

    //this 表示当前窗口/部件作为父对象。父对象销毁时会自动销毁模型，避免内存泄漏。
    listModel = new QStandardItemModel(this); // 创建模型
    //数据模型和视图绑定起来，进而实现数据的展示
    ui->listView->setModel(listModel);       // 将模型设置到 QListView（一维列表）

}

friendWorksItem::~friendWorksItem()
{
    delete ui;
}
//初始化ui文件
void friendWorksItem::initUi(QString username, QString Copywriting, int like, QString content, QImage image1, QImage image2,QImage image3)
{
    ui->label_2->setText(username);
    //判断作品是否为自己的，是的话就可以删除
    if(username!=CellMain::username){
        ui->toolButton_3->setEnabled(false);
         ui->toolButton_3->setStyleSheet("QToolButton { background: white; border: none; padding: 0px; }");
    }else{
    ui->toolButton_3->setIcon(QIcon(":/shanchusekuai.png"));
    ui->toolButton_3->setIconSize(QSize(25,25));
    }

    ui->label_3->setText(Copywriting);
    this->Copywriting=Copywriting;
    ui->label_5->setText(ui->label_5->text()+" : "+QString::number(like));
    this->like=like;
    //图片，需手动构造ui界面
    if(image1.isNull()!=true){
        ui->label_4->setPixmap(QPixmap::fromImage(image1));
        ui->label_4->setScaledContents(true);

    }
    if(image2.isNull()!=true){
        ui->label_6->setPixmap(QPixmap::fromImage(image2));
        ui->label_6->setScaledContents(true);

    }
    //设置头像
    if(image3.isNull()!=true){
        ui->label->setPixmap(QPixmap::fromImage(image3));
        ui->label->setScaledContents(true);

    }



}



//删除作品
void friendWorksItem::on_toolButton_3_clicked()
{


            QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认操作", "你确定要继续吗?",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    // 判断用户的选择
    if (reply == QMessageBox::Yes) {
        // 用户点击了 "确定"
        qDebug() << "用户选择了 '确定'";
        QSqlQuery sql;
        sql.prepare("delete from friendsWorks where username= :username and Copywriting= :Copywriting");
        sql.bindValue(":username",CellMain::username);
        sql.bindValue(":Copywriting",Copywriting);
        if (!sql.exec()) {
            qCritical() << "Error:" ;
        }
          emit updateUi();

    } else {
        // 用户点击了 "取消"
        qDebug() << "用户选择了 '取消'";
    }



}


//评论
void friendWorksItem::on_toolButton_clicked()
{
    QLineEdit *edit;
    if(flag==false){
        edit=new QLineEdit;
        edit->setStyleSheet("background-color:white");
        ui->horizontalLayout_3->insertWidget(1,edit);
        flag=true;
        // 连接回车信号到槽函数
        connect(edit, &QLineEdit::returnPressed,   // 回车按下信号
                this, [=](){

                    // 创建一个 QStandardItem（可存储任意类型的数据并支持层级结构（树状数据））
                    QStandardItem *item = new QStandardItem(CellMain::username+": "+edit->text());

                    // 设置不可选中状态
                    item->setFlags(item->flags() & ~Qt::ItemIsSelectable); // 移除可选中标志位

                    //在模型的末尾添加一行数据
                    listModel->appendRow(item);// 将 item 添加到模型中
                    edit->clear();
                    //待更新到数据库

                }); // 你的处理槽函数
    }
}


//点赞
void friendWorksItem::on_toolButton_2_clicked()
{
    //点赞功能
    QSqlQuery sql;
    sql.prepare("update friendsWorks set like=:like where id=:id");

    if(dianzan==false){
    ui->toolButton_2->setIcon(QIcon(":/aixin.png"));
        ui->toolButton_2->setIconSize(QSize(20,20));
        dianzan=1>0;
        ui->label_5->setText("点赞个数 : "+QString::number(++like));

    }else{
       ui->toolButton_2->setIcon(QIcon(":/dianzan.png"));
         dianzan=1<0;
       ui->label_5->setText("点赞个数"+QString::number(--like));
    }
   //数据库更新
     sql.bindValue(":like",like);
     sql.bindValue(":id",this->id);
     if(!sql.exec()){
         qDebug()<<"更新点赞失败";
     }

}


