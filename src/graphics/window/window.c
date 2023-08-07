#include "graphics/graphics.h"

void window_create(window_t *window)
{
    window->closed = true;
    window->aspect = window->size.x/window->size.y;
    
    window->fullscreen = false;
    window->minimized = false;
    window->cursor = true;

    if (!glfwInit())
    {
        printf(LOG_ERROR "[OPENGL]: failed to init GLFW!\n");
        exit(-1);
    }
    else
        printf(LOG_INFO "[OPENGL]: initialized GLFW!\n");
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window->glfw = glfwCreateWindow(window->size.x, window->size.y, window->title, NULL, NULL);

    glfwMakeContextCurrent(window->glfw);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {

        printf(LOG_ERROR "[OPENGL]: failed to init GLAD!\n");
        exit(-1);
    }
    else
    {
        printf(LOG_INFO "[OPENGL]: initialized GLAD!\n");
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

void window_update(window_t *window)
{
    glfwSwapBuffers(window->glfw);
    glfwPollEvents();

    if (glfwWindowShouldClose(window->glfw))
        window->closed = true;
    if (key_pressed(window, KEY_BACKSPACE))
        window->closed = true;
    int size_x, size_y;
    glfwGetWindowSize(window->glfw, &size_x, &size_y);
    window->size.x = size_x;
    window->size.y = size_y;
    window->aspect = window->size.x / window->size.y;

    if (key_pressed(window, KEY_ESCAPE) && window->cursor)
        glfwSetInputMode(window->glfw, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else if (key_pressed(window, KEY_ESCAPE) && !window->cursor)
        glfwSetInputMode(window->glfw, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    double x, y;
    glfwGetCursorPos(window->glfw, &x, &y);
    window->mouse_pos.x = x;
    window->mouse_pos.y = y;

    static float lf = 0.0f;
    float cf = (float)glfwGetTime();
    window->dt = cf - lf;
    lf = cf;

    //----FPS----//
    static float count;
    count++;
    static float average;
    if (count <= 100)
    {
        average += 1 / window->dt / 100;
    }
    else
    {
        char str[100];
        sprintf(str, "OKAPI -  FPS: %i", (int)average);
        glfwSetWindowTitle(window->glfw, str);
        average = 0;
        count = 0;
    }
}
void window_exit(window_t *window)
{
    glfwDestroyWindow(window->glfw);
    glfwTerminate();
}

// Input
bool key_pressed(window_t *window, key key)
{
    if (glfwGetKey(window->glfw, key) == GLFW_PRESS)
        return 1;
    return 0;
}

bool mouse_button_pressed(window_t *window, mouse_button button)
{
    if (glfwGetMouseButton(window->glfw, button) == GLFW_PRESS)
        return 1;
    return 0;
}
