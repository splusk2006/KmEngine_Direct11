#pragma once

// When allocating memory in heap,
// Visual studio "new/delete" allocates memory with 8byte align.
// This class ables 16byte align
template<size_t Alignment>
class AlignedAllocationPolicy
{
public:
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, Alignment);
	}

	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}
};