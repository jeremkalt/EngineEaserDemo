#pragma once
// demo code by J. Kaltenmark

#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>

using namespace std::chrono;
#define TimeNow std::chrono::high_resolution_clock::now
typedef std::chrono::duration<double, std::milli> Duration;

class EngineThread
{
public:
    EngineThread(const char* name, bool autoStart = false);

    virtual ~EngineThread();
    virtual void Run () = 0;

    void StartThread();
    void Join();
    void StopThreadWithoutJoining();
    void StopThread();
    bool isStarted();

private: 
    void RunInternal();

protected:
    std::mutex m_startMutex;
    volatile bool m_Run;

private: 
    std::condition_variable m_cv;
    std::thread m_thread;
    const char* m_Name;
};