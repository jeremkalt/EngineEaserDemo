#include "Easing.h"

float EvaluateEasing(EasingType type, Function func, float t, float b, float c, float d)
{
    switch (func)
    {
    case Linear:
    {
        return EaseLinear(t, b, c, d);
    }
    case Sinusoidal:
    {
        switch (type)
        {
        case EaseIn:
        {
            return EaseInSinusoidal(t, b, c, d);
        }
        case EaseOut:
        {
            return EaseOutSinusoidal(t, b, c, d);
        }
        case EaseInOut:
        {
            return EaseInOutSinusoidal(t, b, c, d);
        }
        }
    }
    case Quadratic:
    {
        switch (type)
        {
        case EaseIn:
        {
            return EaseInQuadratic(t, b, c, d);
        }
        case EaseOut:
        {
            return EaseOutQuadratic(t, b, c, d);
        }
        case EaseInOut:
        {
            return EaseInOutQuadratic(t, b, c, d);
        }
        }
    }
    }

    ASSERT(0, "Wrong easing type or function");
    return 0.f;
}
