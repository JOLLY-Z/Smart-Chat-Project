
#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include "locker.h"
#include <sys/uio.h>
#include<list>
#include<map>
#include<string>
#include"sqlite_for_chat.h"
#include<vector>
class http_conn
{
public:
    static const int FILENAME_LEN = 200;        // 文件名的最大长度
    static const int READ_BUFFER_SIZE = 2048;   // 读缓冲区的大小
    static const int WRITE_BUFFER_SIZE = 1024;  // 写缓冲区的大小


  static std::map<int,int> bc;//返还给客户端的公共聊天套接字,为所有对象所
public:
    http_conn(){}
    ~http_conn(){}
public:
    void init(int sockfd, const sockaddr_in& addr); // 初始化新接受的连接
    void close_conn();  // 关闭连接
    void process(); // 处理客户端请求
    bool read();// 非阻塞读
    bool write(int writefd);// 非阻塞写
    bool write(int writefd,int len);//返还给客户端用于登陆验证操作
    bool write(int writefd,std::vector<std::string> &v);//返还给客户端好友列表的消息
    bool write(int writefd,std::string str);//转发客户端发送过来的消息的消息
    bool getHeadandLast(std::string &str,int &head,int &last,int &rear);//获得username得首位索引
   
private:
    void init();    // 初始化连接


    
public:
    static int m_epollfd;       // 所有socket上的事件都被注册到同一个epoll内核事件中，所以设置成静态的
    static int m_user_count;    // 统计用户的数量
    char *m_read_buf;   // 读缓冲区
    std::string strUser;
    std::string strPassword;
    int m_sockfd;           // 该HTTP连接的socket和对方的socket地址
    bool isLogin;
    bool getfriedslgin;
    const char *login_buf;//返还给客户端是否登陆正确,直接字符串赋值是常量char类型，所以这边也要申明为常量
    std::vector<std::string> getfriends;
  
   

    


};


#endif
