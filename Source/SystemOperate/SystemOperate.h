#pragma once

#include <io.h>
#include <direct.h>

#include <fstream>
#include <string>
#include <cstring>
#include <vector>

#include <Tlhelp32.h>

#include "../../Lib/Bitscode/Bitscode.h"

#pragma comment(lib,"../../Lib/Bitscode/Bitscode.lib")

class File{
    public:
        std::string Name;
        std::string DirPath;
        std::string Path;

        File(std::string dirPath,std::string fileName){
            Name = fileName;
            DirPath = dirPath;
            Path = dirPath + "\\" + fileName;
        };
        void Write(std::string content);
        bool Read(std::string* content);
};

bool ExeCmd(char* cmd,char* result);

bool CopyFile(std::string destDir,std::string sourcePath);

std::vector<std::string> DirList(std::string &dirPath);

DWORD QueryProcessID(std::string processName);

bool IsProcessExist(DWORD processID);