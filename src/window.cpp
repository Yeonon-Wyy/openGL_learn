#include "window.h"
#include "log.h"
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

Window::Window()
    : Window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)
{ }

Window::Window(float width, float height)
    : m_width(width)
    , m_height(height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_width, m_height, "learn-openGL", nullptr, nullptr);
    if(m_window == nullptr)
    {
        glfwTerminate();
        GL_LOG_E("failed to init gl window");
        std::abort();
    }

    glfwMakeContextCurrent(m_window);
    gladLoadGL();
}

Window::~Window()
{
    if(m_window)
    {
        // release
        GL_LOG_D("release window");
        glfwTerminate();
    }
}

GLFWwindow* Window::glfwWindow() const
{
    return m_window;
}

void Window::setFrameBufferSizeCallback(FrameBufferSizeCallbackFunc cb)
{
    glfwSetFramebufferSizeCallback(m_window, cb);
}

void Window::setKeyCallback(KeyCallbackFunc cb)
{
    glfwSetKeyCallback(m_window, cb);
}

void Window::setMouseCallback(MouseCallback cb)
{
    glfwSetCursorPosCallback(m_window, cb);
}
void Window::setScrollCallback(ScrollCallback cb)
{
    glfwSetScrollCallback(m_window, cb);
}