#include "JsonCompare.h"

void Json::Comparer::compare(const Json::Value compared, const Json::Value referred) {
    m_jsonLevel ++;
    Json::Value::Members compared_keys = compared.getMemberNames(), referred_keys = referred.getMemberNames();
    Json::Value differencesLevel, sameLevel;
    bool isRollObject = false;
    int differencesCount = 0, compared_keys_iter_traversed;
    for(Json::Value::Members::iterator referred_keys_iter = referred_keys.begin(); referred_keys_iter != referred_keys.end(); referred_keys_iter++){
        compared_keys_iter_traversed = 0;
        for(Json::Value::Members::iterator compared_keys_iter = compared_keys.begin(); compared_keys_iter != compared_keys.end(); compared_keys_iter++){
            if(*compared_keys_iter == *referred_keys_iter){
                if(compared[*compared_keys_iter].isObject() == 1 && referred[*referred_keys_iter].isObject() == 1){
                    if(compared[*compared_keys_iter].getMemberNames().size() != 0 && referred[*referred_keys_iter].getMemberNames().size() != 0) {
                        compare(compared[*compared_keys_iter], referred[*referred_keys_iter]);
                        m_jsonLevel --;
                        differencesLevel[*referred_keys_iter] = m_differences_roll_compare;
                        sameLevel[*referred_keys_iter] = m_same_roll_compare;
                        isRollObject = true;
                        break;
                    }
                }else if(compared[*compared_keys_iter] == referred[*referred_keys_iter]){
                    m_same_roll_compare[*referred_keys_iter] = referred[*referred_keys_iter];
                    break;
                }
            }
            compared_keys_iter_traversed++;
            if (compared_keys_iter_traversed == compared_keys.size()) {
                differencesCount++;
            }
        }
    }
    differencesLevel["DIFFERENCESCOUNT"] = differencesCount;
    if(isRollObject == true) {
        m_same_roll_compare = sameLevel;
    }
    m_differences_roll_compare = differencesLevel;
    if(m_jsonLevel == 1){
        m_compared = compared;
        m_referred = referred;
        differences = m_differences_roll_compare;
        same = m_same_roll_compare;
        m_jsonLevel = 0;
        m_differences_roll_isSame = differences;
    }
}

bool Json::Comparer::isIncluded(const std::string key) {
    Json::Value differences = (key.size() == 0) ? (m_differences_roll_isSame) : (m_differences_roll_isSame[key]), differencesLevel;
    Json::Value::Members differences_keys = differences.getMemberNames();
    if(m_differences_roll_isSame["DIFFERENCESCOUNT"] != 0){
        return false;
    }
    for(Json::Value::Members::iterator iter = differences_keys.begin();iter != differences_keys.end();iter++){
        if(m_differences_roll_isSame[*iter].isObject() == 1){
            differencesLevel = m_differences_roll_isSame;
            m_differences_roll_isSame = m_differences_roll_isSame[*iter];
            if(isIncluded(*iter) == false){
                return false;
            }
            m_differences_roll_isSame = differencesLevel;
        }
    }
    return true;
}

bool Json::Comparer::isSame(const std::string key){
    Json::Value compared = (key.size() == 0)?(m_compared):(m_compared[key]), referred = (key.size() == 0)?(m_referred):(m_referred[key]);
    if(compared == referred){
        return true;
    } else{
        return false;
    }
}