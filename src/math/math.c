#include "math/math.h"

f32 f32_max(f32 f1, f32 f2)
{
    if (f1 > f2)
        return f1;
    return f2;
}

f32 lerp(f32 a, f32 b, f32 f)
{
    return (a * (1.0 - f)) + (b * f);
}

f32 radians(f32 deg)
{
    return (f32)(deg * (M_PI / 180.0));
}
