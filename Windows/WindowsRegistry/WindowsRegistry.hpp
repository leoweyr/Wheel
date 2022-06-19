#pragma once

#include <stdio.h>
#include <assert.h>
#include <tchar.h>

#include <iostream>
#include <string>
#include <vector>

#include <windows.h>
#include <Winreg.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

namespace N_WindowsRegistry {
    struct S_Value_SZ {
        std::string valueName;
        std::string valueData;
    };

    class C_Registry {
    private:
        HKEY m_hKey;
        TCHAR m_achClass[MAX_PATH] = TEXT(""); //buffer for class name
        DWORD m_cchClassName = MAX_PATH; //size of class string
        DWORD m_cSubKeys = 0; //number of subkeys
        DWORD m_cbMaxSubKey; //size of longest subkey
        DWORD m_cchMaxClass; //size of longest class string
        DWORD m_cValues; //number of values for key
        DWORD m_cchValue = MAX_VALUE_NAME; // max size of value name
        DWORD m_cchMaxValue; //size of longest value name
        DWORD m_cbMaxValueData; //size of longest value data
        DWORD m_cbSecurityDescriptor; //size of security descriptor
        FILETIME m_ftLastWriteTime; // last write time

    public:
        C_Registry(HKEY hKey = HKEY_CURRENT_USER) {//Get Administrator exchange:HKEY_LOCAL_MACHINE
            m_hKey = hKey;
        };
        bool SetHKEY(LPCTSTR lpstrKey);
        bool QueryInfo();
        bool OpenKey(LPCTSTR lpSubKey);
        void Close();
        bool CreateKey(LPCTSTR lpSubKey);
        bool DeleteKey(LPCTSTR lphKey, LPCTSTR lpSubKey);
        bool DeleteValue(LPCTSTR lpValueName);
        bool SaveKey(LPCTSTR lpFileName);
        bool RestoreKey(LPCTSTR lpFileName);
        bool Read(LPCTSTR lpValueName, std::string *spValueData);
        bool Read(LPCTSTR lpValueName, DWORD *dwpValueData);
        bool Read(LPCTSTR lpValueName, int *ipValueData);
        bool Write(LPCTSTR lpValueName, LPCTSTR lpValueData);
        bool Write(LPCTSTR lpValueName, DWORD dwValueData);
        bool Write(LPCTSTR lpValueName, int iValueData);
        bool ForeachKey(std::vector<std::string> *keyArray);
        bool ForeachValue(std::vector<S_Value_SZ> *valueArray);
        ~C_Registry() {
            Close();
        }
    };

    bool C_Registry::SetHKEY(LPCTSTR lpstrKey) {
        assert(m_hKey);
        assert(lpstrKey);
        if (0 == strcmp(lpstrKey, "HKEY_CLASSES_ROOT")) {
            m_hKey = HKEY_CLASSES_ROOT;
            return true;
        }
        if (0 == strcmp(lpstrKey, "HKEY_CURRENT_USER")) {
            m_hKey = HKEY_CURRENT_USER;
            return true;
        }
        if (0 == strcmp(lpstrKey, "HKEY_LOCAL_MACHINE")) {
            m_hKey = HKEY_LOCAL_MACHINE;
            return true;
        }
        if (0 == strcmp(lpstrKey, "HKEY_USERS")) {
            m_hKey = HKEY_USERS;
            return true;
        }
        if (0 == strcmp(lpstrKey, "HKEY_PERFORMANCE_DATA")) {
            m_hKey = HKEY_PERFORMANCE_DATA;
            return true;
        }
        if (0 == strcmp(lpstrKey, "HKEY_PERFORMANCE_TEXT")) {
            m_hKey = HKEY_PERFORMANCE_TEXT;
            return true;
        }
        if (0 == strcmp(lpstrKey, "HKEY_PERFORMANCE_NLSTEXT")) {
            m_hKey = HKEY_PERFORMANCE_NLSTEXT;
            return true;
        }
#if(WINVER >= 0x0400)
        if( 0 == strcmp(lpstrKey,"HKEY_CURRENT_CONFIG") ){
        m_hKey = HKEY_CURRENT_CONFIG;
        return true;
        }
        if( 0 == strcmp(lpstrKey,"HKEY_DYN_DATA") ){
        m_hKey = HKEY_DYN_DATA;
        return true;
        }
#endif
        return false;
    }

    bool C_Registry::QueryInfo() {
        assert(m_hKey);
        long lRetCode = RegQueryInfoKey(
                m_hKey,
                m_achClass,
                &m_cchClassName,
                NULL,
                &m_cSubKeys,
                &m_cbMaxSubKey,
                &m_cchMaxClass,
                &m_cValues,
                &m_cchMaxValue,
                &m_cbMaxValueData,
                &m_cbSecurityDescriptor,
                &m_ftLastWriteTime);
        if(lRetCode == ERROR_SUCCESS){
            return true;
        }else{
            return false;
        }
    }

    bool C_Registry::OpenKey(LPCTSTR lpSubKey) {
        //lpSubKey = lpSubKeyConsist(lpSubKeyTail);
        assert(m_hKey);
        assert(lpSubKey);
        HKEY hKey;
        long lRetCode = RegOpenKeyEx(m_hKey, lpSubKey, 0L, KEY_READ | KEY_WRITE | KEY_EXECUTE, &hKey);
        if (lRetCode == ERROR_SUCCESS) {
            m_hKey = hKey;
            return true;
        }
        return false;
    }

    void C_Registry::Close() {
        if (m_hKey) {
            RegCloseKey(m_hKey);
            m_hKey = NULL;
        }
    }

    bool C_Registry::CreateKey(LPCTSTR lpSubKey) {
        //CreateKey() contains the OpenKey() operation.
        //lpSubKey = lpSubKeyConsist(lpSubKeyTail);
        assert(m_hKey);
        assert(lpSubKey);
        HKEY hKey;
        DWORD dw;
        long lRetCode = RegCreateKeyEx(m_hKey, lpSubKey, 0L, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,
                                      &dw);
        if (lRetCode == ERROR_SUCCESS) {
            m_hKey = hKey;
            return true;
        }
        return false;
    }

    bool C_Registry::DeleteKey(LPCTSTR lphKey, LPCTSTR lpSubKey) {
        assert(lphKey);
        assert(lpSubKey);
        assert(m_hKey);
        SetHKEY(lphKey);
        long lRetCode = RegDeleteValue(m_hKey, lpSubKey);
        if (lRetCode == ERROR_SUCCESS) {
            return true;
        }
        return false;
    }

    bool C_Registry::DeleteValue(LPCTSTR lpValueName) {
        assert(m_hKey);
        assert(lpValueName);
        long lRetCode = RegDeleteValue(m_hKey, lpValueName);
        if (lRetCode == ERROR_SUCCESS) {
            return true;
        }
        return false;
    }

    bool C_Registry::SaveKey(LPCTSTR lpFileName) {
        assert(m_hKey);
        assert(lpFileName);
        long lRetCode = RegSaveKey(m_hKey, lpFileName, NULL);
        if (lRetCode == ERROR_SUCCESS) {
            return true;
        }
        return false;
    }

    bool C_Registry::RestoreKey(LPCTSTR lpFileName) {
        assert(m_hKey);
        assert(lpFileName);
        long lRetCode = RegRestoreKey(m_hKey, lpFileName, 0);
        if (lRetCode) {
            return true;
        }
        return false;
    }

    bool C_Registry::Read(LPCTSTR lpValueName, std::string *spValueData) {
        assert(m_hKey);
        assert(lpValueName);
        assert(spValueData);
        DWORD dwType;
        DWORD dwSize = 200;
        char szString[2048];
        memset(szString, 0, 2048 * sizeof(char));
        long lRetCode = RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, (BYTE *) szString, &dwSize);
        if (lRetCode == ERROR_SUCCESS) {
            *spValueData = szString;
            return true;
        }
        return false;
    }

    bool C_Registry::Read(LPCTSTR lpValueName, DWORD *dwpValueData) {
        assert(m_hKey);
        assert(lpValueName);
        assert(dwpValueData);
        DWORD dwType;
        DWORD dwSize = sizeof(DWORD);
        DWORD dwDest;
        long lRetCode = RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, (BYTE * ) & dwDest, &dwSize);
        if (lRetCode == ERROR_SUCCESS) {
            *dwpValueData = dwDest;
            return true;
        }
        return false;
    }

    bool C_Registry::Read(LPCTSTR lpValueName, int *ipValueData) {
        assert(m_hKey);
        assert(lpValueName);
        assert(ipValueData);
        DWORD dwType;
        DWORD dwSize = sizeof(DWORD);
        DWORD dwDest;
        long lRetCode = RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, (BYTE * ) & dwDest, &dwSize);
        if (lRetCode == ERROR_SUCCESS) {
            *ipValueData = (int) dwDest;
            return true;
        }
        return false;
    }

    bool C_Registry::Write(LPCTSTR lpValueName, LPCTSTR lpValueData) {
        assert(m_hKey);
        assert(lpValueName);
        assert(lpValueData);
        long lRetCode = RegSetValueEx(m_hKey, lpValueName, 0L, REG_SZ, (const BYTE *) lpValueData,
                                     strlen(lpValueData) + 1);
        if (lRetCode == ERROR_SUCCESS) {
            return true;
        }
        return false;
    }

    bool C_Registry::Write(LPCTSTR lpValueName, DWORD dwValueData) {
        assert(m_hKey);
        assert(lpValueName);
        long lRetCode = RegSetValueEx(m_hKey, lpValueName, 0L, REG_DWORD, (const BYTE *) &dwValueData, sizeof(DWORD));
        if (lRetCode == ERROR_SUCCESS) {
            return true;
        }
        return false;
    }

    bool C_Registry::Write(LPCTSTR lpValueName, int iValueData) {
        assert(m_hKey);
        assert(lpValueName);
        DWORD iValueDate_dword;
        iValueDate_dword = (DWORD) iValueData;
        long lRetCode = RegSetValueEx(m_hKey, lpValueName, 0L, REG_DWORD, (const BYTE *) &iValueDate_dword,
                                     sizeof(DWORD));
        if (lRetCode == ERROR_SUCCESS) {
            return true;
        }
        return false;
    }

    bool C_Registry::ForeachKey(std::vector<std::string> *keyArray) {
        assert(m_hKey);
        assert(keyArray);
        TCHAR achKey[MAX_KEY_LENGTH];
        DWORD cbName;
        long lRetCode;
        QueryInfo();
        if(m_cSubKeys){
            for(DWORD i = 0;i < m_cSubKeys;i++){
                cbName = MAX_KEY_LENGTH;
                lRetCode = RegEnumKeyEx(m_hKey,i,achKey,&cbName,NULL,NULL,NULL,&m_ftLastWriteTime);
                if(lRetCode == ERROR_SUCCESS){
                    keyArray->push_back((LPSTR)achKey);
                }else{
                    return false;
                }
            }
            return true;
        }else{
            return false;
        }
    }

    bool C_Registry::ForeachValue(std::vector<S_Value_SZ> *valueArray) {
        assert(m_hKey);
        assert(valueArray);
        S_Value_SZ value;
        TCHAR achValue[MAX_VALUE_NAME];
        DWORD cchValue;
        DWORD dwType;
        long lRetCode;
        QueryInfo();
        if(m_cValues){
            for(DWORD i = 0;i < m_cValues;i++){
                cchValue = MAX_VALUE_NAME;
                achValue[0] = '\0';
                lRetCode = RegEnumValue(m_hKey,i,achValue,&cchValue,NULL,&dwType,NULL,NULL); //TODO:Getting value data is only done in function<RegEnumValue> and not in function<Read>.
                if(lRetCode == ERROR_SUCCESS && dwType == REG_SZ){
                    value.valueName = (LPSTR)achValue;
                    Read((LPCTSTR)achValue,&value.valueData);
                    valueArray->push_back(value);
                }else{
                    return false;
                }
            }
            return true;
        }else{
            return false;
        }
    }
}