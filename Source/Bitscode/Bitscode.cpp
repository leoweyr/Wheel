#include "Bitscode.h"

void CharToString(std::string &dest_str,char *source_charArray){
    int charLen = strlen(source_charArray);
    dest_str = "";
    for(size_t bit=0;bit < charLen;bit++){
        dest_str += *source_charArray;
        source_charArray++;
    }
}

void StringToChar(char *dest_charArray,const std::string &source_str){
    int charLen = source_str.length();
    for (size_t bit=0;bit < charLen;bit++){
        *dest_charArray = source_str.operator[](bit);
        *dest_charArray ++;
    }
    *dest_charArray = '\0';
}

std::vector<std::string> StringSplit(const std::string &str,const std::string &pattern)
{
    std::vector<std::string> strUnitGroup;
    if ("" == str)
    {
        return strUnitGroup;
    }
    std::string strWithPattern = str + pattern;
    size_t pos = strWithPattern.find(pattern);
    size_t size = strWithPattern.size();
    while (pos != std::string::npos)
    {
        std::string strUnit = strWithPattern.substr(0,pos);
        strUnitGroup.push_back(strUnit);
        strWithPattern = strWithPattern.substr(pos + pattern.size(), size);
        pos = strWithPattern.find(pattern);
    }
    return strUnitGroup;
}