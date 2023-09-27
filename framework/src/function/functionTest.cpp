/*********************************
 * Reviewer:  
 * Author:             
 * File: function.hpp
 * ******************************/
#include <cstdio>
#include <cstring>
#include <iostream>
#include <ostream>

#include "function.hpp"


class number
{
    public:
        explicit number(int num): m_num(num) {}
        int add_print(int n)
        {
            m_num += n;
            printf("Member Function new num: %d\n", m_num);
            return m_num;
        };
    private:
        int m_num;
};


class Str
{
    public:
        explicit Str(){};
        int cpy_print(std::string str2)
        {
            m_str = str2;
            std::cout << m_str << std::endl;
            return 0;
        };
    private:
        std::string m_str;
};

class Number
{
    public:
        explicit Number(int num): m_number(num){std::cout << m_number << std::endl;};
        int Add_print(int newnum)
        {
            m_number = newnum;
            std::cout << m_number << std::endl;
            return 0;
        };
    private:
        int m_number;
};

int ADDPrintNum(int num)
{
    num +=2;
    printf("Free Function new num: %d\n", num);
    return 0;
}


int main()
{

    number num(6);

    Function<int(void)> f1(&number::add_print,&num,4);

    f1();

    Function<int(void)> f2(&ADDPrintNum,7);

    f2();
    

    Number number(4);
    
    Function<int(int)> fn(&Number::Add_print,&number);
    
    fn(5);

    Function<int(int)> fnc(&ADDPrintNum);

    fnc(6);


    Str sentence;

    Function<int(std::string)> fs1(&Str::cpy_print,&sentence);

    std::string s2 = "hello world";

    fs1(s2);

    return 0;
}
