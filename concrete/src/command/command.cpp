/*********************************
 * Author: Yohai Azoulai			
 * File: command.cpp					
 * ******************************/
 
 /*  */

#include <cstring>           /* memset */
#include "command.hpp"
/*******Defines******/



/*******Structs******/



/*******Service Funcs******/



/*******Funcs Funcs******/


void BuildBase(std::shared_ptr<slice_t> m_msgInfo);
/***************************************************************************/
Command::Command(std::shared_ptr<slice_t> msgInfo) : m_msgInfo(msgInfo)
{
    memset(&m_msgInfo->m_msghdr, 0 ,sizeof(m_msgInfo->m_msghdr));
}

int Command::Send()
{
    //printf("Command sending fregment\n");
    return (sendmsg(m_msgInfo->m_socket_fd, &m_msgInfo->m_msghdr, 0));
}

Write::Write(std::shared_ptr<slice_t> msgInfo) : Command(msgInfo){}

// changed this to be like buildbase
void Write::Build()
{

    BuildBase(m_msgInfo);
    PrintColor(BGreen);
    printf("Write\n\n");
    PrintColor(Color_Off);
    
    m_msgInfo->m_msghdr.msg_iov[1].iov_base = m_msgInfo->request->dataBuf +
                        m_msgInfo->m_header.m_len *
                        m_msgInfo->m_header.m_fragment_Indx;
    m_msgInfo->m_msghdr.msg_iov[1].iov_len = m_msgInfo->m_header.m_len;
    m_msgInfo->m_msghdr.msg_iovlen = 2;
    m_msgInfo->m_msghdr.msg_iov = m_msgInfo->payload;

}

Read::Read(std::shared_ptr<slice_t> msgInfo) : Command(msgInfo){}

void Read::Build()
{
    BuildBase(m_msgInfo);
    PrintColor(BRed);
    printf("Read\n\n");
    PrintColor(Color_Off);
}

void BuildBase(std::shared_ptr<slice_t> msgInfo)
{
    msgInfo->payload[0].iov_base = &msgInfo->m_header;
    msgInfo->payload[0].iov_len = sizeof(AtlasHeader);
    msgInfo->m_msghdr.msg_iovlen = 1; 
    msgInfo->m_msghdr.msg_iov = msgInfo->payload;
}


