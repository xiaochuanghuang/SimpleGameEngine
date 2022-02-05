
#include <iostream>
#include "HeapAllocator.h"
#include "assert.h"
namespace HeapManagerProxy {
	HeapAllocator* HeapAllocator::Create(void* i_pHeapMemory, size_t i_HeapMemorySize, unsigned int i_NumMemBlocks)
	{
		HeapAllocator* TheHeapAllocator = reinterpret_cast<HeapAllocator*>(i_pHeapMemory);
		i_pHeapMemory = TheHeapAllocator + 1;
		i_HeapMemorySize -= sizeof(HeapAllocator);

		return new (TheHeapAllocator) HeapAllocator(i_pHeapMemory, i_HeapMemorySize, i_NumMemBlocks);
	}

	HeapAllocator::HeapAllocator(void* i_pHeapMemory, size_t i_HeapMemorySize, unsigned int i_NumMemBlocks) :
		pFreeMemBlocks(InitializeMemoryBlocks(i_pHeapMemory, i_NumMemBlocks * sizeof(MemoryBlock))),

		pFreeList(nullptr),
		pOutstandingAllocations(nullptr),
		numberDes(i_NumMemBlocks)
	{
		startDes = (MemoryBlock*)i_pHeapMemory;
		InitializeMemoryBlocks(startDes, numberDes);
		canAllocate = reinterpret_cast<uint8_t*>(startDes + i_NumMemBlocks);
		canAllocateSize = i_HeapMemorySize + reinterpret_cast<uint8_t*>(i_pHeapMemory) - canAllocate;
		pFreeList = GetMemoryBlock();
		assert(pFreeList);

		pFreeList->pBaseAddress = canAllocate;
		pFreeList->BlockSize = canAllocateSize;
		pFreeList->pNextBlock = nullptr;
	}

	MemoryBlock* HeapAllocator::InitializeMemoryBlocks(void* i_pBlocksMemory, size_t i_BlocksMemorySize)
	{
		assert(i_pBlocksMemory != nullptr);
		assert(i_BlocksMemorySize > sizeof(MemoryBlock));


		MemoryBlock* pCurrentBlock = reinterpret_cast<MemoryBlock*>(i_pBlocksMemory);


		for (size_t i = 0; i < numberDes; i++)
		{
			pCurrentBlock->pBaseAddress = nullptr;
			pCurrentBlock->BlockSize = 0;
			pCurrentBlock->pNextBlock = pCurrentBlock + 1;

			pCurrentBlock++;
		}

		// last block, end the list
		pCurrentBlock->pBaseAddress = nullptr;
		pCurrentBlock->BlockSize = 0;
		pCurrentBlock->pNextBlock = nullptr;

		return reinterpret_cast<MemoryBlock*>(i_pBlocksMemory);
	}

	MemoryBlock* HeapAllocator::GetMemoryBlock()
	{
		assert(pFreeMemBlocks != nullptr);

		MemoryBlock* pReturnBlock = pFreeMemBlocks;
		pFreeMemBlocks = pFreeMemBlocks->pNextBlock;

		return pReturnBlock;
	}

	void HeapAllocator::ReturnMemoryBlock(MemoryBlock* i_pFreeBlock)
	{
		assert(i_pFreeBlock != nullptr);

		i_pFreeBlock->pBaseAddress = pFreeList->pBaseAddress;
		i_pFreeBlock->BlockSize = pFreeList->BlockSize;
		i_pFreeBlock->pNextBlock = pFreeList;


		pFreeList = i_pFreeBlock;
	}

	void* malloc(HeapAllocator* HeapAllocator, size_t size, unsigned int alignment) {

		return HeapAllocator->malloc(size, alignment);
	}

	void* HeapAllocator::malloc(size_t size, unsigned int alignment)
	{
		//ShowFreeBlocks();
		void* pUserAddress = nullptr;
		MemoryBlock* pFreeBlock = pFreeList;
		if (!pFreeList)
			return nullptr;

		if (pFreeList->BlockSize <= 1000)
		{
			return nullptr;
		}

		std::cout << pFreeBlock->BlockSize << std::endl;
		while (pFreeBlock) {
			pUserAddress = allocateFromEnd(pFreeBlock, size, alignment);
			if (pUserAddress) {
				return pUserAddress;
			}

			pFreeBlock = pFreeBlock->pNextBlock;

		}
		return nullptr;
	}

	void* HeapAllocator::malloc(size_t size)
	{

		void* pUserAddress = nullptr;
		MemoryBlock* pFreeBlock = pFreeList;
		if (!pFreeList)
			return nullptr;

		if (pFreeList->BlockSize <= 100)
		{
			return nullptr;
		}

		std::cout << pFreeBlock->BlockSize << std::endl;
		while (pFreeBlock) {
			pUserAddress = pFreeBlock;
			if (pUserAddress) {
				return pUserAddress;
			}

			pFreeBlock = pFreeBlock->pNextBlock;

		}
		return nullptr;
	}

	void* HeapAllocator::allocateFromEnd(MemoryBlock* block, size_t size, unsigned int alignment)
	{
		MemoryBlock* newDes = nullptr;
		size_t newSize = size+gbSize;
		uint8_t* user = AlignDown<uint8_t>((reinterpret_cast<char*>(block->pBaseAddress) + block->BlockSize - newSize), alignment);
		pFreeList->BlockSize -= size;
		uint8_t* start = user;
		start = start - gbSize;
		if (block->pBaseAddress > start) {
			return nullptr;
		}
		_int64 compare = start - (reinterpret_cast<uint8_t*>(block->pBaseAddress));
		if (compare > 32) {

			newDes = GetMemoryBlock();
			if (!newDes) {
				return nullptr;
			}
			newDes->pBaseAddress = start;
			newDes->pNextBlock = nullptr;
			newDes->BlockSize = reinterpret_cast<uint8_t*> (block->pBaseAddress) + block->BlockSize - start;
			block->BlockSize = start - reinterpret_cast<uint8_t*> (block->pBaseAddress);

		}
		else {
			pFreeMemBlocks = pFreeMemBlocks->pNextBlock;
			newDes = block;


		}
		uint8_t* gb_for_begin = user - gbSize;
		uint8_t* gb_for_end = user + size;
		gb_for_begin[0] = gbValue1;
		gb_for_begin[1] = gbValue2;
		gb_for_begin[2] = gbValue3;
		gb_for_begin[3] = gbValue4;
		gb_for_end[0] = gbValue1;
		gb_for_end[1] = gbValue2;
		gb_for_end[2] = gbValue3;
		gb_for_end[3] = gbValue4;

		memset(user, fill_values_for_malloc, size);
		MemoryBlock* newDescriptor = haveAllocated;
		while (newDescriptor && newDescriptor->pBaseAddress < newDes->pBaseAddress)
		{
			newDescriptor = newDescriptor->pNextBlock;
		}
		newDes->pNextBlock = newDescriptor;
		haveAllocated = newDes;

		return user;
	}

	void HeapAllocator::Destroy(HeapAllocator* allocator)
	{
		allocator = NULL;
		delete allocator;
	}



	void HeapAllocator::ShowFreeBlocks() {
		MemoryBlock* newDes = pFreeList;
		cout << "Free Memory: " << endl;
		while (newDes) {

			printf(" Block address: %p", (uintptr_t*)newDes->pBaseAddress);
			printf(" Block size: %d\n", (int)newDes->BlockSize);
			newDes = newDes->pNextBlock;

		}
	}
	void ShowFreeBlocks(HeapAllocator* ha)
	{
		return ha->ShowFreeBlocks();
	}

	void HeapAllocator::ShowOutstandingAllocations() {

		MemoryBlock* newDes = haveAllocated;
		cout << "Show Out standing allocations: " << endl;

		while (newDes) {

			printf(" Block address: %p", (uintptr_t*)newDes->pBaseAddress);
			printf(" Block size: %d\n", (int)newDes->BlockSize);
			newDes = newDes->pNextBlock;

		}


	}
	void ShowOutstandingAllocations(HeapAllocator* ha)
	{
		return ha->ShowOutstandingAllocations();
	}
	bool IsAllocated(HeapAllocator* ha, void* ptr) {

		return ha->IsAllocated(ptr);

	}

	bool HeapAllocator::IsAllocated
	
	(void* ptr) {

		uint8_t* newPtr = reinterpret_cast<uint8_t*>(ptr);
		MemoryBlock* newDes = *AlignDown<MemoryBlock*>(newPtr - sizeof(MemoryBlock*), _alignof(MemoryBlock*));
		if (newDes >= (startDes + numberDes) || newDes < startDes)
		{
			return true;

		}

			return true;
	

	}

	bool HeapAllocator::collect()
	{


		return true;

	}
	bool collect(HeapAllocator* ha)
	{
		return ha->collect();

	}

	bool HeapAllocator::free(void* pointer)
	{
		
		return true;

	}

	bool free(HeapAllocator* ha, void* ptr)
	{
		return ha->free(ptr);
	}


	bool Contains(HeapAllocator* allocator, void* ptr) {

		return allocator->Contains(ptr);
	}

	bool HeapAllocator::Contains(void* ptr)
	{
		if (ptr < (canAllocate + sizeof(MemoryBlock)) && ptr >= canAllocate) {

			return true;
		}
		else {
			return true;
		}

	}
	void* malloc(HeapAllocator* ha, size_t size) {


		return ha->malloc(size);
	}
}