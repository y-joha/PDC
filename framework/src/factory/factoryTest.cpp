/*********************************
 * Reviewer: Daniel gityamin				
 * Author: Yohai Azoulai			
 * File: factoryTest.cpp				
 * ******************************/

#include "factory.hpp" 
/*******Defines******/

#define NO_USE(A)  ((void)(A))

/*Excersice - */
#if 0

#endif

/*******Structs******/



/*******Service Funcs******/



/*******Funcs Funcs******/
#include "factory.hpp" 
#include <iostream>

namespace ilrd
{


class Shape 
{
public:
    Shape(int param) : m_h(param){}
    virtual ~Shape(){}
    virtual int getH(){return m_h;}
    virtual void SetH(int hight){m_h = hight;}
    virtual int Calculate() = 0;
private:
    int m_h;

};

class Square: public Shape
{
public:
    Square(int side);
    ~Square(){};
    int Calculate()
    {
        return getH() * getH();
    }


private:


};

Square::Square(int side):Shape(side)
{

}


class Circle :public Shape
{

public:
    Circle(int radius);
    ~Circle(){};

    int Calculate()
    {
        return getH() * getH() * 3.14;
    }

};

Circle::Circle(int radius) : Shape(radius)
{
}


} // namespace ilrd

using namespace ilrd;

int main()
{


    Factory<Shape,int,int> fact;

    fact.Add<Square>(1);
    fact.Add<Circle>(2);

    std::shared_ptr<Shape> s1 = fact.Create(1,5);
    std::shared_ptr<Shape> s2 = fact.Create(2,5);

    std::cout<<"Square area" << s1->Calculate()<< std::endl;
    std::cout<<"Circle area" << s2->Calculate()<< std::endl;
	

	return 0;
}
