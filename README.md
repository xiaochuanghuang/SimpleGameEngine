
Implement the FixedSizeAllocator class(css,.h) 
it will provides usable memory blocks of fixed size. 
And it has a return Null, not crash if it runs out of blocks like the Homework instruction. In FixedSizeAllocator class there
are 4 functions,
A constructor 
A create function for creating a HeapAllocator instance, then use that to allocate memory for the FixedSizeAllocators.
A alloc function that allocate the memory for FSA, coding is simular to the PPT. A free function that free the memory block for FSA.
A contains function for checking the pointers is coming up from the allocator.


Implement the BitArray class(css, .h). 
A class allowing us to create and access an array of n bits.
	•Where n is a really big number (much larger than 
	•Each bit represents a true / false state, instead of using a bool (8 bits). 
Similar to std::vector<bool> but with an interface (and optimizations) that supports our needs for the Fixed Size Allocator.
std::vector<> also not very good with fixed sized arrays.
	•push_back()
It can represent an arbitrary length array of bits using internally an array of a native data type optimal for the platform it's built for (32- or 64-bit).

Use the Sample BitArray API from the PPT and add it into the .h, then implement those functions in BitArray.css by following the instruction 
Finding the first set or clear bit.
A query for if a bit is set or not.
Setting or clearing a given bit.
Setting or clearing all the bits.

And I use the compiler intrinsics _BitScanForward() and _BitScanForward64() to help find the first set bit in a 32- or 64-bit value.


Implement function into the Allocator.cpp and MemorySystem.cpp

MemorySystem.cpp:
InitializeMemorySystem() : 
initialize 3 Fixed size allocators(0 - 16 bytes with 100 blocks in the FixedSizeAllocator.
16 - 32 bytes with 200 blocks in the FixedSizeAllocator.
33 - 96 bytes with 400 blocks in the FixedSizeAllocator.) 
and a HeapAllocator

collect() collect the fragments

delete() delete the FSA and HeapAllocator

FindFixedSizeAllocator() is finding the correct fixed size allocator


Allocator.cpp 
adding the code from PPT to new and delete for the unit test
