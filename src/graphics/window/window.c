#include "graphics/graphics.h"

void window_create(window_t* window)
{
    window->closed = true;
    window->aspect = (f32)window->width / (f32)window->height;

    window->minimized = false;

    if (!glfwInit())
    {
        printf(LOG_ERROR "[OPENGL]: failed to init GLFW!\n");
        exit(-1);
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if(window->resizable == false)
        glfwWindowHint(GLFW_RESIZABLE, 0);
    if(window->transparent)
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
#endif


    window->glfw = glfwCreateWindow(window->width, window->height, window->title, NULL, NULL);

    glfwMakeContextCurrent(window->glfw);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {

        printf(LOG_ERROR "[OPENGL]: failed to init GLAD!\n");
        exit(-1);
    }


    if (window == NULL)
        printf("[ERROR]: Failed to create GLFW window!\n");
    else
        window->closed = 0;

    glfwSwapInterval(window->vsync);

    if (!window->cursor)
    {
        glfwSetInputMode(window->glfw, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void window_update(window_t* window)
{
    glfwSwapBuffers(window->glfw);
    glfwPollEvents();

    if (glfwWindowShouldClose(window->glfw))
        window->closed = true;
    if (key_being_pressed(window, KEY_BACKSPACE))
        window->closed = true;
    i32 size_x, size_y;
    glfwGetWindowSize(window->glfw, &window->width, &window->height);

    window->aspect = (float)window->width / (float)window->height;

    if (window->cursor)
        glfwSetInputMode(window->glfw, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else
        glfwSetInputMode(window->glfw, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    f64 x, y;
    glfwGetCursorPos(window->glfw, &x, &y);
    window->mouse_pos.x = x;
    window->mouse_pos.y = y;


    //----Delta-Time----//
    static f32 lf = 0.0f;
    f32 cf = (f32)glfwGetTime();
    window->dt = cf - lf;
    lf = cf;

    //----FPS----//
    static f32 count;
    count++;
    static f32 average;
    if (count <= 100)
    {
        average += 1 / window->dt / 100;
    }
    else
    {
        char str[100];
        sprintf(str, "%s -  FPS: %i", window->title, (int)average);
        glfwSetWindowTitle(window->glfw, str);
        average = 0;
        count = 0;
    }
}
void window_exit(window_t* window)
{
    glfwDestroyWindow(window->glfw);
    glfwTerminate();
}

// Input
bool key_being_pressed(window_t* window, key key)
{
    if (glfwGetKey(window->glfw, key) == GLFW_PRESS)
        return true;
    return false;
}

bool mouse_button_pressed(window_t* window, mouse_button button)
{
    if (glfwGetMouseButton(window->glfw, button) == GLFW_PRESS)
        return true;
    return false;
}

bool key_pressed(window_t* window, key key)
{
    /* static bool state;
    if (glfwGetKey(window->glfw, key) == GLFW_PRESS && state == false)
    {
        state = true;
        return true;
    }

    if (glfwGetKey(window->glfw, key) == GLFW_RELEASE)
        state = false;
*/
    return false;
}