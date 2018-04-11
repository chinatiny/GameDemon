#include "CommTool.h"
#include <windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

namespace fs = std::tr2::sys;


//获取指定目录下，指定扩展名文件列表，由vecFiles返回
void GetAllFiles(char *filePath, char *suffix, std::vector<std::string> &vecFiles)
{
	vecFiles.clear();
	fs::directory_iterator end_itr1;

	for (fs::directory_iterator itr(filePath); itr != end_itr1; ++itr)
	{
		if (fs::is_directory(itr->status()))
		{
			continue;
		}
		if (itr->path().extension() == suffix)
		{
			vecFiles.push_back(itr->path().filename().c_str());
		}
	}
}

//获取exe执行的根目录
void GetExeRootPath(std::string &strRootPath)
{
	CHAR szBuff[MAX_PATH] = {0};
	::GetModuleFileNameA(NULL, szBuff, sizeof(szBuff)-1);
	PathRemoveFileSpecA(szBuff);
	strRootPath = szBuff;
	strRootPath += "\\";
}

void RandomFromList(std::vector<int> &vecIndex, int nNeedNum, int nMaxNum)
{
	std::vector<int> tmpVec;
	for (int i = 0; i < nMaxNum; i++)
	{
		tmpVec.push_back(i);
	}
	for (int i = 0; i < nNeedNum; i++)
	{
		int nIndex = rand() % nMaxNum;
		vecIndex.push_back(tmpVec[nIndex]);
	}

}


typedef struct {
	int nMin;
	int nMax;
}Region;
int GetWeightOutput(std::vector<int> &vecWeight)
{
	int nTotal = 0;
	std::vector<Region> vecRegion;
	for (size_t i = 0; i < vecWeight.size(); i++)
	{
		Region region;
		region.nMin = nTotal;
		nTotal += vecWeight[i];
		region.nMax = nTotal;
		vecRegion.push_back(region);
	}
	int nRandomValue = rand() % nTotal;
	for (size_t i = 0; i < vecRegion.size(); i++)
	{
		if (nRandomValue >= vecRegion[i].nMin && nRandomValue < vecRegion[i].nMax)
		{
			return i;
		}
	}
	return -1;
}