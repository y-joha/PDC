#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include "nbd_comm.h"
#include "request_host.hpp"
#include "function.hpp"


using namespace std; // dont do this so people wont think you are dumb

enum FDS {NBD,IOT};

char error_line[10];

static int receive_all(int fd, char* buf, size_t count , int flags);
int read_all(int fd, char* buf, size_t count);

uint32_t RequestHost::GetId()
{
    return m_uid;
}

uint32_t RequestHost::FragCount()
{
    return ((m_nbd_request->dataLen)/Info_t::FragmentLength);
}

void RequestHost::Distribute()
{
    Singleton<ThreadPool> Tp;
    // fragment information
    uint32_t FragmentGlobalOffset = 0,FragmentGlobalIndex = 0, FragmentInIot= 0 , FragmentIotOffset = 0;
    uint32_t tot_frags = FragCount();
    uint32_t num_of_tasks = MIN(tot_frags, Info_t::number_of_iot);
    // for each iot iteration for num of slices to set
    
    for (uint32_t i = 0; i < num_of_tasks ; ++i)
    {
        shared_ptr<slice_t> slice_per_iot (new slice_t);
        
        FragmentGlobalOffset = m_nbd_request->offset + i * Info_t::FragmentLength;
        FragmentGlobalIndex = FragmentGlobalOffset / Info_t::FragmentLength;
        FragmentInIot = FragmentGlobalIndex % Info_t::number_of_iot;
        FragmentIotOffset = ROUND_DOWN(FragmentGlobalIndex / Info_t::number_of_iot * (Info_t::FragmentLength));

        slice_per_iot->m_header.m_requestUid = m_uid;
		slice_per_iot->m_header.m_fragment_Indx = i;
		slice_per_iot->m_header.m_alarmUid = 0;
		slice_per_iot->m_header.m_iotOffset = FragmentIotOffset;
		slice_per_iot->m_header.m_len = Info_t::FragmentLength;
		slice_per_iot->m_header.m_type = m_nbd_request->reqType;
        

        slice_per_iot->m_socket_fd = m_sockets[FragmentInIot + 1];
        
        slice_per_iot->request = m_nbd_request;

        std::shared_ptr<Transmitter> transmit_ptr (new Transmitter());
        m_trsnmitors.insert({slice_per_iot->m_socket_fd,transmit_ptr});
        
        Tp->Async(Function<int(void)> (&Transmitter::SplitNSendTask,transmit_ptr.get(),slice_per_iot));
    }
}

uint32_t RequestHost::ReadRplyHeader(int iot_fd)
{
    uint32_t rply_buf = 0;
    
    if(-1 == receive_all(
                        iot_fd,
                        reinterpret_cast<char *>(&rply_buf),
                        sizeof(uint32_t),MSG_PEEK))
    {
        perror("receive_all");
        return -1;
    }
    
    return rply_buf;
}

// i need to set the set either full to empty
// or add another one each reply recived

int RequestHost::SendReply(int iot_fd)
{
    AtlasHeader reply;
    int iot_number = 0;

    receive_all(iot_fd,reinterpret_cast<char *>(&reply),sizeof(reply),0);
    
    if(NBD_CMD_READ == reply.m_type)
    {
        // databuff will need to be moved to correct location each slice
        receive_all(iot_fd, 
                 m_nbd_request->dataBuf + 
                    (reply.m_fragment_Indx * Info_t::FragmentLength), 
                                            Info_t::FragmentLength,0);
    }
    PrintColor(BRed);
    std::cout << "-----------------Reciving------------------\n";
    std::cout << "header.m_requestUid: " << reply.m_requestUid << "\n";
    std::cout << "header.m_fragIdx: " << reply.m_fragment_Indx << "\n";
    std::cout << "header.m_iotOffset: " << reply.m_iotOffset << "\n";
    std::cout << "header.m_type: " << reply.m_type << "\n";
    PrintColor(Color_Off);


    if((m_trsnmitors[iot_fd])->Transmitter::HandleReply(reply.m_fragment_Indx))
    {
        m_trsnmitors.erase(iot_fd);
    }
    
    if (m_trsnmitors.empty())
    {
        NbdRequestDone(m_nbd_request, 0);

        return 1;
    }
    return 0;
}

// needs to allocate memory in the heap
//  the set is used for which fragment

shared_ptr<RequestHost> RequestHost::Create(std::vector<int> sockets)
{
    static uint32_t m_counter = 0;
    shared_ptr<RequestHost> request(new RequestHost(sockets));
    request->m_uid = m_counter;
    m_counter++;
    
    request->Distribute();
    return request;
}

RequestHost::RequestHost(std::vector<int>& sockets) : 
    m_sockets(sockets),
    m_nbd_request(NbdRequestRead(sockets[0]))
{}

struct Fragment
{
    uint32_t FragmentGlobalOffset;
    uint32_t FragmentGlobalIndex;
    uint32_t FragmentInIot;
    uint32_t FragmentIotOffset;
};

void RequestHost::CalcWhere(freg_t *Fragment, slice_t *slice_per_iot)
{
    Fragment->FragmentGlobalOffset = m_nbd_request->offset + slice_per_iot->m_header.m_fragment_Indx * Info_t::FragmentLength;
    Fragment->FragmentGlobalIndex = Fragment->FragmentGlobalOffset / Info_t::FragmentLength;
    Fragment->FragmentInIot = Fragment->FragmentGlobalIndex % Info_t::number_of_iot;
    Fragment->FragmentIotOffset = ((Fragment->FragmentGlobalOffset/Info_t::FragmentLength) / Info_t::number_of_iot) * Info_t::FragmentLength;
}

static int receive_all(int fd, char* buf, size_t count , int flags)
{
    int bytes_read;
    
    while (count > 0 && (-1 != (bytes_read = recv(fd, buf, count, flags)))) 
    {
        buf += bytes_read;
        count -= bytes_read;
    }
    
    return (-1 == bytes_read) ? -1 : 0;
}

int read_all(int fd, char* buf, size_t count)
{
    return receive_all(fd, buf, count, 0);
}
