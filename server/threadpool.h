#ifndef THREADPOOL
#define THREADPOOL
//实现线程池
//线程中封装实现了线程的开辟和销毁，请求队列的创建与任务的添加
#include<pthread.h>
#include<list> 
#include"locker.h"
#include<iostream>
#include<exception>
//线程池中任务采用模版的方式,定义成模版类是为了代码复用，T就是任务类
template<class T>
class threadpool{
public:
threadpool(int thread_number=8,int max_requests=10000);
~threadpool();
//添加任务队列
bool append(T *request);
//使线程去工作队列取任务执行
void run();
private:
//c++中线程去执行的任务函数必须要是全局的或者静态的，因为线程开辟后，子线程只能访问全局或静态变量或者函数
//局部变量或者函数的地址在栈区，只能局部内访问，生命周期短
static void * worker(void *arg);

private:
//线程数量
int m_thread_number;

//线程数组,指针类型
   pthread_t *m_threads;
   //请求队列中最多允许的等待处理的请求数量
   int m_max_requests;
//请求队列
std::list<T*> m_workqueue;
//互斥锁
locker m_queuelocker;
//信号量
sem m_queuestat;
//是否结束线程
bool m_stop;
};
template<class T>
threadpool<T>::threadpool(int thread_number,int max_requests){
    this->m_thread_number=thread_number;
    this->m_max_requests=max_requests;
    this->m_stop=false;
    this->m_threads=NULL;
    if(thread_number<=0||max_requests<=0){
        throw std::exception();
    }
    m_threads=new pthread_t[m_thread_number];//指定开辟线程数量
     if(!m_threads){
        throw std::exception();
     }
     //创建thread_number个线程
     for (int i = 0; i < thread_number; i++)
     {
        std::cout<<"create the "<<i<<"thread"<<std::endl;
        //循环创建每一个线程，传递this表示后面强转时可以转成参数类型的数据结构
        if(pthread_create(m_threads+i,NULL,worker,this)!=0){
            //如果有异常，销毁并抛出异常
              delete [] m_threads;
             throw std::exception();
        }
        //创建成功，设置线程分离
        if(pthread_detach(m_threads[i])){
            delete [] m_threads;
            throw std::exception();
        }
     }
     
}
template<class T>
threadpool<T>::~threadpool(){
    delete []  m_threads;
    m_stop=true;

}
template<class T>
bool threadpool<T>::append(T *request){
    //保证线程同步
    m_queuelocker.lock();
    //判断请求队列中是否是否大于最大请求，如果大于则停止添加请求
    if(m_workqueue.size()>m_max_requests){
        m_queuelocker.unlock();
        return false;
    }
    m_workqueue.push_back(request);
    //添加完任务后解锁
    m_queuelocker.unlock();
    //表示增加一个任务，增加一个信号量，用于判断线程是否要阻塞
    m_queuestat.post();
    return true;
}
//使每个线程都去运行这个工作函数,不直接使用run的原因是子线程的任务函数只能使用全局或静态的
template<class T>
void * threadpool<T>::worker(void * arg){

       threadpool *pool=(threadpool *) arg;
       pool->run();
       return pool;
}

template<class T>
void threadpool<T>::run(){
     while(!m_stop){
        //判断是否有任务，没任务则线程阻塞，有任务则添加
         m_queuestat.wait();
         m_queuelocker.lock();
        if(m_workqueue.empty()){
            m_queuelocker.unlock();
            continue;
        }
       T* request=m_workqueue.front();
        m_workqueue.pop_front();
        m_queuelocker.unlock();
        if(!request){
            continue;
        }
        //取到任务执行任务，后面会定义一个任务类
        request->process();

     }
}

#endif