/*********************************
 * Author: Yohai Azoulai			
 * File: function.hpp					
 * ******************************/

#pragma once

#include <memory> /* In order to use shared_ptr*/
#include "yoh_color.h"



template<typename>
class Function;

//---------------------------------------BaseClass----------------------------------------------------//

template<typename RET>
class Function<RET(void)>
{
public:
    explicit Function(){}
    template<typename T , typename BIND_ARG>
    explicit Function(RET (T::*callable)(BIND_ARG arg), T *object, BIND_ARG arg);
        
    template<typename BIND_ARG>
    explicit Function(RET (*callable)(BIND_ARG arg), BIND_ARG arg);
    // NO destructor Needed (all dynamic is done by shared PTR)
    /* virtual ~Function(); */
    
    RET operator()();    //Will Activate derived RUN();
      
private:
    class IFunction;
    std::shared_ptr<IFunction> m_ifunction;
    
    template <typename BIND_ARG>
    class FreeFunction;
    
    template<typename T , typename BIND_ARG>
    class MemberFunction;
}; // Class Function

//---------------------------------------InterfaceClass-----------------------------------------------//

template<typename RET>
class Function<RET(void)>::IFunction
{
public :
    explicit IFunction() {}
    // NO destructor Needed (all dynamic is done by shared PTR)
    /* virtual ~IFunction() {} */
    virtual RET Run() = 0;
}; // class Function<RET(void)>::IFunction

//---------------------------------------Free FuncClass-----------------------------------------------//
template<typename RET>
template<typename BIND_ARG>
class Function<RET(void)>::FreeFunction: public Function::IFunction
{
public:
    explicit FreeFunction(RET (*callable)(BIND_ARG arg), BIND_ARG arg);
    // destructor dependent on use
    RET Run();

private:
    RET (*m_callable)(BIND_ARG arg);
    BIND_ARG m_arg;
}; // class Function<RET(void)>::FreeFuncion

//---------------------------------------Member FuncClass---------------------------------------------//
template<typename RET>
template<typename T, typename BIND_ARG>
class Function<RET(void)>::MemberFunction: public Function::IFunction
{
public:
    explicit MemberFunction(RET (T::*callable)(BIND_ARG arg), T *object, BIND_ARG arg);
    // NO destructor Needed (all dynamic is done by shared PTR)

    RET Run();

private:
    RET (T::*m_callable)(BIND_ARG arg);     // func ptr
    T* m_object;                            // object
    BIND_ARG m_arg;                         // arguments passed
};


template <typename RET>
template <typename BIND_ARG>
inline Function<RET(void)>::FreeFunction<BIND_ARG>::FreeFunction(RET (*callable)(BIND_ARG arg),
            BIND_ARG arg) : m_callable(callable), m_arg(arg)
{}

template <typename RET>
template <typename BIND_ARG>
inline RET Function<RET(void)>::FreeFunction<BIND_ARG>::Run()
{
    return m_callable(m_arg);
}


//---------------------------------------Member FuncClass---------------------------------------------//

template <typename RET>
template <typename T,typename BIND_ARG>
inline Function<RET(void)>::MemberFunction<T, BIND_ARG>::MemberFunction(RET (T::*callable)(BIND_ARG arg),
            T *object, BIND_ARG arg) : m_callable(callable), m_object(object), m_arg(arg)  
{}

template <typename RET>
template <typename T, typename BIND_ARG>
inline RET Function<RET(void)>::MemberFunction<T, BIND_ARG>::Run()
{
    return (m_object->*m_callable)(m_arg);
}

//-------*************---------New Template BaseClass----------*************

template<typename RET, typename INVOCATION>
class Function<RET(INVOCATION)>
{
public:
    explicit Function(){}
    template<typename T>
    explicit Function(RET (T::*callable)(INVOCATION), T *object);
        
    explicit Function(RET (*callable)(INVOCATION));
    // NO destructor Needed (all dynamic is done by shared PTR)
    /* virtual ~Function(); */
    
    RET operator()(INVOCATION arg) const;    //Will Activate derived RUN();
      
private:
    class IFunction;
    std::shared_ptr<IFunction> m_ifunction;
    
    class FreeFunction;
    
    template<typename T>
    class MemberFunction;
}; // Class Function<INVOCATION>

//---------------------------------------InterfaceClass-----------------------------------------------//

template<typename RET, typename INVOCATION>
class Function<RET(INVOCATION)>::IFunction
{
public :
    explicit IFunction() {}
    // NO destructor Needed (all dynamic is done by shared PTR)
    /* virtual ~IFunction() {} */
    virtual RET Run(INVOCATION) = 0;
}; // class Function<RET(INVOCATION)>::IFunction

//---------------------------------------Free FuncClass-----------------------------------------------//
template<typename RET, typename INVOCATION>
class Function<RET(INVOCATION)>::FreeFunction: public Function<RET(INVOCATION)>::IFunction
{
public:
    explicit FreeFunction(RET (*callable)(INVOCATION));
    // destructor dependent on use
    RET Run(INVOCATION arg);

private:
    RET (*m_callable)(INVOCATION);
}; // class Function<RET(INVOCATION)>::FreeFuncion

//---------------------------------------Member FuncClass---------------------------------------------//
template<typename RET , typename INVOCATION>
template<typename T>
class Function<RET(INVOCATION)>::MemberFunction: public Function<RET(INVOCATION)>::IFunction
{
public:
    explicit MemberFunction(RET (T::*callable)(INVOCATION), T *object);
    // NO destructor Needed (all dynamic is done by shared PTR)

    RET Run(INVOCATION arg);

private:
    RET (T::*m_callable)(INVOCATION);     // func ptr
    T* m_object;                              // object
}; // class MemberFunction<INVOCATION>


template <typename RET, typename INVOCATION>
template <typename T>
inline Function<RET(INVOCATION)>::Function(RET (T::*callable)(INVOCATION), T *object)
: m_ifunction(new Function<RET(INVOCATION)>::MemberFunction<T>(callable,object) ) {}

template <typename RET, typename INVOCATION>
inline Function<RET(INVOCATION)>::Function(RET (*callable)(INVOCATION))
 : m_ifunction(new Function<RET(INVOCATION)>::FreeFunction(callable) ) {}

template <typename RET, typename INVOCATION>
inline RET Function<RET(INVOCATION)>::operator()(INVOCATION arg) const
{
    return m_ifunction->Run(arg);
}



//-----------------------------------Base Function Defenitions----------------------------------------//

// Malloc per corresponding type required


//-----------------------------------Interface Function Defenitions-----------------------------------//



//---------------------------------------Free FuncClass-----------------------------------------------//
template<typename RET, typename INVOCATION>
inline Function<RET(INVOCATION)>::FreeFunction::FreeFunction(RET (*callable)(INVOCATION)): m_callable(callable) {}

template<typename RET, typename INVOCATION>
inline RET Function<RET(INVOCATION)>::FreeFunction::Run(INVOCATION arg)
{
    return m_callable(arg);
}

template<typename RET, typename INVOCATION>
template <typename T>
inline Function<RET(INVOCATION)>::MemberFunction<T>::MemberFunction(RET (T::*callable)(INVOCATION), T *object)
 : m_callable(callable), m_object(object)  
 {}

template<typename RET, typename INVOCATION>
template <typename T>
inline RET Function<RET(INVOCATION)>::MemberFunction<T>::Run(INVOCATION arg)
{
    return (m_object->*m_callable)(arg);
}

template <typename RET>
template <typename T, typename BIND_ARG>
inline Function<RET(void)>::Function(RET (T::*callable)(BIND_ARG arg), T *object, BIND_ARG arg)
: m_ifunction(new MemberFunction<T, BIND_ARG>(callable, object, arg))
{
}

template <typename RET>
template <typename BIND_ARG>
inline Function<RET(void)>::Function(RET (*callable)(BIND_ARG arg), BIND_ARG arg)
 : m_ifunction(new FreeFunction<BIND_ARG>(callable, arg))
{
}

template <typename RET>
inline RET Function<RET(void)>::operator()()
{
    return m_ifunction->Run();
}


