 
#include "http_conn.h"
#include<iostream>
#include<vector>
#include"ai_to_recv.h"
std::map<int,int> http_conn::bc;
// 设置文件描述符非阻塞
int setnonblocking( int fd ) {
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
  
    return old_option;
}


// 向epoll中添加需要监听的文件描述符
void addfd( int epollfd, int fd, bool one_shot ) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLRDHUP;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    // 设置文件描述符非阻塞
    setnonblocking(fd);  
}

// 从epoll中移除监听的文件描述符
void removefd( int epollfd, int fd ) {
    epoll_ctl( epollfd, EPOLL_CTL_DEL, fd, 0 );
    close(fd);
}

// 修改文件描述符，重置socket上的EPOLLONESHOT事件，以确保下一次可读时，EPOLLIN事件能被触发
void modfd(int epollfd, int fd, int ev) {
    epoll_event event;
    event.data.fd = fd;
    // event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
     event.events = ev | EPOLLET  | EPOLLRDHUP;
    epoll_ctl( epollfd, EPOLL_CTL_MOD, fd, &event );
}

// 所有的客户数
int http_conn::m_user_count = 0;
// 所有socket上的事件都被注册到同一个epoll内核事件中，所以设置成静态的
int http_conn::m_epollfd = -1;

// 关闭连接
void http_conn::close_conn() {
    if(m_sockfd != -1) {
        removefd(m_epollfd, m_sockfd);
        m_sockfd = -1;
        m_user_count--; // 关闭一个连接，将客户总数量-1
    }


}

// 初始化连接,外部调用初始化套接字地址
void http_conn::init(int sockfd, const sockaddr_in& addr){
    m_sockfd = sockfd;
    
    // 端口复用
    int reuse = 1;
    setsockopt( m_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse ) );
    addfd( m_epollfd, sockfd, true );

    // 用户数加1
    m_user_count++; 
    isLogin=false;
    getfriedslgin=false;
    getfriends.clear();
    init();
}

// 初始化其他信息
void http_conn::init()
{
   
   login_buf=(char*)malloc(READ_BUFFER_SIZE);
    m_read_buf=(char*)malloc(READ_BUFFER_SIZE);
    bzero(m_read_buf, READ_BUFFER_SIZE);
 
}

// 循环读取客户数据，直到无数据可读或者对方关闭连接
bool http_conn::read() {
   
    int bytes_read = 0;
    while(true) {
       //recv函数的第三个参数的意思为读取的最大长度字符，未到最大长度则不用读到最大长度，第四个表示阻塞或非阻塞的去读
        bytes_read = recv(m_sockfd, m_read_buf, 
        READ_BUFFER_SIZE, 0 );
        if (bytes_read == -1) {
            if( errno == EAGAIN || errno == EWOULDBLOCK ) {
                // 没有数据
                break;
            }
            return false;   
        } else if (bytes_read == 0) {   // 对方关闭连接
            return false;
        }
    }

    return true;
}

bool http_conn::write(int writefd)
{ 
    printf("发送数据为：%s\n",m_read_buf);
    std::cout<<"w:"<<writefd<<std::endl;
   int ret=send(writefd, m_read_buf, 
        READ_BUFFER_SIZE , 0 );
        if(ret==-1){
            std::cout<<"发送消息失败"<<std::endl;
            return false;
        }
        return true;

}
bool http_conn::write(int writefd,int len){
      std::cout<<"向客户端发送消息登陆验证消息"<<std::endl;
      std::string text(login_buf);
        std::cout<<text<<std::endl;
    int ret=send(writefd,login_buf, 
        len, 0 );
        if(ret==-1){
            return false;
        }
        return true;
}

bool http_conn::write(int writefd,std::vector<std::string> &v){
     for(auto &x:v){
    std::string str(x);
    std::cout<<"给客户端发送的消息为: "<<str<<"长度为："<<x.length()<<std::endl;
      int ret=send(writefd,x.c_str(),x.length(),0);
       usleep(400000);//线程睡眠等待客户端一条一条的去接受消息，不然客户端会把所有消息接收成一个字符串！
      if(ret==-1){
        std::cout<<"加载好友消息发送失败！"<<std::endl;
        return false;
      }
     }
   
     return true;
}

bool http_conn::write(int writefd,std::string str){
    std::cout<<"套接字和向好友发送的内容为："<<writefd<<" "<<str<<std::endl;
    int ret=send(writefd, str.c_str(), 
        str.size() , 0 );
        if(ret==-1){
            std::cout<<"发送消息失败"<<std::endl;
            return false;
        }
        return true;

}

bool http_conn::getHeadandLast(std::string &str,int &head,int &last,int &rear){
    for (size_t i = 0; ; i++)
    {
        if(str[i]==':'){
            head=i+1;
            for(int j=i+1;;j++){
                if(str[j]==':'){
                    last=j;
                    for(int k=j+1;;k++){
                        if(str[k]==':'){
                            rear=k;
                            return true;
                        }
                    }
                }
            }
        }
    }
    
}


void http_conn::process() {

    //读完数据后进行逻辑处理
    //将客户端发过来的数据进行判断为登陆，加载好友，添加好友以及删除好友
     

    //先打开数据库！
    std::string str(m_read_buf);

    //判断是不是测压，测压直接返回
    if(str.find("GET") == 0 || str.find("POST") == 0){
         std::cout << "Received HTTP request: " << std::endl;
        std::cout << str << std::endl;

        // 模拟一个 HTTP 响应报文
        std::string response = "HTTP/1.0 400 Bad Request\r\n";
        response += "Content-Length: 68\r\n";
        response += "Content-Type: text/plain\r\n";
        response += "Connection: close\r\n";
        response += "\r\n"; // 空行表示头部结束
        response += "Hello, this is a simulated HTTP response!\r\n";
         write(m_sockfd,response);
         close(m_sockfd);
        return;
    }

     sqlite db("familyGuy.db");
    if (!db.isConnected()) {
        std::cerr << "Database connection failed." << std::endl;
       
    }


    //如果是注册账号
    if(str[0]=='|'&&isLogin==false){
         std::cout<<"注册: "<<str<<std::endl;  
         int index=0;
         int num(0);
          for (size_t i = 1; i < str.size(); i++)
    {
        if(str[i]==':') {
            for(int j=i+1;j<str.size();j++){
                if(str[j]=='|') break;
                 else  num++;
            }
            break;
        }
        index++;
    }
    //验证发过来的字符串和密码进行拆分
      strUser=str.substr(1,index);
      strPassword=str.substr(index+2,num);
       std::cout<<strUser<<std::endl;
       std::cout<<strPassword<<std::endl;
       db.insert("INSERT INTO login (username, password,sockfd) VALUES ('"+strUser+"', '"+strPassword+"',0);");
    }
    //判断如果是登陆验证
    else if(isLogin==false){
        
      int index=0;
      int num(0);
    for (size_t i = 0; i < str.size(); i++)
    {
        if(str[i]==':') {
            for(int j=i+1;j<str.size();j++){
                if(str[j]=='|') break;
                 else  num++;
            }
            break;
        }
        index++;
    }
    //验证发过来的字符串和密码进行拆分
   
    strUser=str.substr(0,index);
    strPassword=str.substr(index+1,num);
     std::cout<<strUser<<std::endl;
     std::cout<<strPassword<<std::endl;
    
    //进行登陆验证测试
   
     std::cout << "Querying data..." << std::endl;
     if(db.query("SELECT * FROM login;",strUser,strPassword)==true){
        login_buf="yes\0";
        int len=strlen(login_buf);
         const std::string upsockfd="UPDATE login SET sockfd = "+std::to_string(m_sockfd)+" WHERE username='"+strUser+"';";
         std::cout<<"修改语句为：  "<<upsockfd<<std::endl;
        db.update(upsockfd);
        //验证成功以后，判断数据库中是否存在该账号的好友表，存在则不用创建，不存在则创建！，用于主界面加载好友时使用
        db.createfiendtable(strUser);
       //将验证成功的消息发送给客户端
        write(m_sockfd,len);
        isLogin=true;
     }else{
        std::cout<<"验证失败"<<std::endl;
        login_buf="no\0";
         int len=strlen(login_buf);
        write(m_sockfd,len);

     }
     std::cout<<"socketfd:  "<<m_sockfd<<std::endl;

    }
   //登陆验证成功以后！！！
   //获取好友的消息列表信号
      else if(getfriedslgin==false){
        std::cout<<"登陆后:"<<str<<std::endl;
        //将查询好的数据放入vector容器中
        db.query("SELECT * FROM friendtable"+strUser,getfriends);
        //将容器中的数据取出发送到客户端
            write(m_sockfd,getfriends);
        getfriedslgin=true;
        }
        //添加好友！
        else if(str[0]=='|'){
              int index(0);
              for(int i=1;i<str.size();i++){
                if(str[i]==':') break;
                else index++;
              }

            std::cout<<"添加好友的语句和内容为："<<str<<"  "<<"INSERT INTO friendtable"+strUser+" (username) VALUES ('"+str.substr(1,index)+"');"<<std::endl;


            if(db.query("select * from login",str.substr(1,index))==true){
             db.insert("INSERT INTO friendtable"+strUser+" (username) VALUES ('"+str.substr(1,index)+"');");
              write(m_sockfd,"&");
            }else{
                std::cout<<"无当前账号"<<std::endl;
                write(m_sockfd,"@");
            }



              
        }
        //删除好友
        else if(str[0]=='*'){

               int index(0);
              for(int i=1;i<str.size();i++){
                if(str[i]==':') break;
                else index++;
              }
              std::cout<<"删除好友的账号为: "<<str.substr(1,index)<<std::endl;
              db.update("DELETE FROM friendtable"+strUser+"  WHERE username="+str.substr(1,index));   

        }
        //修改密码
        else if(str[0]=='&'){
               int index(0);
              for(int i=1;i<str.size();i++){
                if(str[i]==':') break;
                else index++;
              }
             std::cout<<"修改密码为为: "<<str.substr(1,index)<<std::endl;
             std::cout<<"sql语句为: "<<"updata login set password="+str.substr(1,index)+"  where username="+strUser<<std::endl;
             db.update("update login set password="+str.substr(1,index)+" where username="+strUser);
        }
     //发送好友消息
        else if(str[0]==':')
        {
            //如果不是群聊消息
            if(str.substr(0,6)!=":group"){
             //将客户端发送过来的消息转发给指定用户的客户端上去
             int head=0,last=0,rear=0;
             //获得对方的账号
             getHeadandLast(str,head,last,rear);
             std::cout<<"收到的消息为："<<str<<std::endl;
             std::cout<<head<<" "<<last<<" "<<rear<<std::endl;
             //在login表中，通过对方的账号查到对方的sockfd，在根据sockfd发送到对方客户端去！
             //这里的参数str需要截取出！
             //将要发送的内容组装好，发送给客户端包含:自己的账号+信息，用|字符作为隔断字符
             std::string wrtieTofriend=strUser+"|"+str.substr(last+1,rear-last-1);
             write(db.getsockfd("SELECT *FROM login",str.substr(head,last-head)),wrtieTofriend);  
            }else{
                 std::cout<<"收到的群聊消息为："<<str<<std::endl;
                  int index(0),index1,index2,index3;

                 for (size_t i = 0; i < str.size(); i++)
                 {
                    if(str[i]==':'){
                        index++;
                           if(index==2){
                         index1=i;
                           }
                    if(index==3){
                        index2=i;
                    }
                    if(index==4){
                         index3=i;
                    }
                    }
                 
                 }
                 std::cout<<str.substr(index1+1,index2-index1-1)<<std::endl;

                 
                 std::vector<std::pair<int,std::string>> v=db.sendGruopMessage(str.substr(index1+1,index2-index1-1));

                  //查询是谁发的
                 std::string who_send;
                 for (const auto& pair : v) {
                    std::cout << "Integer: " << pair.first << ", String: " << pair.second << std::endl;
                    if(pair.first==m_sockfd){
                     who_send=pair.second;
                     std::cout<<"这是："<<who_send<<" 发送的"<<std::endl;
                    }
                }


                 for(int i=0;i<v.size();i++){
                     std::cout<<"用户socket为: "<<v[i].first<<std::endl;
                     if(v[i].first!=m_sockfd)
                     write(v[i].first,"group:"+str.substr(index1+1,index2-index1-1)+":"+who_send+"|"+str.substr(index2+1,index3-index2-1));
                     
                 }

            }


        }else if(str[0]=='$'){
            //传输文件的内容为

            std::cout<<str<<std::endl;
           for (size_t i = 0; i <str.size(); i++)
           {
                if(str[i]=='+'){
                    std::cout<<"yes"<<std::endl;
                    std::cout<<str.substr(1,i-1)<<std::endl;
                    write(db.getsockfd("SELECT *FROM login",str.substr(1,i-1)),str);  
                    break;
                }
           }     
        }
        //建立群聊
        else if (str[0]=='~')
        {
             std::cout<<"当前为建立群聊："<<str<<std::endl;
             std::vector<std::string> v;
               std::string str1;
             for (size_t i = 0; i < str.size(); i++)
             {
             
                if(str[i]=='~'){
                    if(str1.empty()!=true){
                         v.push_back(str1);
                         str1.clear();
                    }
                    continue;
                }
                str1+=str[i];
             }
             for(auto &x:v) std::cout<<x<<std::endl;
               db.creatGroup(v);
               
               for(int i=0;i<v.size()-1;i++){
                   int fd=db.getsockfd("SELECT *FROM login",v[i]);
                    if(fd!=m_sockfd)
                   write(fd,"~"+v.back());
               }
           db.insertIntofriend(v,v.back());

        }
        //ai智能回复
        else {
            std::cout<<str<<std::endl;
              ai_to_recv ai;
            std::string isAiRecv=ai.get_ai_recv(str);
            if(isAiRecv!=""){
                 std::cout<<"ai的回答是: \n"<<isAiRecv<<std::endl;
                 write(m_sockfd,"*"+isAiRecv);
            }else{
                std::cout<<"http请求错误"<<std::endl;
            }
           
        }
        
    
    

     
     str.clear();
      //清楚掉读取数据的读缓冲区，避免下次读取会有上一次的数据残留
     free(m_read_buf);
     m_read_buf=(char*)malloc(READ_BUFFER_SIZE);
    
} 
