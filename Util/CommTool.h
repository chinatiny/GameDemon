#pragma  once

#include <filesystem>
#include <vector>
#include <string>

//获取指定目录下，指定扩展名文件列表，由vecFiles返回
void GetAllFiles(char *filePath, char *suffix, std::vector<std::string> &vecFiles);

//获取当前进程执行的根目录
void GetExeRootPath(std::string &strRootPath);

//从列表中随机nRandomNum个index（index不能重复）
void RandomFromList(std::vector<int> &vecIndex, int nNeedNum, int nMaxNum);

//根据weight权重进行随机，返回它的索引
int GetWeightOutput(std::vector<int> &vecWeight);