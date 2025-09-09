#include<iostream>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include"locker.h"
#include"threadpool.h"
#include<signal.h>
#include"http_conn.h"

#define MAX_FD 65535 //处理最大的文件描述符个数
//添加信号捕捉,注册信号以及信号处理函数
#define MAX_EVENT_NUM 10000
void addsig(int sig,void(handler)(int)){
  struct sigaction sa;
  memset(&sa,'\0',sizeof(sa));
    sa.sa_handler=handler;
    sigfillset(&sa.sa_mask);
    sigaction(sig,&sa,NULL);

}
//添加文件描述符到epoll中
extern void addfd(int epollfd,int fd,bool one_shot);
//删除epoll中的文件描述符
extern void removefd(int epollfd,int fd);
//修改文件描述符的事件
extern void modfd(int epollfd,int fd,int ev);
int main(int argc,char *argv[]){
       
       if(argc<=1){
        printf("按照如下格式运行: %s port_number\n",basename(argv[0]));
        return -1;
       }
       //获取端口号
       int port=atoi(argv[1]);
       //对信号做捕捉,处理动作为忽略此信号
        addsig(SIGPIPE,SIG_IGN);
      //创建线程池，线程池一创建，子线程也会被创建开始运行阻塞
      threadpool<http_conn>* pool=NULL;
       //异常捕捉
      try{
        pool=new threadpool<http_conn>;
      }catch(...){
        return -1;
      }
     //创建一个数组用于保存所有的客户端信息,实际已经创建了MAX_FD个数组
      http_conn *users=new http_conn[MAX_FD];
     
     //创建套接字符
     int listenfd = socket(PF_INET,SOCK_STREAM,0);
     //设置端口复用
     int reuse=1;
     setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
     //绑定套接字
     struct sockaddr_in address;
     address.sin_family=AF_INET;
     address.sin_addr.s_addr=INADDR_ANY;
     address.sin_port=htons(port);
     bind(listenfd,(struct sockaddr*)&address,sizeof(address));
//  监听
    listen(listenfd,5);
    
   //事件数组,用于存储当循环检测epoll中有事件触发的文件描述符号
     epoll_event events[MAX_EVENT_NUM];
    //创建epoll对象,用于控制访问epoll
      int epollfd=epoll_create(5);
    //将监听的文件描述符添加到epoll中,将监听的文件描述符封装成event事件在加入epoll中！，通过epoll对象获取该文件描述符，并设置该文件描述符为非阻塞
      addfd(epollfd,listenfd,false);
      http_conn::m_epollfd=epollfd;
    //循环检测添加的文件描述符有没有事件发生
  
    while(true){
      
      //num代表有多少个事件触发，events用于存储有触发的事件
     int num=epoll_wait(epollfd,events,MAX_EVENT_NUM,-1);
    
      if((num<0)&&(errno != EINTR)){
        printf("epoll failure\n");
        break;
      }
      
      //循环遍历事件数组
      for (int i = 0; i < num; i++)
      {
        //获取文件描述符
        int sockfd=events[i].data.fd;
        //判断是否为监听文件描述符

        if(sockfd==listenfd){
          //有客户端连接进来
          struct sockaddr_in client_address;
          socklen_t client_addrlen=sizeof(client_address);
          //进行连接，并获得客户端文件描述符
          int connfd=accept(listenfd,(struct sockaddr*)&client_address,&client_addrlen);
          //判断存储http响应的数组是否到达规定的最大数量
          if(http_conn::m_user_count>=MAX_FD){
            //说明目前连接数已经满了，回写给客户端连接正忙
            close(connfd);
            continue;
          }
          //将新的客户端的连接请求封装为http_conn类型并初始化，封装好后将文件描述符放入epoll中并继续监听此文件描述符的活动
          users[connfd].init(connfd,client_address);

          //将所有已经建立连接后的套接字加入数组中
           http_conn::bc.insert({connfd,0});
        }else if(events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)){
          //对方异常断开或者错误事件
         users[sockfd].close_conn();
        }else if(events[i].events & EPOLLIN){
          //读事件,因此之前连接也是让connfd作为索引选择users数组中的某个内存初始化,所以读数据时也让其作为索引访问该初始化的内存片段
          if(users[sockfd].read()){
            //一次性把数据读完
        
              //  printf("%s",users[sockfd].m_read_buf);
              //先禁止使用线程保证数据安全
               pool->append(users+sockfd); 
            
          }else{
            //读失败了
           users[sockfd].close_conn();
          }
        }
      }
      

    }
    //释放资源

    close(epollfd);
    close(listenfd);
    delete [] users;
    delete pool;
    return 0;
}