
/*********************************
 * Reviewer: 
 * Author: Mike Meyers
 * File: reactor.hpp
 * ******************************/

#pragma once
#include <unordered_map>

#include "imonitor.hpp"
#include "function.hpp"


template<typename MONITOR> 
class Reactor
{
public:
  explicit Reactor();
	~Reactor();
    
  void RegisterReadHandler(int fd, const Function<int(void)>& func);
  void Run();
  //Void Stop();
  
private:
	std::unordered_map<int, Function<int(void)> > m_handlers;
    MONITOR m_monit;

}; // ****************class MONITOR*******************




// Ctor Initializes Heap for Task Handeling
// initializes the Monitor instance with type
template <typename MONITOR>
inline Reactor<MONITOR>::Reactor() : m_handlers() , m_monit(MONITOR())
{
    (void)static_cast<IMonitor*>(&m_monit);
}
// generic Dtor
template <typename MONITOR>
inline Reactor<MONITOR>::~Reactor(){}


template <typename MONITOR>
inline void Reactor<MONITOR>::RegisterReadHandler(int fd, const Function<int(void)>& func)
{
    //inserts and attaches the handler function with correct fd
    // by having the fd as the index of the handler function inside LUT
    // registring the file descriptor in Monitored list for select
    
    m_handlers[fd] = func;
    m_monit.SetRead(fd);
}

template <typename MONITOR>
inline void Reactor<MONITOR>::Run()
{
    int n = 0;
    while (1)
    {
        n = m_monit.WaitForEvent();

        while(-1 != (n = m_monit.GetNextReadFd()))
        {
            (m_handlers[n])();	//returns a Function from the map, activates the Functor
	    }
    }
}


