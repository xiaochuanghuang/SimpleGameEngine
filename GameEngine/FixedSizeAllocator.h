#pragma once
#include "BitArray.h"
namespace FSA {
	class FixedSizeAllocator{
	public:
		bool free(void*);
		void* alloc(size_t);
		static  FixedSizeAllocator* Create(unsigned int , size_t, HeapManagerProxy::HeapAllocator*);
		bool contains(void*) const;
		unsigned int new_size;
	private:
		FixedSizeAllocator(unsigned int , size_t , void* , BitArray*);
		
		size_t nums;
		BitArray* new_bitArr;
		uint8_t* new_memory;
	};
}