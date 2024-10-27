#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>


int main()
{
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Gradient Triangle ", NULL, NULL);
    if(!window){ 
        const char* description;
        int code = glfwGetError(&description);
        fprintf(stderr, "Failed to create GLFW window: %s (Error Code: %d)\n", description, code);
        glfwTerminate();
        return -1; 
    }

    glfwMakeContextCurrent(window); 
    glewInit();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window); 
        glfwPollEvents();
    }

    glfwDestroyWindow(window); 
    glfwTerminate();

    return 0;
}