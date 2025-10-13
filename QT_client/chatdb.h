#ifndef CHATDB_H
#define CHATDB_H

#include <QObject>
#include<QSqlDatabase>
#include<QSqlQuery>
class chatdb :public QObject
{
    Q_OBJECT
public:
    //单例模式，避免数据库重复初始化
    static chatdb& instance(){
        static chatdb instance;
        return instance;
    }

    // 禁用拷贝构造函数和赋值运算符
   chatdb(const chatdb&) = delete;
   chatdb& operator=(const chatdb&) = delete;

   void insertheader(QString username);



private:
   // 构造函数私有化，防止外部实例化
   chatdb(QObject *parent=nullptr);
   QSqlDatabase sqldb;
};

#endif // CHATDB_H
