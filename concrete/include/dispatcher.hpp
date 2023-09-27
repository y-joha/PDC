/*********************************
 * Reviewer: Unknown				
 * Author: Yohai Azoulai			
 * File: dispatcher.hpp					
 * ******************************/

#pragma once

#define UNUSED(expr) do { (void)(expr); } while (0)
 
#include <memory> /* shared_ptr */
#include <unordered_map> /* unordered_map */

#include "request_host.hpp" /* RequestHost */
#include "yoh_color.h"



class RequestDispatcher
{

public:

    int NbdRequestHandler(std::vector<int> sockets);
    int IotRequestHandler(int sock);

private:

    std::unordered_map<uint32_t,std::shared_ptr<RequestHost> > m_activeRequest;
};



