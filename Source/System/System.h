#pragma once

#include <io.h>
#include <direct.h>

#include <string>
#include <cstring>

#include <Tlhelp32.h>
#include "../../Lib/Bitscode/Bitscode.h"

#pragma comment(lib, "../../Lib/Bitscode/Bitscode")

#define BUFFER_SIZE 256
#define CURRENT_EXE_PATH N_File::GetCurrentExePath()
#define CURRENT_WORKING_DIRECTORY_PATH N_File::GetCurrentWorkingDirectoryPath()

bool ExeCmd(const char* cmd,char* result = nullptr);

std::string GetCurrentExePath();

std::string GetCurrentWorkingDirectoryPath();

bool SetCurrentWorkingDirectoryPath(std::string path);

DWORD QueryProcessID(std::string processName);

bool IsProcessExist(DWORD processID);