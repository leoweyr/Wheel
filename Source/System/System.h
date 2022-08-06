#pragma once

#include <io.h>
#include <direct.h>

#include <string>
#include <cstring>
#include <vector>

#include <Tlhelp32.h>

//#include "../../Lib/Bitscode/Bitscode.h"

//#pragma comment(lib,"../../Lib/Bitscode/Bitscode.lib")

#define BUFFER_SIZE 256

bool ExeCmd(const char* cmd,char* result);

DWORD QueryProcessID(std::string processName);

bool IsProcessExist(DWORD processID);