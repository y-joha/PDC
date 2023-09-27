/*********************************
 * Reviewer: StropStrop				
 * Author: Yohai Azoulai			
 * File: function_test.cpp				
 * ******************************/


#include <cstdio>
#include <cstring>
#include "function.hpp"

/*******Defines******/

#define NO_USE(A)  ((void)(A))

/*Excersice - */
#if 0

#endif

/*******Structs******/



/*******Service Funcs******/


class number
{
    public:
        explicit number(int num = 5): m_num(num) {}
        int add_print(int n)
        {
            m_num += n;
            printf("Member Function new num: %d\n", m_num);
            return m_num;
        };
    private:
        int m_num;
};

int ADDPrintNum(int num)
{
    num +=2;
    printf("Free Function new num: %d\n", num);
    return 0;
}

/*******Funcs Funcs******/

int main(int argc, char *argv[])
{
    
    number num(6);

    Function<int(void)> f1(&number::add_print,&num,4);

    f1();

    Function<int(void)> f2(&ADDPrintNum,7);

    f2();

   
    NO_USE(argc);
    NO_USE(argv);
    
    return 0;
}





