/*********************************
 * Reviewer:  
 * Author: Yohai Azoulai
 * ******************************/

#include "singleton.hpp" 


class Widget
{
public:
	Widget():m_member(4) {}
	int m_member;
private:
};

int main(int argc, char *argv[])
{
	Singleton<Widget> w1;
	
	int a = w1->m_member;
	
	(void)argc;
	(void)argv;
	
	return 0;
}

