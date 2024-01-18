#pragma once
#ifndef FCONTEXT_H
#define FCONTEXT_H

typedef void* fcontext_t;

typedef struct
{
	fcontext_t fctx;
	void* data;
}transfer_t;

extern "C"
fcontext_t make_fcontext(void* sp, size_t size, void(*fn)(transfer_t));
extern "C"
transfer_t jump_fcontext(fcontext_t const to, void* vp);

#endif // !FCONTEXT_H
