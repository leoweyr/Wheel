#include "File.h"

int N_File::Is(const std::string path) {
    struct stat buffer;
    if(stat(path.data(),&buffer) == 0 && S_ISREG(buffer.st_mode)){
        return IS_FILE;
    }else if(stat(path.data(),&buffer) == 0 && S_ISDIR(buffer.st_mode)){
        return IS_DIR;
    } else{
        return IS_NULL;
    }
}

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

bool N_File::C_File::IsExist() {
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

bool N_File::C_Dir::IsExist(const int mode) {
    if(_access(m_path.data(),0) == -1){
        if(mode == 1){
            std::string pathLevel;
            std::vector<std::string> m_path_split = StringSplit(m_path,"\\");
            for (std::vector<std::string>::iterator m_path_split_iter = m_path_split.begin(); m_path_split_iter != m_path_split.end(); m_path_split_iter++) {
                pathLevel += (*m_path_split_iter) + "\\";
                if(C_Dir(pathLevel).IsExist() == false){
                    _mkdir(pathLevel.data());
                }
            }
        }
        return false;
    }else{
        return true;
    }
}

std::vector<std::string> N_File::C_Dir::List() {
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

bool N_File::C_Dir::Delete(const std::string path) {
    const_cast<std::string &>(path) = (m_delete_roll = 0)?(m_path):(path);
    C_Dir currentDir(path.data());
    std::vector<std::string> subFiles;
    subFiles = currentDir.List();
    for (std::vector<std::string>::iterator subFiles_iter = subFiles.begin(); subFiles_iter != subFiles.end(); subFiles_iter++) {
        if(Is(path + "\\" + (*subFiles_iter)) == IS_FILE){
            if(remove((path + "\\" + (*subFiles_iter)).data()) != 0){
                return false;
            }
        }else if(Is(path + "\\" + (*subFiles_iter)) == IS_DIR){
            m_delete_roll ++;
            Delete(path + "\\" + (*subFiles_iter));
            m_delete_roll --;
        }
    }
    rmdir(path.data());
    return true;
}