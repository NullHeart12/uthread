#include <iostream>
#include "uthread.h"

void func1()
{
	for (int i = 0; i < 10; i++)
	{
		printf("enter thread pid=%d; times=%d\n", TCB::current_id, i);
		TCB::yield();
	}
	printf("exit thread pid=%d\n", TCB::current_id);
	return;
}

void func2()
{
	for(int i = 0; i < 10; i++)
	{
		printf("enter thread pid=%d; times=%d\n", TCB::current_id, i);
		TCB::yield();
	}
	TCB::wait(0);
	printf("exit thread pid=%d\n", TCB::current_id);
	return;
}

void func3()
{
	for (int i = 0; i < 10; i++)
	{
		printf("enter thread pid=%d; times=%d\n", TCB::current_id, i);
		TCB::yield();
	}
	TCB::wait(1);
	printf("exit thread pid=%d\n", TCB::current_id);
	return;
}

void func4()
{
	for (int i = 0; i < 10; i++)
	{
		printf("enter thread pid=%d; times=%d\n", TCB::current_id, i);
		TCB::yield();
	}
	TCB::wait(2);
	printf("exit thread pid=%d\n", TCB::current_id);
	return;
}

int main()
{
	TCB thread1;
	TCB thread2;
	TCB thread3;
	TCB thread4;
	thread1.create(150, func1);
	thread2.create(146, func2);
	thread3.create(152, func3);
	thread4.create(144, func4);
	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
	TCB::run_all();
	return 0;
}