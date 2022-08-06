#pragma once

#include <io.h>
#include <direct.h>

#include <string>
#include <cstring>

#include <Tlhelp32.h>

#define BUFFER_SIZE 256

bool ExeCmd(const char* cmd,char* result);

DWORD QueryProcessID(std::string processName);

bool IsProcessExist(DWORD processID);