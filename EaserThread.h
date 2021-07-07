#pragma once
// demo code by J. Kaltenmark

#include "EngineThread.h"
#include "Easing.h"

typedef std::chrono::duration<double> DurationSec;

class EaserThread : public EngineThread
{
public:
    EaserThread();
    virtual ~EaserThread() {}

    void Run();

    void AdjustTargetMT(const float& target);
    void AdjustFunctionMT(const Function& func);
    void AdjustTypeMT(const EasingType& type);
    void AdjustDurationMT(const float& duration);

private:
    std::mutex m_EaseLock;
    EasedValue<float> m_EaseValue;
    float m_Target;
    EasingContext m_Context;
    Duration m_FrameTiming;
};