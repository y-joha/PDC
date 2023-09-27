
#pragma once



#include <stdint.h>
#include <linux/nbd.h>
#include <sys/types.h>
#include <sys/socket.h>


#ifdef __cplusplus

typedef struct StaticInfo
{
    static const uint32_t number_of_iot = 5;
    static const uint32_t FragmentLength = 1024;
}Info_t;

extern "C" {

#endif


typedef struct nbd
{
    uint32_t reqType;
    uint32_t dataLen;
    uint64_t offset;
    char *dataBuf;
}NbdRequest;

typedef struct AtlasHeader
{
    uint32_t    m_requestUid;
    uint32_t    m_fragment_Indx;
    uint32_t    m_alarmUid;
    uint32_t    m_iotOffset;
    uint32_t    m_type;
    uint32_t    m_len;
}AtlasHeader;


typedef struct SlicePerIOT
{
    AtlasHeader m_header;
    int m_socket_fd;
    NbdRequest *request;
    struct msghdr m_msghdr;
    struct iovec payload[2];
}slice_t;



int NbdDevOpen(const char* dev_name, uint64_t size);

NbdRequest *NbdRequestRead(int sock);

void NbdRequestDone(NbdRequest* req, int error);




#ifdef __cplusplus
}

#endif
