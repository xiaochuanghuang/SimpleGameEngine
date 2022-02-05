#pragma once
#include"FixedSizeAllocator.h"
#include"HeapAllocator.h"
#include <vector>
static FSA::FixedSizeAllocator* new_fsa;
static FSA::FixedSizeAllocator* new_fsa2;
static FSA::FixedSizeAllocator* new_fsa3;
static vector<FSA::FixedSizeAllocator*> FSAs;

static HeapManagerProxy::HeapAllocator* ha;
// InitializeMemorySystem - initialize your memory system including your HeapManager and some FixedSizeAllocators
bool InitializeMemorySystem(void * i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors);

// Collect - coalesce free blocks in attempt to create larger blocks
void Collect();

// DestroyMemorySystem - destroy your memory systems
void DestroyMemorySystem();

FSA::FixedSizeAllocator* FindFixedSizeAllocator(size_t);