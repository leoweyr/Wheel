#include "SystemOperate.h"

bool ExeCmd(char* cmd,char* result){
    char buffer[256];
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
    std::string result_str = result;
    StringToChar(result,result_str);
    return true;
}

bool CopyFile(std::string destDir,std::string sourcePath){
    FILE *sourceFile = fopen(sourcePath.data(),"rb");
    if(!sourceFile){
        return false;
    }
    if(_access(destDir.data(),0) == -1){
        if(_mkdir(destDir.data()) == -1){
            return false;
        }
    }
    std::string destPath = destDir + "\\" + StringSplit(sourcePath,"\\")[StringSplit(sourcePath,"\\").size() - 1];
    FILE *destFile = fopen(destPath.data(),"wb");
    if(!destFile){
        return false;
    }
    unsigned char *buffer;
    unsigned int len;
    fseek(sourceFile,0,SEEK_END);
    len = ftell(sourceFile);
    buffer = new unsigned char[len+1];
    memset(buffer,0,len+1);
    fseek(sourceFile,0,SEEK_SET);
    fread(buffer,len,1,sourceFile);
    fwrite(buffer,len,1,destFile);
    fclose(sourceFile);
    fclose(destFile);
    delete [] buffer;
    return true;
}

std::vector<std::string> DirList(std::string &dirPath) {
    std::vector<std::string> dirFiles;
    intptr_t hFile = 0;
    _finddata_t fileInfo;
    if ((hFile = _findfirst(dirPath.append("/*").c_str(), &fileInfo)) != -1) {
        while (_findnext(hFile, &fileInfo) == 0) {
            if (strcmp(fileInfo.name, ".."))
                dirFiles.push_back(fileInfo.name);
        }
        _findclose(hFile);
    }
    return dirFiles;
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