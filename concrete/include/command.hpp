/*********************************
 * Reviewer: Daniel gityamin				
 * Author: Yohai Azoulai			
 * File: command.hpp					
 *********************************/

#pragma once
#include <cstdint>          /* uint32_t */
#include <memory>           /* shared_ptr */
#include <sys/types.h>      /* sendmsg */
#include <sys/socket.h>     /* socket */


#include "nbd_comm.h" 
#include "yoh_color.h"

#define UNUSED(expr) do { (void)(expr); } while (0)


/********************************** Command **********************************/    

class Command
{

public:
    explicit Command(std::shared_ptr<slice_t> msgInfo);
    virtual int Send();
    virtual ~Command(){}
    virtual void Build() = 0;

protected:

    std::shared_ptr<slice_t> m_msgInfo;
};

/*********************************** Write ***********************************/    

class Write : public Command
{
public:
    explicit Write(std::shared_ptr<slice_t> msgInfo);
    ~Write(){};
    void Build();
};


/*********************************** Read ***********************************/    

class Read : public Command
{

public:
    explicit Read(std::shared_ptr<slice_t> msgInfo);
    ~Read(){};
    void Build();

};



