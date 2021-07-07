#include "EngineThread.h"

EngineThread::EngineThread(const char* name, bool autoStart)
    : m_Name(name)
{
    if (autoStart)
        StartThread();
}

EngineThread::~EngineThread()
{
    StopThread();
}

void EngineThread::StartThread()
{
    m_thread = std::thread(&EngineThread::RunInternal, this);
    std::unique_lock<std::mutex> locker(m_startMutex);
    m_cv.wait(locker);
    m_Run = true;
}

void EngineThread::Join()
{
    if (m_thread.joinable())
        m_thread.join();
}

void EngineThread::StopThreadWithoutJoining()
{
    m_Run = false;
}

void EngineThread::StopThread()
{
    m_Run = false;
    Join();
}

bool EngineThread::isStarted()
{
    return m_Run;
}

void EngineThread::RunInternal()
{
    m_cv.notify_one();
    Run();
}
