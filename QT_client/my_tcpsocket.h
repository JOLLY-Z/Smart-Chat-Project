#ifndef MY_TCPSOCKET_H
#define MY_TCPSOCKET_H

//单例模式
//设置为全局唯一对象，方便使用同一个套接字进行通信(涉及到c++内存分布知识！！)
#include <QObject>
#include<QTcpSocket>
#include<QHostAddress>
#include<QByteArray>
class my_TcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit my_TcpSocket(QObject *parent = nullptr);
    void connectToServer();

    void writeToServer(QString str);
    QByteArray recvFromServer();
    void closeToSocket();
    //单例模式，全局唯一对象
    static my_TcpSocket* instance(){
        if(!m_instance){
            m_instance=new my_TcpSocket;
        }
        return m_instance;
    }
     Q_DISABLE_COPY(my_TcpSocket)//禁止用拷贝构造
signals:

public:
    static QTcpSocket *socket;
    static my_TcpSocket * m_instance;

};

#endif // MY_TCPSOCKET_H
