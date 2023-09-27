/*********************************
 * Reviewer: Who knows
 * Author: Yohai Azoulai
 * File: threadpool.hpp
 * ******************************/

#pragma once

#define UNUSED(expr) do { (void)(expr); } while (0)


#include <iostream>
#include <thread>            /* std::thread */
#include <future>            /* std::async std::future */
#include <atomic>            /* std::atomic int for enqueue flag*/
#include "function.hpp"      /* in order to use our function object*/
#include "btsqueue.hpp"      /* in order to use our blocking threadsafe queue*/
#include "semaphore.hpp"     /* in order to use our semaphore*/
#include "singleton.hpp"     /* in order to */

const int g_bad_apple = 0xbadbab;
const int NUM_OF_THREADS = 4;



class ThreadPool
{
public :

template <typename T>
friend class Singleton;

class Future;

public:
    
//  Done 
/* template <typename T >
 */
    
    
    Future Async(const Function<int(void)>& func);
    static int BadApple(int a);
    void ShutDown();      // terminate New tasks! finishes what currently in queue
    void Cancel();        // cancel all tasks and empties the queue
    void Joining();
    
    ThreadPool(const ThreadPool& other) = delete;
    void operator=(const ThreadPool& other) = delete;

    explicit ThreadPool(int num_of_threads = NUM_OF_THREADS);
    ~ThreadPool(){};
private:
    // Check If Done
    class Task
    {
    public :
        Task(const Function<int(void)> &func, int priority);
        int AccessFunc();
        void SemPost();
        void SemWait();
        int GetStatus();
        int GetPriority();

    private :
        Function<int(void)> func_to_run;
        Semaphore m_sarah;
        int m_status;
        int m_priority;
    }; // class Task

    class Compare
    {
    public : 
        bool operator()(const std::shared_ptr<Task>& lhs, const std::shared_ptr<Task>& rhs)
        {
            UNUSED(lhs);
            UNUSED(rhs);
            return true;
        }
    }; // class Compare

    std::vector<std::thread> m_threads;
    BtsQueue<std::shared_ptr<Task>, Compare> m_tasks;
    std::atomic<int> m_atomic_open_flag;
    void TaskDispatcher();

}; //class ThreadPool


class ThreadPool::Future
{
public:
    explicit Future(const std::shared_ptr<Task> &s_ptr = nullptr);
    // if i want to utilize template the Wait func return type is <T>
    int Wait() const;
    
private:
    std::shared_ptr<Task> m_ptr;
}; //class Future


