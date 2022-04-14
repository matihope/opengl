#include <iostream>
#include <fstream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    std::cout << "Resized to: " << width << " " << height << std::endl;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

uint compileShader(uint type, const std::string input){
    if(type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER){
        std::cerr << "ERROR::SHADER::INVALID_SHADER_TYPE\n";
        return -1;
    }

    // create and compile shader
    const char *shaderSource = input.c_str();
    uint shader;
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, 0);
    glCompileShader(shader);

    // check for compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

std::string readFile(const std::string file_location){
    std::fstream file(file_location, std::ios::in);
    std::string content;
    if(file.is_open()){
        std::string line;
        while(std::getline(file, line)){
            content += line + '\n';
        }
    } else {
        std::cerr << "Failed to open file " << file_location << std::endl;
    }
    file.close();
    return content;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[] = {
        -0.5f, -0.5f, -0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    uint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // handle shaders
    uint vertexShader = compileShader(GL_VERTEX_SHADER, readFile("src/vertex_shader.glsl"));
    uint fragmentShader = compileShader(GL_FRAGMENT_SHADER, readFile("src/fragment_shader.glsl"));

    uint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    } else {
        glUseProgram(shaderProgram);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}