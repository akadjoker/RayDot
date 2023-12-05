#pragma once


#include <raylib.h>
#include <rlgl.h>
#include <math.h>
#include <string>
#include <random>
#include <utility>
#include <memory>

#include <vector>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <memory>

#include <bitset>
#include <cstring>
#include <ctime>


#define CONSOLE_COLOR_RESET "\033[0m"
#define CONSOLE_COLOR_GREEN "\033[1;32m"
#define CONSOLE_COLOR_RED "\033[1;31m"
#define CONSOLE_COLOR_PURPLE "\033[1;35m"
#define CONSOLE_COLOR_CYAN "\033[0;36m"

const float RECIPROCAL_PI = 1.0f / PI;
const float HALF_PI = PI / 2.0f;
const float DEGTORAD = PI / 180.0f;
const float RADTODEG = 180.0f / PI;

#define PI_TIMES_TWO 6.28318530718f
#define PI2 PI * 2
#define DEG -180 / PI
#define RAD PI / -180
#define COLLIDE_MAX(a, b) ((a > b) ? a : b)
#define COLLIDE_MIN(a, b) ((a < b) ? a : b)

const bool FIX_ARTIFACTS_BY_STRECHING_TEXEL = true;

typedef struct rVertex
{
    float x, y, z;
    Color col;
    float tx, ty;

} rVertex;

typedef struct rQuad
{
    rVertex v[4];
    Texture2D tex;
    int blend;
} rQuad;

void Log(int severity, const char *fmt, ...);



