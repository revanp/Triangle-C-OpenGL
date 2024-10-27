#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

const char* readSource(const char* filename)
{
    FILE* file = fopen(filename, "rb");

    if(!file){
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);

    fseek(file, 0, SEEK_SET);
    char* source = (char*) malloc(length + 1);

    fread(source, 1, length, file);
    fclose(file);

    source[length] = '\0';

    return source;
}

GLuint compileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint createShaderProgram()
{
    const char* vertexSource = readSource("vertex_shader.glsl");
    const char* fragmentSource = readSource("fragment_shader.glsl");

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    free((void*)vertexSource);
    free((void*)fragmentSource);
    return program;
}

int main()
{
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Gradient Triangle 😜", NULL, NULL);
    if(!window){ 
        const char* description;
        int code = glfwGetError(&description);
        fprintf(stderr, "Failed to create GLFW window: %s (Error Code: %d)\n", description, code);
        glfwTerminate();
        return -1; 
    }

    glfwMakeContextCurrent(window); 

    glewExperimental = GL_TRUE;
    glewInit();

    GLuint program = createShaderProgram();
    glUseProgram(program);

    GLfloat vertices[] = {
        // Position         // Color
        0.0f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,  2.0f, 0.0f, 1.0f
    };

    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLint posAttrib = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    GLint colAttrib = glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window); 
        glfwPollEvents();
    }

    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(window); 
    glfwTerminate();

    return 0;
}