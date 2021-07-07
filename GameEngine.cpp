#include "GameEngine.h"
#include <iostream>

GameEngine::GameEngine()
    : EngineThread("Engine")
    , m_FrameTiming(300)
    , m_Easer()
    , m_Reader()
{
    StartThread();
}

GameEngine::~GameEngine()
{
    m_Easer.StopThread();
    m_Reader.StopThreadWithoutJoining();
    m_Reader.Join();
    StopThread();
}

void GameEngine::Run()
{
    auto start = TimeNow();
    Duration elapsed = TimeNow() - start;
    std::cout << "-- You can use 'v=<num>' to set target" << std::endl;
    std::cout << "-- You can use 'd=<num>' to set easing duration" << std::endl;
    std::cout << "-- You can use 'f=<num>' to set function (linear, sinusoidal, quadratic)" << std::endl;
    std::cout << "-- You can use 'y=<num>' to set easing type (In, Out, or InOut)" << std::endl;
    std::cout << "-- You can use 'e=<num>' to set gameEngine main function sleep" << std::endl << std::endl;
    std::cout << "-- You can use 'q'       to quit properly" << std::endl << std::endl;

    std::cout << "\t\t|Start Engine thread" << std::endl;
    m_Easer.StartThread();
    m_Reader.StartThread();

    while (m_Run)
    {
        start = TimeNow();
        RunOneFrame(elapsed);

        elapsed = TimeNow() - start;
        if (elapsed < m_FrameTiming)
            std::this_thread::sleep_for(m_FrameTiming - (elapsed));
    }
}

void GameEngine::RunOneFrame(Duration& elapsed)
{
    std::string line;
    if (m_Reader.WaitForInputMT(line))
    {
        if (line.empty())
        {
            return;
        }

        if (line[0] == 'q')
        {
            m_Run = false;
            return;
        }

        if (line.size() >= 3)
        {
            if (line[0] == 'v')
            {
                SetEasingTarget((float)::atof(line.substr(2).c_str()));
            }
            else if (line[0] == 'd')
            {
                SetEasingDuration((float)::atof(line.substr(2).c_str()));
            }
            else if (line[0] == 'f')
            {
                uint32_t t = ::atoi(line.substr(2).c_str());
                if (t < Function::COUNT)
                {
                    SetEasingFunc((Function)t);
                }
            }
            else if (line[0] == 'y')
            {
                uint32_t t = ::atoi(line.substr(2).c_str());
                if (t > 0 && t <= 3)
                {
                    SetEasingType((EasingType)t);
                }
            }
            else if (line[0] == 'e')
            {
                SetFrameTiming(::atol(line.substr(2).c_str()));
            }
        }
    }

    //std::cout << "\t\t|engine delta: " << elapsed.count() << "ms" << std::endl;
}
