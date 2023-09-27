/*********************************
 * Reviewer:  
 * Author: Irit Ofir			
 * File: singleton.hpp
 * ******************************/

#pragma once

#include <atomic>
#include <stdlib.h>

template<typename T>
class Singleton
{
public:
    explicit Singleton();
    T* operator->();
    ~Singleton() = default; 

    Singleton(Singleton &other) = delete;        //prevent copy
    void operator=(const Singleton &) = delete;    //prevent assignment

private:
    static void CleanUp();
    static T *m_instance;
    static std::atomic_bool m_lock;
};// class Singleton

template<typename T>
T* Singleton<T>::m_instance = nullptr;

template<typename T>
std::atomic_bool Singleton<T>::m_lock(1);

template <typename T>
inline Singleton<T>::Singleton()
{
    if(!m_instance)
    {
        while(!m_lock.load());

        m_lock.store(0);

        if (!m_instance)
        {
            m_instance = new T;
            atexit(CleanUp);
        }
        m_lock.store(1);
    }
}

template <typename T>
inline T *Singleton<T>::operator->()
{
    return m_instance;
}

template <typename T>
inline void Singleton<T>::CleanUp()
{
    delete m_instance;
    m_instance = reinterpret_cast<T*>(0xdeadbeef);
}


