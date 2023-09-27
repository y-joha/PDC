
/*********************************
 * Author: Yohai AZoulai
 * File: reactor.hpp
 * ******************************/

#pragma once
#include <unordered_map>



class IMonitor
{
public:
    virtual int WaitForEvent() = 0;
    virtual int GetNextReadFd() = 0;
    
    virtual void SetRead(int fd) = 0;
    virtual void ClearRead(int fd) = 0;
   
}; // IMonitor class


