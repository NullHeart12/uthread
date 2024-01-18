#pragma once
#ifndef FIBER_H
#define FIBER_H

#include <algorithm>
#include "fcontext.h"
#include "allocator.h"

#define min_stack_size 16 * 1024
#define max_stack_size 4 * 1024 * 1024
#define default_stack_size 1024 * 1024

namespace ctx
{

class fiber
{
private:
	size_t stack_size;
	Allocator allocator;
	fcontext_t context;
	fiber func(fiber&&);

	fiber(fcontext_t context) noexcept : context(context) {}
	static void fiber_entry(transfer_t context) noexcept;
public:
	fiber()  noexcept :stack_size(default_stack_size) {};
	fiber(fiber fn(fiber&&));
	

	void swap(fiber& other) noexcept { std::swap(context, other.context); }
};

}
#endif // !FIBER_H
