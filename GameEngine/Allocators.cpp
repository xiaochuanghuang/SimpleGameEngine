#include <inttypes.h>
#include <malloc.h>

#include <stdio.h>
#include "FixedSizeAllocator.h"
#include "MemorySystem.h"
#include <vector>

//void * __cdecl malloc(size_t i_size)
//{
//	// replace with calls to your HeapManager or FixedSizeAllocators
//	printf("malloc %zu\n", i_size);
//	void* pReturn = nullptr;
//	
//	FSA::FixedSizeAllocator* pFSA = FindFixedSizeAllocator(i_size);
//
//	if (pFSA)
//		pReturn = pFSA->alloc(i_size);
//	if (pReturn == nullptr)
//		pReturn = ha->malloc(i_size,4);
//	return pReturn;
//}
//
//void __cdecl free(void * i_ptr)
//{
//	// replace with calls to your HeapManager or FixedSizeAllocators
//	printf("free 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
//	const size_t numFSAs = FSAs.size();
//	for (size_t i = 0; i < numFSAs; i++)
//	{
//		if (FSAs[i]->contains(i_ptr))
//		{
//			FSAs[i]->free(i_ptr);
//			return;
//		}
//	}
//	assert(ha->Contains(i_ptr));
//	ha->free(i_ptr);
//}

void * operator new(size_t i_size)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("new %zu\n", i_size);
	void* pReturn = nullptr;

	FSA::FixedSizeAllocator* pFSA = FindFixedSizeAllocator(i_size);

	if (pFSA)
		pReturn = pFSA->alloc(i_size);
	if (pReturn == nullptr) {
		if (ha) {
			pReturn = ha->malloc(i_size, 4);
		}
		else {

			return std::malloc(i_size);

		}
	}
	return pReturn;
}

void operator delete(void * i_ptr)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("delete 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
	const size_t numFSAs = FSAs.size();
	for (size_t i = 0; i < numFSAs; i++)
	{
		if (FSAs[i]->contains(i_ptr))
		{
			FSAs[i]->free(i_ptr);
			return;
		}
	}
	
	ha->free(i_ptr);
}

void * operator new[](size_t i_size)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("new [] %zu\n", i_size);
	void* pReturn = nullptr;

	FSA::FixedSizeAllocator* pFSA = FindFixedSizeAllocator(i_size);

	if (pFSA)
		pReturn = pFSA->alloc(i_size);
	if (pReturn == nullptr) {
		if (ha) {
			pReturn = ha->malloc(i_size, 4);
		}
		else {

			return std::malloc(i_size);

		}
	}
	return pReturn;
}

void operator delete [](void * i_ptr)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	printf("delete [] 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
	const size_t numFSAs = FSAs.size();
	for (size_t i = 0; i < numFSAs; i++)
	{
		if (FSAs[i]->contains(i_ptr))
		{
			FSAs[i]->free(i_ptr);
			return;
		}
	}
	
	ha->free(i_ptr);
}