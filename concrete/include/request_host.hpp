/*********************************
 * Reviewer: Unknown				
 * Author: Yohai Azoulai			
 * File: request_host.hpp					
 * ******************************/

#pragma once

#define UNUSED(expr) do { (void)(expr); } while (0)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define ROUND_DOWN(num) (((num) / (Info_t::FragmentLength)) * (Info_t::FragmentLength))

#include <set> /* set */
#include <memory> /* shared_ptr */
#include <sys/types.h>
#include <sys/socket.h>
#include <cassert>
#include <cstring>           /* memset */
#include <unordered_map>
#include "transmitter.hpp"
#include "factory.hpp"
#include "threadpool.hpp" /* ThreadPool , Task , Future*/
#include "nbd_comm.h" 

typedef struct Fragment freg_t;


class RequestHost
{

/* the handler gets the sp from create and put it in the map
 */
public:
    friend class RequestDispatcher;
    
    uint32_t GetId();
    uint32_t FragCount();
    void Distribute();
    static uint32_t ReadRplyHeader(int iot_fd);
    int SendReply(int iot_fd);

private:
    static std::shared_ptr<RequestHost> Create(std::vector<int> sockets);
    explicit RequestHost(std::vector<int>& sockets);

        void CalcWhere(freg_t *Fragment,  slice_t *Thrd_Strct);
    // for the sockets PER - IOT
    std::vector<int> m_sockets;
    // the Request recived from nbd
    NbdRequest * m_nbd_request;
    // the number of fragments left to recive from relevent request
    
    std::unordered_map<int,std::shared_ptr<Transmitter> > m_trsnmitors;

    //request uid
    uint32_t m_uid;
    // static runing counter to number requests

};


 
