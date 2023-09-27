/*********************************
 * Reviewer: Who knows				
 * Author: Yohai Azoulai			
 * File: semaphore.hpp					
 * ******************************/

#pragma once

#define UNUSED(expr) do { (void)(expr); } while (0)

#include <mutex>
#include <atomic>
#include <condition_variable>
#include "yoh_color.h"


class Semaphore 
{
public:
    Semaphore (int count_ = 0);
    void Post();
    void Wait();

private:
    std::mutex m_mtx;
    std::condition_variable m_cv;
    std::unique_lock<std::mutex> sarah_lock;
    std::atomic_bool m_count;
}; // class Semaphore
 

