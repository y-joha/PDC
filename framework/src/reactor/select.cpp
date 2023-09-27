
#include <cstddef>

/* #include <sys/timerfd.h>
#include <sys/signalfd.h> */
#include "select.hpp"
#include "imonitor.hpp"

#define MAX(a,b) (((a)>(b))?(a):(b))



Select::Select() :  m_max_fd(0)
{
    FD_ZERO(&m_master);
    FD_ZERO(&m_work_set);
}

int Select::WaitForEvent()
{
    int jesus_christ = 0;
    m_work_set = m_master;
    jesus_christ = select(m_max_fd + 1,&m_work_set,NULL,NULL,NULL);
    return jesus_christ;
}

int Select::GetNextReadFd()
{
    for (int i = 0; i <= m_max_fd; ++i)
    {
        if(FD_ISSET(i, &m_work_set))
        {
            FD_CLR(i, &m_work_set);
            return i;
        }
    }
    return -1;
}

void Select::SetRead(int fd)
{
    m_max_fd = MAX(m_max_fd,fd);
    FD_SET(fd, &m_master);
    m_work_set = m_master;
}

void Select::ClearRead(int fd)
{
    FD_CLR(fd, &m_work_set);
}


