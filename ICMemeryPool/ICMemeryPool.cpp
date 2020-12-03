// ICMemeryPool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CMemryPool.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>

std::wstring GetFullFilePath(wchar_t* filename)
{
	wchar_t filePath[MAX_PATH] = {0};
	::GetModuleFileNameW(NULL,filePath,MAX_PATH);
	wchar_t* filepathptrr = wcsrchr(filePath,L'\\') + 1;
	*filepathptrr = '\0';
	wcscat(filePath,filename);
	return filePath;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CMemeryPool pool;	
	
	std::wstring filepath = GetFullFilePath(L"ICMemeryPool.rar");
	std::ifstream readfile(filepath.c_str(), std::ios::in | std::ios::binary);
	if (!readfile)
	{
		std::cout<<"read file error!"<<std::endl;
	}
	readfile.seekg(0,readfile.end);
	int filesize = readfile.tellg();
	readfile.seekg(0,readfile.beg);

	char *p = (char*)pool.GetMemery(filesize);
	readfile.read(p,filesize);
	while(filesize > 0)
	{
		char cp = *p;
		if (cp != 0)
		{
			printf("%c",cp);
		}
		p ++;
		filesize --;
	}
	readfile.close();

	pool.FreeMemery((void*)p);

	getch();
	return 0;
}

