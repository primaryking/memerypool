#ifndef CMEMRYPOOL_H
#define CMEMRYPOOL_H

#include <list>
#include <vector>
#include <map>

#define MIN_MEMERY_SIZE 1024*1024*2 //2M

/*
	内存单元
	pData：地址
	dataSize：地址大小
*/
struct MemeryUnit
{
    void* pData;
    unsigned dataSize;
};



/*
	内存块
*/
class CMemeryBlock
{
public:
    CMemeryBlock(unsigned int memerysize, unsigned int unitSize);
    ~CMemeryBlock();

public:
     void* GetMemery(unsigned int size);
     bool IsFree();
	 void FreeMemery(void* mem);
	 int CalSize(unsigned int size);

protected:
     void InitBlock();
     inline unsigned int GetLastPos()
     {
         return m_MemeryUnitVec.size() -1;
     }

private:
    unsigned char *m_MemeryPtr;
    unsigned int m_MemerySize;
    unsigned int m_UnitSize;
    std::vector<MemeryUnit*> m_MemeryUnitVec;
	std::map<void*,int> m_UsedBlockSizeMap; //使用内存对应的 单元大小
    unsigned int m_CurrentPos;
};


/*
	内存池
*/
class CMemeryPool
{
public:
    CMemeryPool();
    ~CMemeryPool();

    void* GetMemery(int size);
	void  FreeMemery(void* mem);

private:
    std::vector<CMemeryBlock*> m_Pool;
	std::map<void*,int> m_UsedMemeryBlockMap;//使用内存 对应的CMemeryBlock 下标
	CSingleLock* m_Lock;
	CMutex m_Mutex;
};

#endif // CMEMRYPOOL_H
