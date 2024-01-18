#pragma once
#ifndef UTHREAD_H
#define UTHREAD_H

#include <iostream>
#include <unordered_map>
#include <queue>
#include <boost/context/fiber.hpp>

namespace ctx = boost::context;

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
	static void run_all();
private:
	int id;
	int priority;
	void (*function)();
	ThreadState state;
	ctx::fiber context;

	static ctx::fiber main;
	static ctx::fiber* anchor;
	static std::vector<TCB*> ready_queue;
	static std::unordered_map<int, TCB*> blocked_queue;
	static int schedule();
	static ctx::fiber task_proc(ctx::fiber&&);
public:
	TCB() {};
	~TCB() {};

	const int get_id() { return id; }

	int create(int, void (*function)());
	void exit();
	void join();
};

#endif // UTHREAD_H