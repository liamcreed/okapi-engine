#include "graphics/graphics.h"

float lf = 0.0f;

float scroll;
float last_scroll;
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    scroll = yoffset;
    if (scroll == last_scroll)
        scroll = 0;
    last_scroll = yoffset;
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width*2, height*2);
}

void window_create(window_t *window, vec2_t size, const char *title, bool vsync)
{
    window->closed = 1;
    window->size = size;
    window->title = title;
    window->vsync = vsync;
    window->fullscreen = 0;
    window->minimized = 0;

    if (!glfwInit())
    {
        printf(LOG_ERROR "[OPENGL]: failed to init GLFW!\n");
        exit(-1);
    }
    else
        printf(LOG_INFO "[OPENGL]: initialized GLFW!\n");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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

    glfwSetScrollCallback(window->glfw, scroll_callback);
    glfwSetFramebufferSizeCallback(window->glfw, framebuffer_size_callback);

}

void window_update(window_t *window)
{
    glfwSwapBuffers(window->glfw);
    glfwPollEvents();

    if (glfwWindowShouldClose(window->glfw))
        window->closed = true;
    if (key_pressed(window, KEY_BACKSPACE))
        window->closed = true;
    glfwGetWindowSize(window->glfw, (int *)&window->size.x, (int *)&window->size.y);

    float cf = (float)glfwGetTime();
    window->dt = cf - lf;
    lf = cf;

    if (key_pressed(window, KEY_ESCAPE) && window->cursor)
        glfwSetInputMode(window->glfw, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else if (key_pressed(window, KEY_ESCAPE) && !window->cursor)
        glfwSetInputMode(window->glfw, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    double x, y;
    glfwGetCursorPos(window->glfw, &x, &y);
    window->mouse_pos.x = x;
    window->mouse_pos.y = y;

    window->scroll = scroll;

    window->aspect = window->size.x / window->size.y;

    static vec2_t last_size;
    if (last_size.x != window->size.x && !window->resized|| last_size.y != window->size.y && !window->resized)
    {
        window->resized = true;
    }else
    {
         window->resized = false;
    }

    last_size = window->size;
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
