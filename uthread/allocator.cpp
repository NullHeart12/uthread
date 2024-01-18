#include "allocator.h"

size_t Allocator::GetPageSize()
{
	SYSTEM_INFO page_size;
	GetSystemInfo(&page_size);
	return page_size.dwPageSize;
}

std::pair<char* ,size_t> Allocator::allocate()
{
	size_t page_size = GetPageSize();
	if (_size == 0)
		_size = default_stack_size;
	else if(_size < min_stack_size)
		_size = min_stack_size;
	else if (_size > max_stack_size)
		_size = max_stack_size;

	_pages = (size_t)floorl(float(_size) / float(page_size));
	assert(_pages > 2);
	
	_size = _pages * page_size;
	void* vp = VirtualAlloc(nullptr, _size, MEM_COMMIT, PAGE_READWRITE);
	if (!vp)
		throw std::bad_alloc();

	DWORD old_protect;
	VirtualProtect(vp, page_size, PAGE_READWRITE | PAGE_GUARD, &old_protect);

	char* sp = static_cast<char*>(vp) + _size;
	return { sp, _size };	
}

void Allocator::deallocate(void* vp)
{
	assert(vp);

	vp = static_cast<char*>(vp) - _size;
	VirtualFree(vp, 0, MEM_RELEASE);
}