/*********************************
 * Reviewer: No body				
 * Author: Yohai Azoulai			
 * File: transmitter.hpp					
 * ******************************/

#pragma once
#define UNUSED(expr) do { (void)(expr); } while (0)


#include <set>
#include <memory>
#include "observer.hpp"
#include "command.hpp"
#include "yoh_color.h"


class Transmitter
{
public:
    bool HandleReply(int socket);
    int SplitNSendTask(std::shared_ptr<slice_t> messeg_per_iot);
    static void NotifyTheSubs(slice_t messeg);
    static void Add(const Callback<slice_t>& callback); //AtlasHeader? not TaskInfo?
private:
    std::set<uint32_t> m_set_fregs_to_publish;
    static Dispatcher<slice_t>* m_dispatcher;
    
};



 
