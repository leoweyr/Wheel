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

std::vector<std::string> StringSplit(const std::string unitedString, const std::string pattern){
    std::vector<std::string> splitStringArray;
    if (unitedString != ""){
        const_cast<std::string &>(unitedString) = unitedString + pattern;
        for (size_t pattern_pos = unitedString.find(pattern);pattern_pos != std::string::npos;pattern_pos = unitedString.find(pattern)){
            std::string splitStringUnit = unitedString.substr(0, pattern_pos);
            splitStringArray.push_back(splitStringUnit);
            const_cast<std::string &>(unitedString) = unitedString.substr(pattern_pos + pattern.size(), unitedString.size());
        }
    }
    return splitStringArray;
}

std::string StringUnite(const std::vector<std::string> splitStringArray, const std::string pattern){
    std::string UnitedString = "";
    for(std::vector<std::string>::iterator splitStringArray_iter = const_cast<std::vector<std::string> &>(splitStringArray).begin();splitStringArray_iter != splitStringArray.end();splitStringArray_iter++){
        UnitedString += (*splitStringArray_iter) + pattern;
    }
    return UnitedString.substr(0,UnitedString.size() - pattern.size());
}