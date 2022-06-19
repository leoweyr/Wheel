#include <windows.h>
#include "processthreadsapi.h"

namespace Multithreading{
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

    DWORD WINAPI ThreadProc(LPVOID lpParameter) {
        C_Thread* thread = (C_Thread*)lpParameter;
        if(thread->m_hMutex){
            WaitForSingleObject(thread->m_hMutex,thread->m_dwMilliseconds);
        }
        thread->m_threadReturn = thread->m_ThreadFunction(thread->m_threadParam);
        if(thread->m_hMutex){
            ReleaseMutex(thread->m_hMutex);
        }
        return 0L;
    }

    void C_Thread::SetSync(HANDLE mutex, DWORD milliseconds) {
        m_hMutex = mutex;
        m_dwMilliseconds = milliseconds;
    }

    void C_Thread::Start() {
        HANDLE thread = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
        CloseHandle(thread);
    }
}