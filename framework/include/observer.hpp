/*********************************
 * Reviewer: Valve man				
 * Author: Yohai Azoulai			
 * File: observer.hpp					
 * ******************************/

#pragma once

#define UNUSED(expr) do { (void)(expr); } while (0)

#include <unordered_map>
#include <memory>
#include <set>
#include <stdint.h>         /* uint32_t */

#include "function.hpp"


template<typename RET>
class Callback;


template<typename EVENT>
class Dispatcher
{

public:
    ~Dispatcher();
    void Add(const Callback<EVENT>& callback);
    void Remove(const Callback<EVENT>& callback);
    void Notify(EVENT event);
    
private:
    std::set<const Callback<EVENT>*> m_callbacks;

}; // class Dispatcher

template<typename EVENT>
class Callback
{
public:
    Callback(const Function<void(EVENT)>& func);
    ~Callback();
    void BindDispatcher(const Dispatcher<EVENT> &dispatcher) const; //assgin pointer to the the dispatcher
    void UnBindDispatcher() const; //assgin nullptr pointer to the the dispatcher

private:
    friend class Dispatcher<EVENT>;

    const Function<void(EVENT)> m_function;
    mutable Dispatcher<EVENT> *m_dispatcher;
}; // class Callback

// -----------------Dispatcher Methods ------------------
template <typename EVENT>
inline Dispatcher<EVENT>::~Dispatcher()
{
    typename std::set<const Callback <EVENT>* >::const_iterator iter;
    iter = m_callbacks.begin();
    while(iter != m_callbacks.end())
    {
        (*iter)->UnBindDispatcher(); // because of dangling pointer to callbacks
        ++iter;
    }
}

template <typename EVENT>
inline void Dispatcher<EVENT>::Add(const Callback<EVENT> &callback)
{
    m_callbacks.insert(&callback);
    callback.BindDispatcher(*this);
}

template <typename EVENT>
inline void Dispatcher<EVENT>::Remove(const Callback<EVENT> &callback)
{
    callback.UnBindDispatcher();
    m_callbacks.erase(&callback);
}


template <typename EVENT>
inline void Dispatcher<EVENT>::Notify(EVENT event)
{
    typename std::set<const Callback <EVENT> * >::const_iterator iter;
    
    iter = m_callbacks.begin();
    while (iter != m_callbacks.end())
    {
        (*iter)->m_function(event);
        ++iter;
    }
}

// -----------------Callback Methods --------------------------------

template <typename EVENT>
inline Callback<EVENT>::Callback(const Function<void(EVENT)> &func):
        m_function(func) 
{}

template <typename EVENT>
inline Callback<EVENT>::~Callback()
{
    if (m_dispatcher)
    {
        m_dispatcher->Remove(*this);
        m_dispatcher = nullptr;
    }
}

template <typename EVENT>
inline void Callback<EVENT>::BindDispatcher(const Dispatcher<EVENT> &dispatcher) const
{
    m_dispatcher = const_cast<Dispatcher<EVENT>*>(&dispatcher);
}

template <typename EVENT>
inline void Callback<EVENT>::UnBindDispatcher() const
{
    m_dispatcher = nullptr;
}


