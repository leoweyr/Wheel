#include "JsonCompare.h"
#include <iostream>//

Json::Value Json::Compare(const Json::Value compared, const Json::Value referred, Json::Value &same) {
    Json::Value differences, subSame, subDifferences;
    int differencesCount = 0;
    Json::Value::Members compared_keys = compared.getMemberNames();
    Json::Value::Members referred_keys = referred.getMemberNames();
    int compared_keys_iter_traversed;
    for(Json::Value::Members::iterator referred_keys_iter = referred_keys.begin(); referred_keys_iter != referred_keys.end(); referred_keys_iter++){
        compared_keys_iter_traversed = 0;
        for(Json::Value::Members::iterator compared_keys_iter = compared_keys.begin(); compared_keys_iter != compared_keys.end(); compared_keys_iter++){
            if(*compared_keys_iter == *referred_keys_iter){
                if(compared[*compared_keys_iter].isObject() == 1 && referred[*referred_keys_iter].isObject() == 1){
                    if(compared[*compared_keys_iter].getMemberNames().size() != 0 && referred[*referred_keys_iter].getMemberNames().size() != 0) {
                        subDifferences = Json::Compare(compared[*compared_keys_iter], referred[*referred_keys_iter], subSame);
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
    return differences;
}