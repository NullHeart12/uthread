#include "uthread.h"

int TCB::current_id = -1;
fcontext_t TCB::main = nullptr;
fcontext_t* TCB::anchor = &main;
std::unordered_map<int, TCB*> TCB::threads = {};
std::vector<TCB*> TCB::ready_queue = {};
std::unordered_map<int, TCB*> TCB::blocked_queue = {};

//**************private**************//

void TCB::task_proc(transfer_t previous)
{
	TCB* current = threads[current_id];
	std::swap(*anchor, previous.fctx);
	anchor = &current->context;

	current->function();

	current->exit();
	jump_fcontext(main, nullptr);
}

int TCB::schedule()
{
	if (current_id != -1)
	{
		TCB* current = threads[current_id];
		current->state = READY;
		ready_queue.push_back(current);
		return -1;
	}
	sort(ready_queue.begin(), ready_queue.end(), [](TCB* a, TCB* b)
		{ return a->priority < b->priority; });
	TCB* pick = ready_queue.back();

	for(auto it = ready_queue.begin(); it != ready_queue.end(); ++it)
		(*it)->priority++;
	for(auto it = blocked_queue.begin(); it != blocked_queue.end(); ++it)
		(it->second)->priority++;

	TCB::current_id = pick->id;
	int ret = pick->id;
	pick->state = RUNNING;
	pick->priority -= 3;
	anchor = &main;
	*anchor = jump_fcontext(pick->context, nullptr).fctx;
	TCB::current_id = -1;

	return ret;
}


//**************public**************//

int TCB::create(int prio, void(*f)(), size_t size)
{
	id = threads.size();
	if(id == MAX_THREADS)
		return -1;

	if (threads.find(id) != threads.end())
		return -1;
	threads.insert({ id, this });

	priority = prio;
	state = NEW;
	function = f;
	stack_size = size;
	allocator = Allocator(stack_size);
	std::pair<char*, size_t> stack = allocator.allocate();
	sp = reinterpret_cast<void*>(stack.first);
	context = make_fcontext(stack.first, stack.second, task_proc);
	
	return id;
}

void TCB::join()
{
	if (state != NEW)
		return;

	state = READY;
	ready_queue.push_back(this);
}

void TCB::yield()
{
	int id = TCB::current_id;
	TCB* current = threads[id];
	anchor = &current->context;
	current_id = -1;

	*anchor = jump_fcontext(main, nullptr).fctx;
}

void TCB::exit()
{
	TCB* current = threads[current_id];
	current->state = TERMINATED;
	threads.erase(current_id);
	ready_queue.erase(std::find(ready_queue.begin(), ready_queue.end(), current));
	current_id = -1;
}

void TCB::wait(int tid)
{
	if (threads.find(tid) == threads.end())
	{
		printf("thread pid=%d does not exist\n", tid);
		return;
	}
	TCB* current = threads[current_id];
	current->state = BLOCKED;
	ready_queue.erase(std::find(ready_queue.begin(), ready_queue.end(), current));
	blocked_queue.insert({ tid, current });

	printf("thread pid=%d was blocked, waiting for thread pid=%d\n", current_id, tid);

	yield();
}

void TCB::run_all()
{
	while (!threads.empty())
	{
		if (!threads.empty() && ready_queue.empty())
		{
			std::cerr << "Deadlock detected!\n";
			return;
		}
		while (!ready_queue.empty())
		{
			int sch_id = schedule();
			if (TCB::blocked_queue.find(sch_id) != TCB::blocked_queue.end())
			{
				TCB* blocked = TCB::blocked_queue[sch_id];
				TCB::blocked_queue.erase(sch_id);
				blocked->state = READY;
				ready_queue.push_back(blocked);
			}
		}
	}
}

