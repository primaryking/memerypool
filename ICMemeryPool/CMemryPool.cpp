#include "stdafx.h"
#include "CMemryPool.h"

CMemeryBlock::CMemeryBlock(unsigned int memerysize, unsigned int unitSize)
    : m_MemeryPtr(nullptr)
    , m_MemerySize(memerysize)
    , m_UnitSize(unitSize)
    , m_CurrentPos(0)
{
    InitBlock();

}

CMemeryBlock::~CMemeryBlock()
{
    if(m_MemeryPtr)
    {
        delete m_MemeryPtr;
        m_MemeryPtr = nullptr;
        MemeryUnit* pUinit =  m_MemeryUnitVec.at(0);
        delete[] pUinit;
    }
}

void *CMemeryBlock::GetMemery(unsigned int size)
{
    if(m_MemeryUnitVec[m_CurrentPos]->dataSize >= size && m_CurrentPos != GetLastPos())
    {
        int usize = CalSize(size);
        void* buffer =  m_MemeryUnitVec[m_CurrentPos]->pData;
        m_CurrentPos += usize;
		m_UsedBlockSizeMap.insert(std::make_pair(buffer,size));
        return  buffer;
    }
    return nullptr;
}

bool CMemeryBlock::IsFree()
{
    return m_CurrentPos == 0;
}

void CMemeryBlock::InitBlock()
{
    int usize = CalSize(m_MemerySize);
   MemeryUnit* pUinit = new MemeryUnit[usize];

   m_MemeryPtr = new unsigned char [m_MemerySize];
   unsigned char* pheader = m_MemeryPtr;

   for(int i =0 ; i < usize; i ++)
   {
       pUinit[i].pData = pheader + i*m_UnitSize;
       pUinit[i].dataSize = m_MemerySize - i*m_UnitSize;
       m_MemeryUnitVec.push_back(&pUinit[i]);
   }
}

void CMemeryBlock::FreeMemery( void* mem )
{
	unsigned int memsize = 0;
	auto itor = m_UsedBlockSizeMap.find(mem);
	if (itor != m_UsedBlockSizeMap.end())
	{
		m_CurrentPos -= CalSize(itor->second);
		m_UsedBlockSizeMap.erase(itor);
	}
}

int CMemeryBlock::CalSize( unsigned int size )
{
	int usize = size / m_UnitSize;
	if(size % m_UnitSize)
	{
		usize ++;
	}
	return usize;
}

CMemeryPool::CMemeryPool()
{
	m_Lock = new CSingleLock(&m_Mutex);
    CMemeryBlock *mblock = new CMemeryBlock(MIN_MEMERY_SIZE , 1024*5);
	m_Pool.push_back(mblock);

}

CMemeryPool::~CMemeryPool()
{
	m_Pool.clear();
}

void* CMemeryPool::GetMemery(int size)
{
	if (size < 0)
	{
		return nullptr;
	}
	m_Lock->Lock();
	int poolsize = m_Pool.size();
	for (int i = 0; i < poolsize;i ++)
	{
		void* rltmemery = m_Pool[i]->GetMemery(size);
		if (rltmemery != NULL)
		{
			m_UsedMemeryBlockMap.insert(std::make_pair(rltmemery,i));

			memset(rltmemery,0,size*sizeof(unsigned char));
			m_Lock->Unlock();
			return rltmemery;
		}
	}
	if (size < MIN_MEMERY_SIZE)//ÐÂµÄÄÚ´æ 
	{
		size = MIN_MEMERY_SIZE;
	}
	CMemeryBlock *mblock = new CMemeryBlock(size , 1024*5);
	m_Pool.push_back(mblock);
	void* rltmem =  mblock->GetMemery(size);
	memset(rltmem,0,size*sizeof(unsigned char));
	m_UsedMemeryBlockMap.insert(std::make_pair(rltmem,poolsize));
	m_Lock->Unlock();
	return rltmem;
}

void CMemeryPool::FreeMemery( void* mem )
{
	if (mem == NULL)
	{
		return;
	}
	m_Lock->Lock();
	auto fitor = m_UsedMemeryBlockMap.find(mem);
	if (fitor != m_UsedMemeryBlockMap.end() && m_Pool.size() > fitor->second)
	{
		m_Pool[fitor->second]->FreeMemery(mem);
		m_UsedMemeryBlockMap.erase(fitor);
	}
	m_Lock->Unlock();
}
