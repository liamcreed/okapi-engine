#include "math/math.h"

f32 f32_max(f32 f1, f32 f2)
{
    if (f1 > f2)
        return f1;
    return f2;
}

f32 lerp(f32 a, f32 b, f32 f)
{
    return (a * (1.0f - f)) + (b * f);
}

f32 deg_to_rad(f32 deg)
{
    return (f32)(deg * (M_PI / 180.0f));
}

f32 f32_random(f32 min, f32 max)
{
    return min + (max - min) * rand() / RAND_MAX;

}

f32 f32_clamp(f32 f, f32 min, f32 max)
{
    if (f < min) return min;
    if (f > max) return max;
    return f;
}

