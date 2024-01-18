#pragma once
#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <utility>
#include <new>
#include <assert.h>
#include <Windows.h>

#define min_stack_size 16 * 1024
#define max_stack_size 4 * 1024 * 1024
#define default_stack_size 1024 * 1024

class Allocator
{
private:
	size_t _size;
	size_t _pages;
	size_t GetPageSize();
public:
	Allocator(size_t size = default_stack_size) : _size(size) {};
	~Allocator() {};
	std::pair<char*, size_t> allocate();
	void deallocate(void*);
};

#endif // !ALLOCATOR_H
