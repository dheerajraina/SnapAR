#include <opencv2/opencv.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
        glViewport(0, 0, width, height);
}

GLuint createTexture(int width, int height)
{
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // space allocation for the texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        return textureID;
}

void updateTexture(GLuint textureID, cv::Mat &frame)
{
        // OpenGL expects the image to be flipped vertically
        cv::flip(frame, frame, 0);

        glBindTexture(GL_TEXTURE_2D, textureID);

        // texture update with the new frame data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
        glGenerateMipmap(GL_TEXTURE_2D);
}

int main()
{
        // Initialize GLFW
        if (!glfwInit())
        {
                std::cerr << "Failed to initialize GLFW" << std::endl;
                return -1;
        }

        // Create a GLFW window
        GLFWwindow *window = glfwCreateWindow(800, 600, "AR Application", nullptr, nullptr);
        if (!window)
        {
                std::cerr << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                return -1;
        }

        // Make the window context current
        glfwMakeContextCurrent(window);

        // Initialize GLEW
        if (glewInit() != GLEW_OK)
        {
                std::cerr << "Failed to initialize GLEW" << std::endl;
                return -1;
        }

        // Set the viewport
        glViewport(0, 0, 800, 600);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // Initialize OpenCV
        cv::VideoCapture cap(0); // Capture from the default camera
        if (!cap.isOpened())
        {
                std::cerr << "Error: Could not open camera" << std::endl;
                return -1;
        }

        // Create a texture for the camera feed
        GLuint textureID = createTexture(800, 600);

        // Main render loop
        while (!glfwWindowShouldClose(window))
        {
                // Capture frame from camera
                cv::Mat frame;
                cap >> frame;
                if (frame.empty())
                        break;

                // Update the OpenGL texture with the new frame
                updateTexture(textureID, frame);

                // Clear the screen
                glClear(GL_COLOR_BUFFER_BIT);

                // Render the camera feed as a texture
                glEnable(GL_TEXTURE_2D); // Enable 2D textures
                glBindTexture(GL_TEXTURE_2D, textureID);

                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(-1.0f, -1.0f); // Bottom-left
                glTexCoord2f(1.0f, 0.0f);
                glVertex2f(1.0f, -1.0f); // Bottom-right
                glTexCoord2f(1.0f, 1.0f);
                glVertex2f(1.0f, 1.0f); // Top-right
                glTexCoord2f(0.0f, 1.0f);
                glVertex2f(-1.0f, 1.0f); // Top-left
                glEnd();

                // Swap buffers
                glfwSwapBuffers(window);
                // Poll for events
                glfwPollEvents();
        }

        // Release the camera
        cap.release();

        // Terminate GLFW
        glfwTerminate();
        return 0;
}
