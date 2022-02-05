#include"FixedSizeAllocator.h"
#include <new>
using namespace std;
namespace FSA {

	FixedSizeAllocator::FixedSizeAllocator(unsigned int i, size_t s, void* ptr, BitArray* bit) :
		new_size(i),
		nums(s),
		new_memory(reinterpret_cast<uint8_t*>(ptr)),
		new_bitArr(bit)
	{
		
	
	
	}

	FixedSizeAllocator* FixedSizeAllocator::Create(unsigned int i, size_t s, HeapAllocator* ha) 
	{
		size_t new_byte = i * s;

		void* memory = ha->malloc(new_byte,4);

		if (memory)
		{
			BitArray* new_bit = BitArray::Create(s, true,ha);
			if (new_bit)
			{
				FixedSizeAllocator* finalMem = reinterpret_cast<FixedSizeAllocator*>(memory);
				
				FixedSizeAllocator* new_alloc = sizeof(FixedSizeAllocator*) + finalMem;
				return new (finalMem) FixedSizeAllocator(i, s, new_alloc, new_bit);
			}

		
		
		}
	
		return nullptr;
	}
	bool FixedSizeAllocator::free(void* ptr) {
		
		size_t arr;
		if (ptr) {
		
			arr = (reinterpret_cast<uint8_t*>(ptr) - new_memory) / new_size;

			new_bitArr->ClearBit(arr);
		}
		return true;
	
	}

	void* FixedSizeAllocator::alloc(size_t s) {
		
		
		size_t arr;
		if (new_bitArr->GetFirstSetBit(arr)) {
		
			new_bitArr->ClearBit(arr);
			return arr * new_size + new_memory;
		
		}
		else {

			return nullptr;
		}
	
	
	}

	bool FixedSizeAllocator::contains(void* ptr) const
	{
		return (ptr >= new_memory) && (ptr < (new_memory + (new_size * nums)));
	}
}