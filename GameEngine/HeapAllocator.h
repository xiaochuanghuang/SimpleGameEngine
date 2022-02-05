#pragma once
using namespace std;
#include <assert.h>
#include <inttypes.h>
namespace HeapManagerProxy {
	

	  struct MemoryBlock {
	  public:
		void* pBaseAddress;
		size_t BlockSize;
		struct MemoryBlock* pNextBlock;

	};


	class HeapAllocator {
		
	public:
	

		static HeapAllocator* Create(void* i_pHeapMemory, size_t i_HeapMemorySize, unsigned int i_NumMemBlocks);
		//static void* malloc(HeapAllocator*, size_t size, unsigned int alignment);
		void* malloc(size_t size, unsigned int alignment);
		void* malloc(size_t size);
		//void* malloc(size_t size);
		static void Destroy(HeapAllocator*);
		void ReturnMemoryBlock(MemoryBlock* i_pFreeBlock);
		void* allocateFromEnd(MemoryBlock* block, size_t size, unsigned int alignment);
		bool Contains(void* ptr);
		void ShowOutstandingAllocations();
		void ShowFreeBlocks();
		bool collect();
		bool free(void* pointer);
		bool IsAllocated(void* ptr);

	private:
		int fill_values_for_malloc = 0xff;
		uint8_t gbValue1 = 0xde;
		uint8_t gbValue2 = 0xad;
		uint8_t gbValue3 = 0xbe;
		uint8_t gbValue4 = 0xef;
		size_t gbSize = 4 * sizeof(gbValue1);
		HeapAllocator(void* i_pHeapMemory, size_t i_HeapMemorySize, unsigned int i_NumMemBlocks);

		MemoryBlock* InitializeMemoryBlocks(void* i_pBlocksMemory, size_t i_BlocksMemorySize);

		MemoryBlock* GetMemoryBlock();


		//bool removeIt()
		MemoryBlock* haveAllocated;
		MemoryBlock* startDes;
		unsigned int numberDes;
		struct MemoryBlock* pFreeMemBlocks;
		struct MemoryBlock* pFreeList;
		struct MemoryBlock* pOutstandingAllocations;
		uint8_t* canAllocate;
		size_t canAllocateSize;

		template<typename T>
		inline T* AlignDown(void* address, unsigned int alignment) {

			assert(alignment);
			assert(isPowerOfTwo(alignment));
			return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(address) & ~uintptr_t((alignment - 1)));

		}
		template<typename T>
		inline bool isPowerOfTwo(T value) {
			return value && !(value & (value - 1));
		}



	};

	bool Contains(HeapAllocator* hp, void* ptr);
	bool IsAllocated(HeapAllocator*, void*);
	bool free(HeapAllocator*, void*);
	void ShowOutstandingAllocations(HeapAllocator*);
	void ShowFreeBlocks(HeapAllocator*);
	bool collect(HeapAllocator*);
	void* malloc(HeapAllocator*, size_t);
	void* malloc(HeapAllocator*, size_t, unsigned int);
}
