#pragma once

#include <string>

#include "Jsoncpp.h"

namespace Json{
    class Comparer{
        private:
            Json::Value m_differences;
            Json::Value m_differences_roll;
        public:
            //The method name design caters to the original habit of Jsoncpp ontology.
            Json::Value compare(const Json::Value compared, const Json::Value referred, Json::Value &same);
            bool isSame(const std::string key);
    };
}