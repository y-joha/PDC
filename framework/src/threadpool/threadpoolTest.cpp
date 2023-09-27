#include <iostream>
#include "threadpool.hpp"

int Five(int arg)
{
    (void)arg;
    return 5;
}
int main()
{
    ThreadPool tp(2);

    int arg = 5;
    Function<int(void)> Func(Five,arg);
    ThreadPool::Future future = tp.Async(Func);
    std::cout << future.Wait() << std::endl;


    std::cout << "Now is the shutdown" << std::endl;
    tp.ShutDown();
    
}
