#include "circleoffriends.h"
#include "ui_circleoffriends.h"
#include<QSqlQuery>
#include<QSqlError>
circleOfFriends::circleOfFriends(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::circleOfFriends)
{
    ui->setupUi(this);
    setWindowTitle("朋友圈");

    initfriends();

    pushworks=new pushWorks;
    QObject::connect(pushworks,&pushWorks::updatefriendCil,
                     this,&circleOfFriends::initfriends);

}

circleOfFriends::~circleOfFriends()
{
    delete ui;
}

//初始化展示朋友圈
void circleOfFriends::initfriends()
{
    ui->listWidget->clear();

    //这里查询表时，根据id的大小降序排序，实现先发朋友圈先显示
    QSqlQuery sql;
    //从 friendsWorks 表中检索所有记录，并按照 id 字段降序排列
    if (!sql.exec("select * from friendsWorks order by id desc;")) {
        //qCritical():用于输出严重错误信息。
        qCritical() << "Error:" << sql.lastError().text();
    }
    while(sql.next()){
        qDebug()<<"初始化朋友圈"<<sql.value("id").toInt();
        //更新到朋友圈当中
        friendWorksItem *item1=new friendWorksItem;
        QListWidgetItem *item=new QListWidgetItem(ui->listWidget);
        item->setSizeHint(item1->sizeHint());

        //把QListWidgetItem放到QListWidget
        ui->listWidget->addItem(item);
        //把Widget控件放到QListWidgetItem
        ui->listWidget->setItemWidget(item,item1);

        QByteArray imageData = sql.value("image_1").toByteArray();
        QImage image_1;
        if (imageData.isEmpty()) {
            qWarning() << "BLOB 数据为空";
            image_1=QImage();

        }else{
            image_1.loadFromData(imageData);
        }
        QByteArray imageData1 = sql.value("image_2").toByteArray();
        QImage image_2;
        if (imageData1.isEmpty()) {
            qWarning() << "BLOB 数据为空";
            image_2=QImage();

        }else{
            image_2.loadFromData(imageData1);
        }
        //查头像
        QSqlQuery sql1;
        sql1.clear();
        sql1.prepare("select image from header where username= :username");
        sql1.bindValue(":username",sql.value("username").toString());
        if(!sql1.exec()){
            qDebug()<<"查询头像失败";
        }
         QImage image_3;
        while(sql1.next()){
        QByteArray imageData3 = sql1.value("image").toByteArray();

        if (imageData3.isEmpty()) {
            qWarning() << "BLOB 数据为空";
            image_3=QImage();

        }else{
            image_3.loadFromData(imageData3);
        }

        }


        item1->id=sql.value("id").toInt();
        item1->initUi(sql.value("username").toString(),sql.value("Copywriting").toString(),sql.value("like").toInt(),sql.value("content").toString(),image_1,image_2,image_3);
        //对列表项之间的间距进行设置
        ui->listWidget->setSpacing(5);
        //对每一条朋友圈都设置一个connect，当朋友圈条数多了会不会导致信号槽遍历查找对应的槽函数的速度下降？
        //不会，信号发射时仅遍历当前信号对应的槽函数列表，
        //查找信号通过哈希表，O(1)
        connect(item1,&friendWorksItem::updateUi,this,&circleOfFriends::initfriends);
    }


}

void circleOfFriends::on_toolButton_clicked()
{
    pushworks->show();
    qDebug()<<"111";

}
//更新朋友圈
void circleOfFriends::updateCil()
{
    //这里更新别全部更新，就更新发布




}

