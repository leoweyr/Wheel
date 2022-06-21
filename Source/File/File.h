#pragma once

#include <fstream>
#include <string>

#include "../../Lib/Jsoncpp/Jsoncpp.h"

#pragma comment(lib,"../../Lib/Jsoncpp/Jsoncpp.lib")

namespace N_File{
    class C_File{
        private:
            std::string m_name;
            std::string m_dirPath;
            std::string m_path;

        public:
            C_File(std::string dirPath, std::string fileName);
            void Write(std::string content);
            void Write(Json::Value content);
            bool Read(std::string &content);
            bool Read(Json::Value &content);
    };
}