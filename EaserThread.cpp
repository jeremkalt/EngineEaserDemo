#include "EaserThread.h"
#include <iostream>

EaserThread::EaserThread()
    : EngineThread("Easer")
    , m_EaseValue(0.f)
    , m_Target(0.f)
    , m_Context({ Quadratic, EaseInOut, 0.f, 0.3f })
{

}

void EaserThread::Run()
{
    auto start = TimeNow();
    DurationSec elapsed = TimeNow() - start;
    std::cout << "\t\t\t\t\t\t|Start Easing thread" << std::endl;
    EasingContext context;
    float output = 0;

    while (m_Run)
    {
        elapsed = TimeNow() - start;

        {
            std::unique_lock<std::mutex> locker(m_EaseLock);
            context = m_Context;
            m_EaseValue.Push(m_Target, (float)elapsed.count());
        }
        context.Time = (float)elapsed.count();

        m_EaseValue.Evaluate(output, context);
        if (output != m_EaseValue.Last())
        {
            std::cout << "\t\t\t\t\t\t|Eased value is :" << output << std::endl;
        }

        Duration elapsedMilli = TimeNow() - start;
        if (elapsed < m_FrameTiming)
            std::this_thread::sleep_for(m_FrameTiming - (elapsed));
    }
}

void EaserThread::AdjustTargetMT(const float& target)
{
    std::unique_lock<std::mutex> locker(m_EaseLock);
    m_Target = target;
}

void EaserThread::AdjustFunctionMT(const Function& func)
{
    std::unique_lock<std::mutex> locker(m_EaseLock);
    m_Context.Function = func;
}

void EaserThread::AdjustTypeMT(const EasingType& type)
{
    std::unique_lock<std::mutex> locker(m_EaseLock);
    m_Context.Type = type;
}

void EaserThread::AdjustDurationMT(const float& duration)
{
    std::unique_lock<std::mutex> locker(m_EaseLock);
    if (duration > 0.f)
        m_Context.Duration = duration;
}
