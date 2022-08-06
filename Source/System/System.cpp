#include "System.h"

bool ExeCmd(const char* cmd,char* result){
    char buffer[BUFFER_SIZE];
    FILE* pipe = _popen(cmd,"r");
    if(!pipe){
        return false;
    }
    while (!feof(pipe)){
        if(fgets(buffer,256,pipe)){
            std::strcat(result,buffer);
        }
    }
    _pclose(pipe);
    return true;
}

DWORD QueryProcessID(std::string processName) {
    DWORD processID;
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if(INVALID_HANDLE_VALUE == hSnapshot){
        return false;
    }
    if (Process32First(hSnapshot, &pe) == true)
    {
        while (Process32Next(hSnapshot, &pe) == true)
        {
            if (std::string(pe.szExeFile) == processName) {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pe.th32ProcessID);
                processID = GetProcessId(hProcess);
                CloseHandle(hProcess);
            }
        }
    }
    CloseHandle(hSnapshot);
    return processID;
}

bool IsProcessExist(DWORD processID)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    if(INVALID_HANDLE_VALUE == hSnapshot){
        return false;
    }
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    if(!Process32First(hSnapshot,&pe)){
        CloseHandle(hSnapshot);
        return false;
    }
    while(1){
        //pe.dwSize = sizeof(PROCESSENTRY32);
        if(Process32Next(hSnapshot,&pe)==FALSE)
        {
            CloseHandle(hSnapshot);
            return false;
        }
        if(processID == pe.th32ProcessID)
        {
            CloseHandle(hSnapshot);
            return true;
        }
    }
}