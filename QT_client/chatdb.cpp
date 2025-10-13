#include "chatdb.h"
#include<QDebug>

chatdb::chatdb(QObject *parent)
          : QObject{parent}
{
    //建立和Sqlite的连接，后面执行query.exec等操作的时候就知道来这个数据库来找数据
    sqldb=QSqlDatabase::addDatabase("QSQLITE");
    //需要显示的指定数据库的文件路径，为绝对路径，项目移植是sqlite数据库需转移
    sqldb.setDatabaseName("D:/Jolly/AAA_Wchat/chatdb.db");
    if (!sqldb.open()) {
        qDebug() << "Error: Unable to open database!";
        return;
    }
    qDebug()<<"数据库打开成功";
}

void chatdb::insertheader(QString username)
{
    QSqlQuery query;
    query.prepare("insert into header (username) values(:username  );");
    query.bindValue(":username",username);
    if(!query.exec()){
        qDebug()<<"当前账户已存在";
    }
    //更新在线列表，表示自己在线中,0表示在线，1表示离线
    query.clear();
    query.prepare("insert into isUserRuning (username,isRuning) values(:username,:isRuning);");
    query.bindValue(":username",username);
    query.bindValue(":isRuning",0);
    if(!query.exec()){
        qDebug()<<"在线用户已存在";
    }
    query.clear();
    query.prepare("update isUserRuning set isRuning=:isRuning where username=:username");
    query.bindValue(":isRuning",0);
     query.bindValue(":username",username);
    if(query.exec()){
        qDebug()<<"在线用户更新成功";
    }else{
        qDebug()<<"用户在线列表更新失败";
    }


}
