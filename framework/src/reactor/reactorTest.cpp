/*********************************
 * Author: Yohai Azoulai			
 * File: reactorTest.cpp				
 * ******************************/

#include <iostream>
#include "reactor.hpp" 
#include "select.hpp"
#include "nbd_comm.h"



/*******Defines******/

#define NO_USE(A)  ((void)(A))

/*Excersice - */
#if 0

#endif
/*******Structs******/



/*******Service Funcs******/

int FunkyFunc(int x)
{
    char y;
    std::cin >> y;
    std::cout << "you will not belive what happend" << y << x << std::endl;
    return 0;
}

/*******Funcs Funcs******/

int main(int argc, char *argv[])
{
    Reactor<Select> R1;
    
    R1.RegisterReadHandler(0,Function<int(void)>(FunkyFunc,6022));
    R1.Run();
    
    NO_USE(argc);
    NO_USE(argv);
    
    return 0;
}


