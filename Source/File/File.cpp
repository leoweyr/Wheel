#include "File.h"

N_File::C_File::C_File(std::string dirPath, std::string fileName) {
    m_name = fileName;
    m_dirPath = dirPath;
    m_path = dirPath + "\\" + fileName;
}

void N_File::C_File::Write(std::string content) {
    std::fstream file;
    file.open(m_path.data(), std::ios::out | std::ios::trunc);
    file << content << std::endl;
    file.close();
}

void N_File::C_File::Write(Json::Value content) {
    std::fstream file;
    Json::FastWriter fastWriter;
    file.open(m_path.data(), std::ios::out | std::ios::trunc);
    file << fastWriter.write(content) << std::endl;
    file.close();
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