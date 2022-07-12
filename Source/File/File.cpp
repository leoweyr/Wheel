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