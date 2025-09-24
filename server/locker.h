#ifndef LOCKER_H
#define LOCKER_H
//线程同步封装机制

#include <pthread.h>//提供线程互斥锁和条件变量,以及互斥锁和条件变量创建函数，加锁，解锁函数，销毁函数等等
#include <exception>
#include <semaphore.h>
class locker{
public:
  locker(){
    if(pthread_mutex_init(&m_mutex,NULL)!=0){//调用Linux本身的锁类的锁初始化函数对锁初始化
       throw std::exception();//创建有问题则抛出异常
    }
  }
  //加锁
  bool lock(){
    return pthread_mutex_lock(&m_mutex) == 0;
  }
  bool unlock(){
    return pthread_mutex_unlock(&m_mutex) == 0;
  }
//解锁
  pthread_mutex_t * get(){
    return &m_mutex;
  }

  ~locker(){
    pthread_mutex_destroy(&m_mutex);//调用线程锁销毁函数进行资源销毁
  }
private:
 pthread_mutex_t  m_mutex;//线程互斥锁
};
//条件变量
class cond{
    public:
 cond(){
    if(pthread_cond_init(&m_cond,NULL)!=0){
        throw std::exception();
        }
       
 }
 ~cond(){
    pthread_cond_destroy(&m_cond);
 }
  //封装一个条件等待
  bool  wait(pthread_mutex_t *mutex){
     return pthread_cond_wait(&m_cond,mutex) == 0;
  }
  //超时等待
  bool  timwait(pthread_mutex_t *mutex, struct timespec t)
  {
     
     return pthread_cond_timedwait(&m_cond,mutex,&t) == 0;
  };

  //单个线程换醒
  bool signal( ){
    return pthread_cond_signal(&m_cond)==0;
  }
  //全部线程唤醒
bool broadcast( ){
    return pthread_cond_broadcast(&m_cond)==0;
  }
 

 private:
    pthread_cond_t m_cond;
};

//信号量类
class sem{
    public:
     
     sem(){
   if(sem_init(&m_sem,0,0)!=0){
    throw std::exception();
   }
     }
     ~sem(){
     sem_destroy(&m_sem);
     }
       sem(int num){
       if(sem_init(&m_sem,0,num)!=0){
        throw std::exception();
        }
     }
bool wait(){
    return sem_wait(&m_sem)==0;
}
bool post(){
    return sem_post(&m_sem)==0;
}
   

    private:
    sem_t m_sem;
};
#endif
//将线程同步封装好后，在线程池中直接引用该头，调用各种接口进行加解锁，以及条件等待，信号量等