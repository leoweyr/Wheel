#include "JsonCompare.h"

Json::Value Json::Comparer::compare(const Json::Value compared, const Json::Value referred, Json::Value &same) {
    Json::Value::Members compared_keys = compared.getMemberNames(), referred_keys = referred.getMemberNames();
    Json::Value differences, subSame, subDifferences;
    int differencesCount = 0, compared_keys_iter_traversed;
    for(Json::Value::Members::iterator referred_keys_iter = referred_keys.begin(); referred_keys_iter != referred_keys.end(); referred_keys_iter++){
        compared_keys_iter_traversed = 0;
        for(Json::Value::Members::iterator compared_keys_iter = compared_keys.begin(); compared_keys_iter != compared_keys.end(); compared_keys_iter++){
            if(*compared_keys_iter == *referred_keys_iter){
                if(compared[*compared_keys_iter].isObject() == 1 && referred[*referred_keys_iter].isObject() == 1){
                    if(compared[*compared_keys_iter].getMemberNames().size() != 0 && referred[*referred_keys_iter].getMemberNames().size() != 0) {
                        subDifferences = compare(compared[*compared_keys_iter], referred[*referred_keys_iter], subSame);
                        differences[*referred_keys_iter] = subDifferences;
                        same[*referred_keys_iter] = subSame;
                        break;
                    }
                }else if(compared[*compared_keys_iter] == referred[*referred_keys_iter]){
                    same[*referred_keys_iter] = referred[*referred_keys_iter];
                    break;
                }
            }
            compared_keys_iter_traversed++;
            if (compared_keys_iter_traversed == compared_keys.size()) {
                differencesCount++;
            }
        }
    }
    differences["DIFFERENCESCOUNT"] = differencesCount;
    m_differences = differences;
    m_differences_roll = m_differences;
    return differences;
}

bool Json::Comparer::isSame(const std::string key = "") {
    Json::Value differences = (key.size() == 0)?(m_differences_roll):(m_differences_roll[key]), differences_roll_backup;
    Json::Value::Members differences_keys = differences.getMemberNames();
    if(m_differences_roll["DIFFERENCESCOUNT"] != 0){
        return false;
    }
    for(Json::Value::Members::iterator iter = differences_keys.begin();iter != differences_keys.end();iter++){
        if(m_differences_roll[*iter].isObject() == 1){
            differences_roll_backup = m_differences_roll;
            m_differences_roll = m_differences_roll[*iter];
            if(isSame(*iter) == false){
                return false;
            }
            m_differences_roll = differences_roll_backup;
        }
    }
    return true;
}