#pragma once
#ifndef UTHREAD_H
#define UTHREAD_H

#include <iostream>
#include <unordered_map>
#include <queue>
#include "fcontext.h"
#include "allocator.h"

#define MAX_THREADS 10

enum ThreadState
{
	NEW,
	READY,
	RUNNING,
	BLOCKED,
	TERMINATED
};

class TCB
{
public:
	static int current_id;
	static std::unordered_map<int, TCB*> threads;

	static void yield();
	static void wait(int);
	static void exit();
	static void run_all();
private:
	int id;
	int priority;
	void (*function)();
	ThreadState state;
	fcontext_t context;
	void* sp;
	size_t stack_size;
	Allocator allocator;

	static fcontext_t main;
	static fcontext_t* anchor;
	static std::vector<TCB*> ready_queue;
	static std::unordered_map<int, TCB*> blocked_queue;
	static int schedule();
	static void task_proc(transfer_t);
public:
	TCB() {};
	~TCB() { allocator.deallocate(sp); }

	const int get_id() { return id; }

	int create(int, void (*function)(), size_t size = default_stack_size);
	void join();
};

#endif // UTHREAD_H