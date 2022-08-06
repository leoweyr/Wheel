#include "File.h"

N_File::C_File::C_File(const std::string path) {
    m_path = path;
    for(size_t i=0,iMax=StringSplit(m_path,"\\").size();i < iMax - 1;i++){
        if(i != iMax - 2){
            m_dirPath += StringSplit(m_path,"\\")[i] + "\\";
            if(_access(m_dirPath.data(),0) == -1){
                _mkdir(m_dirPath.data());
            }
        }else{
            m_dirPath += StringSplit(m_path,"\\")[i];
            if(_access(m_dirPath.data(),0) == -1){
                _mkdir(m_dirPath.data());
            }
        }
    }
}

bool N_File::C_File::isExist() {
    return (_access(m_path.data(),0) == -1)?(false):(true);
}

bool N_File::C_File::Read(std::string &content) {
    std::fstream file;
    file.open(m_path.data(), std::ios::in);
    if(!file){
        return false;
    }
    file >> content;
    file.close();
    return true;
}

bool N_File::C_File::Read(Json::Value &content) {
    std::fstream file;
    std::string content_string;
    Json::Reader reader;
    file.open(m_path.data(), std::ios::in);
    if(!file){
        return false;
    }
    file >> content_string;
    file.close();
    if(!reader.parse(content_string,content)){
        return false;
    }
    return true;
}

void N_File::C_File::Write(const std::string content) {
    std::fstream file;
    file.open(m_path.data(), std::ios::out | std::ios::trunc);
    file << content << std::endl;
    file.close();
}

void N_File::C_File::Write(const Json::Value content) {
    std::fstream file;
    Json::FastWriter fastWriter;
    file.open(m_path.data(), std::ios::out | std::ios::trunc);
    file << fastWriter.write(content) << std::endl;
    file.close();
}

bool N_File::C_File::Delete() {
    if(remove(m_path.data()) == 0){
        return true;
    }else{
        return false;
    }
}

int N_File::C_File::Copy(const std::string destPath) {
    std::ifstream sourceFile;
    std::ofstream destFile;
    sourceFile.open(m_path.data(),std::ios::binary);
    if(sourceFile.fail()){
        return -1;
    }
    destFile.open(C_File(destPath).m_path.data(),std::ios::binary);
    if(destFile.fail()){
        return -2;
    }
    destFile << sourceFile.rdbuf();
    destFile.close();
    sourceFile.close();
    return 0;
}

int N_File::C_File::Move(const std::string destPath) {
    int errorCode = Copy(destPath.data());
    if(errorCode != 0){
        return errorCode;
    }
    if(Delete() != true){
        return -3;
    }
    return 0;
}

N_File::C_Dir::C_Dir(const std::string path) {
        m_path = path;
}

bool N_File::C_Dir::isExist(const int mode) {
    if(_access(m_path.data(),0) == -1){
        if(mode == 1){
            _mkdir(m_path.data());
        }
        return false;
    }else{
        return true;
    }
}

std::vector <std::string> N_File::C_Dir::List() {
    std::vector<std::string> filesName;
    intptr_t hFile = 0;
    _finddata_t filesInfo;
    if((hFile = _findfirst(m_path.append("/*").c_str(),&filesInfo)) != -1){
        while(_findnext(hFile,&filesInfo) == 0){
            if(strcmp(filesInfo.name,"..")){
                filesName.push_back(filesInfo.name);
            }
        }
        _findclose(hFile);
    }
    return filesName;
}

std::string N_File::GetCurrentExePath() {
    char currentExePath[MAX_PATH];
    GetModuleFileName(NULL,(LPSTR)currentExePath, sizeof(currentExePath));
    std::string currentExePath_str;
    CharToString(currentExePath_str, currentExePath);
    return currentExePath_str;
}

std::string N_File::GetCurrentWorkingDirectoryPath() {
    char *currentWorkingDirectoryPath;
    currentWorkingDirectoryPath = getcwd(NULL, 0);
    std::string currentWorkingDirectoryPath_str = currentWorkingDirectoryPath;
    free(currentWorkingDirectoryPath);
    return currentWorkingDirectoryPath_str;
}