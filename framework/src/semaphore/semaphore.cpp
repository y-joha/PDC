/*********************************
 * Reviewer: Who knows				
 * Author: Yohai Azoulai			
 * File: semaphore.cpp					
 * ******************************/
 
 /*  */


#include "semaphore.hpp"
/*******Defines******/



/*******Structs******/



/*******Service Funcs******/



/*******Funcs Funcs******/

Semaphore::Semaphore(int count_) : m_count(count_)
{}

void Semaphore::Post()
{
    std::unique_lock<std::mutex> sarah_lock(m_mtx);
    m_count.store(1);
    sarah_lock.unlock();
    m_cv.notify_one();
}

void Semaphore::Wait()
{
    std::unique_lock<std::mutex> sarah_lock(m_mtx);
    while(!m_count)
    {
        m_cv.wait(sarah_lock);
    }
    m_count.store(0);
    
}
