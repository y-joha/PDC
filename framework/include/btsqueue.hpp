/*********************************
 * Reviewer: Rabi Kbir				
 * Author: Yohai Azoulai			
 * File: btsqueue.hpp					
 * ******************************/

#pragma once

#define UNUSED(expr) do { (void)(expr); } while (0)

#include <queue>                /*std::priority_queue*/
#include <mutex>                /*mutex*/
#include <vector>               /*vector*/
#include <functional>           /*functional*/
#include <condition_variable>   /*conditional variable*/



template<typename T, typename COMPARE = std::less<T> >
class BtsQueue
{
public:
    BtsQueue() = default;
    void Enqueue(const T& task);
    void Dequeue(T& task);
	void Clear();

    BtsQueue(BtsQueue const &) = delete;
    void operator=(const BtsQueue& other) = delete;

private:
    // Not allowing copy
	std::priority_queue<T,std::vector<T>,COMPARE> m_priqueue;
	std::mutex m_lock_dequeue;
    std::condition_variable m_cond;

}; // class PriQueue

template <typename T, typename COMPARE>
inline void BtsQueue<T, COMPARE>::Enqueue(const T &task)
{
    {
        std::unique_lock<std::mutex> uni_lock(m_lock_dequeue);
        //uni_lock.lock();
        m_priqueue.push(task);
    }
    m_cond.notify_one();
}

template <typename T, typename COMPARE>
inline void BtsQueue<T, COMPARE>::Dequeue(T &task)
{
    std::unique_lock<std::mutex> uni_lock(m_lock_dequeue);
    //uni_lock.lock(); 
    
    while(m_priqueue.empty())
    {
        m_cond.wait(uni_lock); //, [this]{return !m_priqueue.empty();});
    }
    task = m_priqueue.top();
    m_priqueue.pop();
}

template <typename T, typename COMPARE>
inline void BtsQueue<T, COMPARE>::Clear()
{
    std::unique_lock<std::mutex> uni_lock(m_lock_dequeue);
    while (!m_priqueue.empty())
    {
        m_priqueue.pop();
    }
}

