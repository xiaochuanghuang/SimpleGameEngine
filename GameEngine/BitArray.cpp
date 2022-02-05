#include "BitArray.h"
#include <new>
#include <iostream>
#include <intrin.h>

namespace FSA {


	BitArray::BitArray(size_t i_numBits, bool i_bInitToZero)
	{
		element_nums = (i_numBits + (bitsPerElement - 1)) / bitsPerElement;
		m_pBits = new t_BitData[i_numBits / bitsPerElement];
		total_bit = i_numBits;
		memset(m_pBits, i_bInitToZero ? 0 : 1, i_numBits / bitsPerElement);
	}

	BitArray* BitArray::Create(size_t i_numBits, bool i_startClear, HeapAllocator* i_pAllocator)
	{
		assert(i_numBits > 0);
		size_t elements = (i_numBits + (bitsPerElement - 1)) / bitsPerElement;
		size_t bytes = sizeof(t_BitData) * elements;


		void* new_memory = i_pAllocator->malloc(bytes,4);

		if (new_memory) {
			uint8_t* new_bitArray = reinterpret_cast<uint8_t*>(new_memory);

			return new (new_bitArray)BitArray(i_numBits, i_startClear);
		}
		else {
			return nullptr;
		}
	}

	void BitArray::ClearAll(void)
	{
		memset(m_pBits, 0, element_nums * sizeof(t_BitData));
	}
	void BitArray::SetAll(void)
	{
		memset(m_pBits, 1, element_nums * sizeof(t_BitData));
	}
	bool BitArray::AreAllBitsClear(void) const
	{

		size_t value;
		return !GetFirstSetBit(value);

	}

	bool BitArray::AreAllBitsSet(void) const {
		size_t value;
		return !GetFirstClearBit(value);
	}

	inline bool BitArray::IsBitSet(size_t i_bitNumber) const {
	
		size_t new_bit = i_bitNumber % bitsPerElement;
		size_t new_element = i_bitNumber / bitsPerElement;

		return (m_pBits[new_element] & (t_BitData(1) << new_bit)) ? true : false;
	}
	inline bool BitArray::IsBitClear(size_t i_bitNumber) const {
	
		size_t new_bit = i_bitNumber % bitsPerElement;
		size_t new_element = i_bitNumber / bitsPerElement;

		return (m_pBits[new_element] & (t_BitData(1) << new_bit)) ? false : true;
	}

	void BitArray::SetBit(size_t i_bitNumber) {
		size_t new_bit = i_bitNumber % bitsPerElement;
		size_t new_element = i_bitNumber / bitsPerElement;
		(m_pBits[new_element]) |= t_BitData(1) << new_bit;

	}
	void BitArray::ClearBit(size_t i_bitNumber) {
		size_t new_bit = i_bitNumber % bitsPerElement;
		size_t new_element = i_bitNumber / bitsPerElement;
		m_pBits[new_element] &= ~(t_BitData(1) << new_bit);
	}

	bool BitArray::GetFirstClearBit(size_t& o_bitNumber) const 
	{


		size_t new_element = 0;

		while (m_pBits[new_element] == ~t_BitData(0) && new_element < element_nums) {
			new_element++;

		}

		unsigned long new_bit = 0;
#if _WIN32
		unsigned char bitFound = _BitScanForward(&new_bit, ~m_pBits[new_element]);
#else 
		unsigned char bitFound = _BitScanForward64(&new_bit, ~m_pBits[new_element]);
#endif //_WIN32

		if (!bitFound) {

			return false;
		}
		o_bitNumber = new_element * bitsPerElement + new_bit;
		return true;



	}

	 bool BitArray::GetFirstSetBit(size_t& o_bitNumber) const {
	 
		 size_t new_element = 0;

		 while (m_pBits[new_element] == t_BitData(0) && new_element < element_nums) {
			 new_element++;
		 
		 }
		 
		 unsigned long new_bit = 0;
#if _WIN32
		 unsigned char bitFound = _BitScanForward(&new_bit, m_pBits[new_element]);
#else 
		 unsigned char bitFound = _BitScanForward64(&new_bit, m_pBits[new_element]);
#endif //_WIN32

		 if (!bitFound) {

			 return false;
		 }
		 o_bitNumber = new_element * bitsPerElement + new_bit;
		 return true;
	 
	 
	 }

	 bool BitArray::operator[](size_t i_index) const {
		 size_t new_bit = i_index % bitsPerElement;
		 size_t new_element = i_index / bitsPerElement;
		 return (m_pBits[new_element] >> new_bit) & t_BitData(1);

	 }


}