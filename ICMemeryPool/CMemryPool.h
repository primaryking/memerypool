#ifndef CMEMRYPOOL_H
#define CMEMRYPOOL_H

#include <list>
#include <vector>
#include <map>

#define MIN_MEMERY_SIZE 1024*1024*2 //2M

/*
	�ڴ浥Ԫ
	pData����ַ
	dataSize����ַ��С
*/
struct MemeryUnit
{
    void* pData;
    unsigned dataSize;
};



/*
	�ڴ��
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
	std::map<void*,int> m_UsedBlockSizeMap; //ʹ���ڴ��Ӧ�� ��Ԫ��С
    unsigned int m_CurrentPos;
};


/*
	�ڴ��
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
	std::map<void*,int> m_UsedMemeryBlockMap;//ʹ���ڴ� ��Ӧ��CMemeryBlock �±�
	CSingleLock* m_Lock;
	CMutex m_Mutex;
};

#endif // CMEMRYPOOL_H
