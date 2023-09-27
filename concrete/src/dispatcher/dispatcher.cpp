
#include "factory.hpp"
#include "singleton.hpp"
#include "nbd_comm.h"
#include "command.hpp"
#include "dispatcher.hpp"

int ilrd::RequestDispatcher::NbdRequestHandler(std::vector<int> sockets)
{
    // add to factory the method to create an object
    Singleton<Factory < ilrd::Command,uint32_t,std::shared_ptr < slice_t > > > factory;

    factory->Add<Write>(NBD_CMD_WRITE);
    factory->Add<Read>(NBD_CMD_READ);

    std::shared_ptr<RequestHost> req(RequestHost::Create(sockets));

    // UID of the request is just a running number
    m_activeRequest.insert({req->GetId(),req});
    return 0;
}

int ilrd::RequestDispatcher::IotRequestHandler(int iot_fd)
{
    uint32_t uid = RequestHost::ReadRplyHeader(iot_fd);
    if(static_cast<uint32_t>(-1) == uid ){perror("peek:");}

    if(m_activeRequest[uid]->SendReply(iot_fd))
    {
        m_activeRequest.erase(uid);
    }

    return 0;
}
