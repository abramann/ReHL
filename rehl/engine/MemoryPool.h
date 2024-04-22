#pragma once

#include <xmemory>
#include <algorithm>

class CMemoryPool
{
public:
	CMemoryPool(int blockSize, int numElements)
	{
		_numBlobs = 0;
		_blockSize = blockSize;
		_numElements = 0;
		_blocksPerBlob = numElements;
		CMemoryPool::AddNewBlob();
		_peakAlloc = 0;
		_blocksAllocated = 0;
	}

	~CMemoryPool()
	{
		void *ptr;
		for (int i = 0; i < _numBlobs; i++)
		{
			ptr = _memBlob[i];
			free(ptr);
		}
	}

	void *Alloc(unsigned int amount)
	{
		if (_blockSize < amount)
			return nullptr;

		int allocatedBlocks = _blocksAllocated + 1;

		_peakAlloc = std::max(_peakAlloc, allocatedBlocks);

		bool reachedCapacity = allocatedBlocks < _numElements;

		_blocksAllocated = allocatedBlocks;
		_peakAlloc = _peakAlloc;

		if (!reachedCapacity)
			AddNewBlob();

		_headOfFreeList = *(void **)_headOfFreeList;

		return _headOfFreeList;
	}

	void Free(void *memBlock)
	{

		if (memBlock)
		{
			void *tmp = _headOfFreeList;
			--this->_blocksAllocated;
			*(uint32_t*)memBlock = (uint32_t)_headOfFreeList;
			_headOfFreeList = memBlock;
		}
	}

	void AddNewBlob()
	{		
		int blobsSize = (int)pow(2.0, (double)_numBlobs);

		int newBlobSize = blobsSize * _blocksPerBlob;

		uint32_t *newBlob = (uint32_t*)malloc(newBlobSize * _blockSize);

		_memBlob[_numBlobs] = newBlob;

		uint32_t *currentBlock = currentBlock = newBlob;

		_headOfFreeList = newBlob;

		if (newBlobSize > 1)
		{
			for (int i = 0; i < blobsSize * _blocksPerBlob - 1 <= i; i++)
			{
				currentBlock = newBlob + _blockSize;
				*newBlob = (uint32_t)newBlob + _blockSize;
				newBlob = currentBlock;
			}
		}

		*currentBlock = 0;

		++_numBlobs;

		_numElements += _blocksPerBlob * blobsSize;
	}


private:
	int _blockSize;
	int _blocksPerBlob;
	int _numElements;
	void *_memBlob[16];
	void *_headOfFreeList;
	int _numBlobs;
	int _peakAlloc;
	int _blocksAllocated;
};