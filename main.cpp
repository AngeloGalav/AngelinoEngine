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

    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

    //openGL compiles shaders at runtime

    float vertices[] = {        //vertices
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    /*
    GLFW Initialization
    */

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


    /*
    SHADERS COMPILATION
    */

    //vertex input shader
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //code added 3/11/2020
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //compile shader by binding to shaderID and compiling the source code
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);  //1 = number of strings passed as vertex source
    glCompileShader(vertexShader);


    //fragment shader compilation (18/11/2020)
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //Shader compilation debug and console

    int  successVertex;
    int  successFrag;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVertex);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFrag);

    if(!successVertex)
    {
        //works similarly to infoLog in unity
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else
    {
        std::cout << "VERTEX SHADER COMPILED SUCCESSFULLY!" << std::endl;
    }

    if(!successFrag)
    {
        //works similarly to infoLog in unity
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else
    {
        std::cout << "FRAGMENT SHADER COMPILED SUCCESSFULLY!" << std::endl;
    }

    double i=0;
    glViewport(0, 0, 800, 600); //sets size of the viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //calls the function whenever the framebuffer size (window size) is changed

    /*
    Shader Program and Shader Program Object
    */

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //previous written shaders attachment to shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram); //links shaders in the shader program


    int successShaderProgram;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successShaderProgram);
    if(!successShaderProgram) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog); //programinfolog instead of shaderinfolog
        std::cout << "ERROR::SHADER::PROGRAM::LINKING\n" << infoLog << std::endl;
    } else
    {
        std::cout << "SHADER PROGRAM LINKED SUCCESSFULLY!" << std::endl;
    }


    glUseProgram(shaderProgram);


    //deleting shaders after we have used it
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);





    /*
    WINDOW FUNCTIONS
    */

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
