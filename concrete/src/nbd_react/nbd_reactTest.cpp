/*********************************
 * Author: Yohai Azoulai			
 * File: nbd_reactTest.cpp				
 * ******************************/


#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cassert>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include "nbd_comm.h"
#include "function.hpp"
#include "reactor.hpp"
#include "imonitor.hpp"
#include "select.hpp"
#include "threadpool.hpp"
#include "transmitter.hpp"
#include "singleton.hpp"
#include "request_host.hpp"
#include "dispatcher.hpp"




#include "nbd_comm.h"


/*******Defines******/

#define STRUCTADDRESS struct sockaddr
#define MAXLINE 1000
#define PORT_NUM 29000
#define HOME_ADDR "127.0.0.1"
#define BUFFSIZE 1024
#define STORAGESIZE  0x10000000
#define NO_USE(A)  ((void)(A))



static uint64_t initial_port = 29000;
/*Excersice - */
#if 0

#endif

/*******Structs******/


/*******Service Funcs******/

int TCPClientConnect(uint16_t port_num);

/*******Funcs Funcs******/
/**
 * read & write classes are there in order to
 * prep the message either to send or recieve
 * class command will be Abstruct
 * virtual Build = 0 (pure virtual) & virtual Send --- methods
 * build builds the msg struct (iovec and etc the msg header)
 * 
 * 
*/

int main(int argc, char *argv[])
{
    
    if(argc < 2)
    {
        printf("add an nbd number if all busy\n write dis in terminal to run script\n\n");
        exit(EXIT_FAILURE);
    }
    
    Reactor<Select> R1;
    RequestDispatcher D1;
    std::vector<int> all_fds(Info_t::number_of_iot + 1);
    // nbdsocket is nbd to daemon communication
    char dev_bd[10] = "/dev/nbd";
    strcat(dev_bd,argv[1]);
    if(-1 == (all_fds[0] = NbdDevOpen(dev_bd, STORAGESIZE)))
    {
        perror("NbdDevOpen");
        return -1;
    }
    // create a vector of socket after they have been connected

    
    // shani tip : change handler to recive array of ints so you can pass both
    // iot and the nbdsocket
    // nbd has to be 0 for reliability
    std::cout<< "This is the nbd fd " << all_fds[0] << std::endl;


    // create TCP socket to listen to port = 29k
    // here put loop to connect to servers
    // each server connects and passed fd 
    for (uint32_t i = 1; i < Info_t::number_of_iot + 1; ++i)
    {
        std::cout<< "This port is " << initial_port << std::endl;
        all_fds[i] = TCPClientConnect(initial_port++);
        R1.RegisterReadHandler(all_fds[i],Function<int(void)>(&RequestDispatcher::IotRequestHandler,&D1,all_fds[i]));
        std::cout<< "This is the Iot fd " << all_fds[i] << std::endl;
    }
    R1.RegisterReadHandler(all_fds[0], Function<int(void)>(&RequestDispatcher::NbdRequestHandler,&D1,all_fds));
    //ConnectTCPSocket(g_iot_socket...);
    // now i have connected daemon to both ends
    PrintColor(Green);
    std::cout<< "Run Forest Run" << std::endl;
    PrintColor(Color_Off);

    R1.Run();

    

    NO_USE(argc);
    NO_USE(argv);
    
    return 0;
}


// currently hard coded
// next section 
int TCPClientConnect(uint16_t port_num)
{
    int socket_fd = 0;
    struct sockaddr_in iot_server_info;
    int addr_len = sizeof(struct sockaddr_in);
    //created socket
    if(-1 == (socket_fd = socket(AF_INET, SOCK_STREAM, 0))){perror("socket connecttion\n"); return -1;}
    // configured socket
    iot_server_info.sin_family = AF_INET;
    iot_server_info.sin_port = htons(port_num);
    iot_server_info.sin_addr.s_addr = INADDR_ANY;

    if(-1 == (connect(socket_fd, reinterpret_cast<struct sockaddr *>(&iot_server_info), addr_len))){perror("connect TCP failed!");};

    PrintColor(IGreen);
    printf("socket_fd: %d\n", socket_fd);
    PrintColor(Color_Off);
    return socket_fd;
}

