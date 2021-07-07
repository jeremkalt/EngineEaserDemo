#pragma once
// demo code by J. Kaltenmark

#include "EngineThread.h"
#include "EaserThread.h"
#include "InputReader.h"
#include <iostream>
#include <regex>

class GameEngine : public EngineThread
{
public:
    GameEngine();

    virtual ~GameEngine();

    void Run();
    void RunOneFrame(Duration& elapsed);
    void SetFrameTiming(long time) { m_FrameTiming = milliseconds(time); }

    void SetEasingType(const EasingType& type) { m_Easer.AdjustTypeMT(type); }
    void SetEasingFunc(const Function& func) { m_Easer.AdjustFunctionMT(func); }
    void SetEasingDuration(const float& duration) { m_Easer.AdjustDurationMT(duration); }
    void SetEasingTarget(const float& target) { m_Easer.AdjustTargetMT(target); }


private:
    Duration m_FrameTiming;
    EaserThread m_Easer;
    InputReader m_Reader;
};
