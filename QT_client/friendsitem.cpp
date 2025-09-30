#include "friendsitem.h"
#include "ui_friendsitem.h"
#include<qsqlquery.h>
FriendsItem::FriendsItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FriendsItem)
{
    ui->setupUi(this);


    ui->label_4->setStyleSheet("background-color: transparent;");



}

FriendsItem::~FriendsItem()
{
    delete ui;
}

void FriendsItem::setHeadImage(QString imgpath)
{
    ui->label->setStyleSheet(imgpath);
}

void FriendsItem::setId(QString str)
{
    ui->label_2->setText(str);
    strname=str;

}

void FriendsItem::setheader(QPixmap pixmap)
{
    //设定标签是否按比例缩放内容。
    ui->label->setScaledContents(true);
    ui->label->setPixmap(pixmap);

    this->pixmap_light=pixmap;
    this->pixmap_gray=convertToGrayscale(pixmap);
}

//更新
void FriendsItem::updateIsRuning()
{
   QString str=ui->label_2->text();
    if(str.left(2)!="群聊"){
    QSqlQuery query;
    //用于准备一个 SQL 查询语句，以便后续执行
    query.prepare("select isRuning from isUserRuning where username=:username");
    QString str1=strname;
    QString str=str1.mid(3);//从字符串中提取子字符串。3 - 末尾
    //命名占位符以 : 开头，然后跟着占位符名称
    query.bindValue(":username",str);//把占位符名称和对应的值关联起来。

    //执行 之前prepare的SQL 语句。
    if(!query.exec()){
        qDebug()<<"在线列表更新失败";
    }
    while(query.next()){
        //在线
        if(query.value("isRuning").toInt()==0){
            //qDebug()<<"在线";
             ui->label_4->setText("在线中……");
            ui->label->setScaledContents(true);
            ui->label->setPixmap(pixmap_light);
        }else
           //不在线
        {
            //qDebug()<<"不在线";
            ui->label_4->setText("离线中……");
            ui->label->setScaledContents(true);
            ui->label->setPixmap(pixmap_gray);
        }
        return ;

    }
    //qDebug()<<"未查询到";
    ui->label_4->setText("离线中……");
    ui->label->setScaledContents(true);
    ui->label->setPixmap(pixmap_gray);
    }else {
      // qDebug()<<"这是群聊";
        ui->label_4->setText("");
    }




}

QPixmap FriendsItem::convertToGrayscale(const QPixmap &pixmap)
{
    // 将 QPixmap 转换为 QImage
    QImage image = pixmap.toImage();

    // 遍历图像的每个像素
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = image.pixelColor(x, y);

            // 计算灰度值
            int gray = qGray(color.red(), color.green(), color.blue());

            // 设置灰度值
            color.setRed(gray);
            color.setGreen(gray);
            color.setBlue(gray);

            // 更新图像中的像素
            image.setPixelColor(x, y, color);
        }
    }

    // 将 QImage 转回 QPixmap
    return QPixmap::fromImage(image);
}
