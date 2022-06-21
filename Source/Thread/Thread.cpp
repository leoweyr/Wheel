#include "Thread.h"

DWORD WINAPI N_Thread::ThreadProc(LPVOID lpParameter) {
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

void N_Thread::C_Thread::SetSync(HANDLE mutex, DWORD milliseconds) {
    m_hMutex = mutex;
    m_dwMilliseconds = milliseconds;
}

void N_Thread::C_Thread::Start() {
    HANDLE thread = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
    CloseHandle(thread);
}