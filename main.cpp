#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
        glViewport(0, 0, width, height);
}
int main()
{
        if (!glfwInit())
        {
                std::cerr << "Failed to initialize GLFW" << std::endl;
                return -1;
        }

        GLFWwindow *window = glfwCreateWindow(800, 600, "AR Application", nullptr, nullptr);

        if (!window)
        {
                std::cerr << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                return -1;
        }

        glfwMakeContextCurrent(window);

        if (glewInit() != GLEW_OK)
        {
                std::cerr << "Failed to initialize GLEW " << std::endl;
                glfwTerminate();
                return -1;
        }

        glViewport(0, 0, 800, 600);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        while (!glfwWindowShouldClose(window))
        {
                glClear(GL_COLOR_BUFFER_BIT);
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glfwSwapBuffers(window);
                glfwPollEvents();
        }
        glfwTerminate();
        return 0;
}
