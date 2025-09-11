#include "my_tcpsocket.h"

//设置为全局静态,单例模式，使在其他类使用时，也只使用该一个套接字
my_TcpSocket* my_TcpSocket::m_instance=nullptr;
QTcpSocket* my_TcpSocket::socket=nullptr;


my_TcpSocket::my_TcpSocket(QObject *parent)
    : QObject{parent}
{
    socket=new QTcpSocket;
}

void my_TcpSocket::connectToServer()
{
    // socket=new QTcpSocket;
    unsigned short port=10000;

    socket->connectToHost(QHostAddress("192.168.186.135"),port);
    if (!socket->waitForConnected(3000)) {
        qDebug() << "Error:" << socket->errorString(); // 打印详细错误
    }
}


void my_TcpSocket::writeToServer(QString str)
{
    socket->write(str.toUtf8());
}

QByteArray my_TcpSocket::recvFromServer()
{
//从套接字里读取所有可用的数据，并且以 QByteArray 类型返回。
    return socket->readAll();

}

void my_TcpSocket::closeToSocket()
{

    //释放资源，还需测试
   //  delete socket;
    // socket->close();
}

