#include "MemorySystem.h"

bool InitializeMemorySystem(void * i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors)
{
	// create your HeapManager and FixedSizeAllocators
	ha = HeapManagerProxy::HeapAllocator::Create(i_pHeapMemory, i_sizeHeapMemory, i_OptionalNumDescriptors);
      
	new_fsa =
		FSA::FixedSizeAllocator::Create(16, 100, ha);
	
	new_fsa2 =
		FSA::FixedSizeAllocator::Create(32, 200, ha);
	

	new_fsa3 =
		FSA::FixedSizeAllocator::Create(96, 400, ha);
	
	FSAs.push_back(new_fsa);
	FSAs.push_back(new_fsa2);
	FSAs.push_back(new_fsa3);

	return true;
}

void Collect()
{
	// coalesce free blocks
	// you may or may not need to do this depending on how you've implemented your HeapManager

	ha->collect();
	
}

void DestroyMemorySystem()
{
	// Destroy your HeapManager and FixedSizeAllocators
	delete(ha);
	delete(new_fsa);
	delete(new_fsa2);
	delete(new_fsa3);

}

FSA::FixedSizeAllocator* FindFixedSizeAllocator(size_t s) {
	if (new_fsa) {
		if (new_fsa->new_size == s) {

			return new_fsa;

		}
	}
	 if (new_fsa2) {
		 if (new_fsa2->new_size == s) {
			return new_fsa2;
		}
	}
	 if (new_fsa3) {
	
		if (new_fsa3->new_size == s)
	{
		return new_fsa3;
	}
	}
	

		return nullptr;



}