#pragma once

#include <windows.h>
#include "processthreadsapi.h"

namespace N_Thread{
    class C_Thread{
        private:
            int (*m_ThreadFunction)(void*);
            void* m_threadParam;
            int m_threadReturn;
            HANDLE m_hMutex;
            DWORD m_dwMilliseconds;
            friend DWORD WINAPI ThreadProc(LPVOID lpParameter);

        public:
            C_Thread(int (*ThreadFunction)(void*), void* threadParam){
                m_ThreadFunction = ThreadFunction;
                m_threadParam = threadParam;
            }
            void SetSync(HANDLE mutex,DWORD milliseconds);
            void Start();
        };
}