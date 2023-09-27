/*********************************
 * Reviewer: Who knows				
 * Author: Yohai Azoulai			
 * File: semaphoreTest.cpp				
 * ******************************/

#include <thread>
#include <unistd.h>
#include <iostream>
#include "semaphore.hpp" 
/*******Defines******/

#define NO_USE(A)  ((void)(A))

/*Excersice - */
#if 0

#endif

/*******Structs******/


static Semaphore sam_i_am(2);


/*******Service Funcs******/

void Thread_wait();

/*******Funcs Funcs******/

int main(int argc, char *argv[])
{
    std::thread z_the_thread(Thread_wait);
    std::thread zod_the_thread(Thread_wait);
    
    while (1)
    {
        sleep(2);
        sam_i_am.Post();
    }
    
    NO_USE(argc);
    NO_USE(argv);
    
    return 0;
}


void Thread_wait()
{
    while (1)
    {
        std::cout << std::this_thread::get_id() << std::endl;
        usleep(100);
        sam_i_am.Wait();
    }
}
