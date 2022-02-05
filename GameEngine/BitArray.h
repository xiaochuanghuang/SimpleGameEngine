#pragma once
using namespace std;
#include "HeapAllocator.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
using namespace HeapManagerProxy;
namespace FSA {
	class BitArray{

		#if _WIN32
		typedef uint32_t t_BitData;
		#else
		typedef uint64_t t_BitData;
		#endif // WIN32
		

	public:
	static BitArray* Create(size_t i_numBits, bool i_startClear, HeapAllocator* i_pAllocator);
	~BitArray();

	void ClearAll(void);
	void SetAll(void);

	bool AreAllBitsClear(void) const;
	bool AreAllBitsSet(void) const;

	inline bool IsBitSet(size_t i_bitNumber) const;
	inline bool IsBitClear(size_t i_bitNumber) const;

	void SetBit(size_t i_bitNumber);
	void ClearBit(size_t i_bitNumber);

	bool GetFirstClearBit(size_t& o_bitNumber) const;
	bool GetFirstSetBit(size_t& o_bitNumber) const;

	bool operator[](size_t i_index) const;

	BitArray(size_t i_numBits, bool i_bInitToZero);
	private:

		size_t element_nums;
		size_t total_bit;
		t_BitData* m_pBits;
		static const size_t bitsPerElement = sizeof(t_BitData) * 8;
	};

	
}