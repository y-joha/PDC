
#include <iostream>
#include "observer.hpp"



#define NUM_SUBS 10
using EventInfo = const char*;
using EventInfoRef = const char*&;

/*___________________ Publisher and Subscriber Definitions ___________________*/

class Publisher
{
public:
	void Subscribe(const Callback<EventInfoRef>& sub);
	void UnSubscribe(Callback<EventInfoRef>& sub);
	void NotifyAll(EventInfo ev);

private:
	Dispatcher<EventInfoRef> m_dispacher;

}; // class Publisher

class Subscriber
{
public:
	Subscriber();
	void OnUpdate(EventInfoRef ev);
	
	Callback<EventInfoRef> m_callback;

private:
	int m_id;

}; // class Subscriber

/*___________________________________ Main ___________________________________*/

int main()
{
	Publisher pub;
	Subscriber subs[NUM_SUBS];

	for(int i = 0; i < NUM_SUBS; ++i)
	{
		pub.Subscribe(subs[i].m_callback);
	}

	pub.NotifyAll("Hello");


	return 0;
}

/*_____________________________ class Publisher ______________________________*/

void Publisher::Subscribe(const Callback<EventInfoRef> &sub)
{
	m_dispacher.Add(sub);
}

void Publisher::UnSubscribe(Callback<EventInfoRef> &sub)
{
	m_dispacher.Remove(sub);
}

void Publisher::NotifyAll(EventInfo ev)
{
	m_dispacher.Notify(ev);
}

/*_____________________________ class Subscriber _____________________________*/

Subscriber::Subscriber(): m_callback(Function<void(EventInfoRef)>(&Subscriber::OnUpdate, this))
{
	static int id = 0;
	m_id = id++;
}

void Subscriber::OnUpdate(EventInfoRef ev)
{
	std::cout << "Subscriber number " << m_id << " Received " << ev << std::endl;
}

