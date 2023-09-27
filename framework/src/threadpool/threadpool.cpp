/*********************************
 * Reviewer: Who knows
 * Author: Yohai Azoulai
 * File: threadpool.cpp
 * ******************************/

/*  */

#include <mutex>
#include <utility>
#include "threadpool.hpp"

/*******Defines******/



/*******Structs******/


/*******Service Funcs******/


/*******Funcs Funcs******/

ThreadPool::ThreadPool(int num_of_threads) : m_threads(num_of_threads) , m_atomic_open_flag(1) 
{
    for (int i = 0; i < num_of_threads; ++i)
    {
        m_threads.emplace_back(std::thread(&ThreadPool::TaskDispatcher,this));
    }
    
    /* m_threads.reserve(num_of_threads);
    for (int i = 0; i < num_of_threads; ++i)
    {
        m_threads.emplace_back(&ThreadPool::TaskDispatcher,this);
    }
    printf("ThreadPool ctor called by\n"); */
}

/* ThreadPool::~ThreadPool()
{
    if(m_atomic_open_flag)
    {
        ShutDown();
    }
} */

// formaly this needs to recive Priority as well
ThreadPool::Future ThreadPool::Async(const Function<int(void)> &func)
{
    if(m_atomic_open_flag)
    {
        std::shared_ptr<Task> place_holder(new Task(func, 0));
        m_tasks.Enqueue(place_holder);
        return Future(place_holder);
    }
    return Future(std::shared_ptr<Task>(new Task(Function<int(void)>(BadApple,g_bad_apple), 0)));
}

void ThreadPool::ShutDown()
{
    Function<int(void)> hungry_for_apples(BadApple,g_bad_apple);

    for (size_t i = 0; i < m_threads.size() ; ++i)
    {
        Async(hungry_for_apples);
    }
    
    /* Joining(); */
     /* for (size_t i = 0; i < m_threads.size() ; ++i)
    {
        m_threads[i].join();
    } */ 
/*     m_threads.clear();
 */}

void ThreadPool::Joining()
{
    for (size_t i = 0; i < m_threads.size() ; ++i)
    {
        m_threads[i].join();
    }
}

void ThreadPool::Cancel()
{
    m_atomic_open_flag.fetch_sub(1);
    ShutDown();
    m_tasks.Clear();
}

void ThreadPool::TaskDispatcher()
{
    size_t i = 0;
    while (m_atomic_open_flag && (i != g_bad_apple))
    {
        std::shared_ptr<Task> tmp_ptr;
        m_tasks.Dequeue(tmp_ptr);
        i = tmp_ptr->AccessFunc();
    }
}

ThreadPool::Task::Task(const Function<int(void)> &func, int priority) :  func_to_run(func) , m_status(60221023) , m_priority(priority) {}

int ThreadPool::Task::AccessFunc()
{
    m_status = func_to_run();
    SemPost();
    return m_status;
}

void ThreadPool::Task::SemPost()
{
    m_sarah.Post();
}
void ThreadPool::Task::SemWait()
{
    m_sarah.Wait();
}
int ThreadPool::Task::GetStatus()
{
    return m_status;
}
int ThreadPool::Task::GetPriority()
{
    return m_priority;
}
// Future Ctor
/* ThreadPool::Future::Future(const std::shared_ptr<Task> &s_ptr) : m_ptr(s_ptr)
{} 

 
int ThreadPool::Future::Wait() const
{
    m_ptr->SemWait();
    return m_ptr->GetStatus();
}
*/
int ThreadPool::BadApple(int bad_apple)
{
    (void)bad_apple;
    return g_bad_apple;
} 


ThreadPool::Future::Future(const std::shared_ptr<Task> &s_ptr) : 
    m_ptr(s_ptr){}

int ThreadPool::Future::Wait() const
{
    if(g_bad_apple != m_ptr->GetStatus()) 
    {
        m_ptr->SemWait();
        return m_ptr->GetStatus();
    }
    return -1;
}
