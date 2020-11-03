#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) //pretty much self explanatory
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    //readfile code here

    //vertex shader code
    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    //openGL compiles shaders at runtime

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //this tells us that we are using version 3.3 of glfw
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //code added 28/10/2020
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //code added 3/11/2020
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //compile shader by binding to shaderID and compiling the source code
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    //check if compilazion is successfull
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        //works similarly to infoLog in unity
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else
    {
        std::cout << "SHADER COMPILER SUCCESSFULLY!" << std::endl;
    }

    double i=0;
    glViewport(0, 0, 800, 600); //sets size of the viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //calls the function whenever the framebuffer size (window size) is changed

    while(!glfwWindowShouldClose(window)) //checks if window is closed
    {
        i+=0.01;
        if (i > 1) { i = 0; }
        processInput(window);
        //std::cout << i << std::endl;
        glClearColor(0.2f, i, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); //we clear the color buffer

        glfwSwapBuffers(window); // we are using double buffers to display the image to avoid flickeringS
        glfwPollEvents();        // checks if any events are triggered
    }

    glfwTerminate(); //deallocates all resources
    return 0;
}