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

namespace N_WindowsRegistry{
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
        C_Registry(HKEY hKey = HKEY_CURRENT_USER) { //TODO:Get Administrator exchange:HKEY_LOCAL_MACHINE
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
}