#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define OPENGL_ERROR   "\033[31m[OPENGL_ERROR]: \033[0m"
#define LOG_ERROR   "\033[31m[ERROR]: \033[0m"
#define LOG_WARNING "\033[33m[WARNING]: \033[0m"
#define LOG_INFO    "\033[32m[INFO]: \033[0m"

#define LOG_GLTF "\033[35m[GLTF]: \033[0m"

typedef float f32;
typedef double f64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;


#include "math/math.h"
#include "tools/tools.h"
