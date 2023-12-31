#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


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

#define OPENGL_ERROR   "\033[31m[OPENGL_ERROR]: \033[0m"
#define LOG_ERROR   "\033[31m[ERROR]: \033[0m"
#define LOG_WARNING "\033[33m[WARNING]: \033[0m"
#define LOG_INFO    "\033[32m[INFO]: \033[0m"

#define LOG_GLTF "\033[35m[GLTF]: \033[0m"

static void GL_clear_error()
{
    if (!glGetError());
}
static bool GL_check_error()
{
    i32 error;
    if ((error = glGetError()))
    {
        const char* string;
        if(error == GL_INVALID_ENUM)
            string = "GL_INVALID_ENUM";
        else if(error == GL_INVALID_VALUE)
            string = "GL_INVALID_VALUE";
        else if(error == GL_INVALID_OPERATION)
            string = "GL_INVALID_OPERATION";
        else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
            string = "GL_INVALID_FRAMEBUFFER_OPERATION";
        else if(error == GL_OUT_OF_MEMORY)
            string = "GL_OUT_OF_MEMORY";
        else if(error == GL_STACK_UNDERFLOW)
            string = "GL_STACK_UNDERFLOW";
        else if(error == GL_STACK_OVERFLOW)
            string = "GL_STACK_OVERFLOW";
        printf(OPENGL_ERROR "%s\n",string);
        return true;
    }
    return false;
}
#define GL(x) GL_clear_error(); x; assert(!GL_check_error())

//#define GL(x) x;



#include "math/math.h"
#include "tools/tools.h"
