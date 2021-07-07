#pragma once
// demo code by J. Kaltenmark

#include <assert.h>
#include <cmath>
#include <limits>
#include <vector>

#define PI 3.14159265358979323846f
#define PI_BY_2 1.57079632679489661923f

#ifndef NDEBUG
#   define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cout << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

enum Function
{
    Linear = 0,
    Sinusoidal = 1,
    Quadratic = 2,
    COUNT
};

enum EasingType
{
    EaseIn = 1,     // will ease at begin and stop abrupt 
    EaseOut = 2,    // will ease only while approching target 
    EaseInOut = 3   // will ease at start and end
};

/*
ex:
ease In
                  ______
                 /
                /
        ....---/
...--'''

ease Out          
                       ________
             ....----ии
     ..---'''
    /     
.../

ease InOut
                               ______
                      ...----ии
                 .--''
                /
               /
       ....---/
..--'''
*/

inline float EaseLinear(float t, float b, float c, float d)
{
    t /= d;
    return c * t + b;
}

inline float EaseInSinusoidal(float t, float b, float c, float d)
{
    return -c * std::cos(t / d * PI_BY_2) + c + b;
}

inline float EaseOutSinusoidal(float t, float b, float c, float d)
{
    return c * std::sin(t / d * PI_BY_2) + b;
}

inline float EaseInOutSinusoidal(float t, float b, float c, float d)
{
    return -c * 0.5f * (std::cos(PI * t / d) - 1.0f) + b;
}

inline float EaseInQuadratic(float t, float b, float c, float d)
{
    t /= d;
    return c * t * t + b;
}

inline float EaseOutQuadratic(float t, float b, float c, float d)
{
    t /= d;
    return -c * t * (t - 2.0f) + b;
}

inline float EaseInOutQuadratic(float t, float b, float c, float d)
{
    const auto halfC = c * 0.5f;
    const auto halfD = d * 0.5f;

    return (t < halfD)
        ? EaseInQuadratic(t, b, halfC, halfD)
        : EaseOutQuadratic(t - halfD, b + halfC, halfC, halfD);
}

float EvaluateEasing(EasingType type, Function func, float t, float b, float c, float d);

// really basic templated linear interpolator.
// can be optimized for more complex types like quaternions/vectors
template<typename T, typename U>
constexpr T Interpolate(const T& Source, const T& Destination, const U& Position)
{
    return Source + static_cast<T>((Destination - Source) * Position);
}

//basic struct to pass data to EasedValue::Evaluate
struct EasingContext
{
    Function Function;
    EasingType Type;
    float Time;
    float Duration;
};

//will ease a value to match a target, keeping track of older but still active easing so that it is always smooth
template <typename T> struct EasedValue
{
private:
    struct Element
    {
        Element() {};
        Element(T val, float time) : m_Value(val), m_Time(time) {}

        T m_Value;
        float m_Time;
    };

public:

    explicit EasedValue(const T& value)
    {
        m_Queue.emplace_back(value, std::numeric_limits<float>::min());
    }

    void Reset(const T& value)
    {
        m_Queue.clear();
        m_Queue.emplace_back(value, std::numeric_limits<float>::min());
    }

    bool Push(const T& value, const float time)
    {
        if (m_Queue.back().m_Value != value)
        {
            m_Queue.emplace_back(value, time);
            return true;
        }
        return false;
    }

    void Evaluate(T& output, const EasingContext& context)
    {
        ASSERT(!m_Queue.empty(), "should always have a current target");

        output = m_Queue.front().m_Value;
        uint32_t outDatedValues = 0;

        for (uint32_t i = 0; i < m_Queue.size(); ++i)
        {
            Element& current = m_Queue[i];

            float timeSinceQueued = context.Time - current.m_Time;
            if (timeSinceQueued > context.Duration)
            {
                ++outDatedValues;
                output = current.m_Value;
            }
            else
            {
                const float weight = EvaluateEasing(context.Type, context.Function, timeSinceQueued, 0.f, 1.f, context.Duration);
                output = Interpolate(output, current.m_Value, weight);
            }
        }

        if (outDatedValues == m_Queue.size())
        {
            std::swap(m_Queue.front(), m_Queue.back());
            m_Queue.resize(1);
        }
        else if (outDatedValues > 1)
        {
            std::move(m_Queue.begin() + outDatedValues, m_Queue.end(), m_Queue.begin());
            m_Queue.resize(m_Queue.size() - outDatedValues);

            ASSERT(!m_Queue.empty(), "should always have a current target");
        }
    }

    T& Last()
    {
        ASSERT(!m_Queue.empty(), "should always have a current target");
        return m_Queue.back().m_Value;
    }

    const T& Last() const
    {
        ASSERT(!m_Queue.empty(), "should always have a current target");
        return m_Queue.back().m_Value;
    }

    bool IsInTransition() const { return m_Queue.size() > 1; }

private:
    std::vector<Element> m_Queue;
};
