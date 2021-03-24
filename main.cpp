#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

const unsigned int wHeight = 600;
const unsigned int wWidth = 800;

bool wireframe = false;


void framebuffer_size_callback(GLFWwindow* window, int width, int height) //when windows is resized, call this function
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
    /**
        SHADER SOURCE CODE
    */

    //vertex shader code
    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor; // output a color to the fragment shader\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";

    //added uniform values to shader
    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec3 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\0";

    //openGL compiles shaders at runtime

    /* float vertices[] = {        //vertices
        0.5f, 0.5f, 0.0f,
        0.5f,-0.5f, 0.0f,
       -0.5f,-0.5f, 0.0f,
       -0.5f, 0.5f, 0.0f
    };
    */

    float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };

    unsigned int indices[] ={
       /* 0,1,3, //first triangle
        1,2,3 //second triangle
        */
        0,1,2

    };

    /**
    GLFW Initialization
    */

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //this tells us that we are using version 3.3 of glfw
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    GLFWwindow* window = glfwCreateWindow(wWidth, wHeight, "CoolWindow", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //calls the function whenever the framebuffer size (window size) is changed

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    /**
    VERTEX BUFFER OBJECT & VERTEX INTERPRETATION
    */

    //vertex input shader
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    //vertex array object generation
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  //we can also generate multiple VAO and VBOs at the same time using an array
    glBindVertexArray(VAO);

    //vbo generation
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //element buffer object so we can use more triangles for a single element and specify the order of what to use
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);


    //telling OpenGL how to interpret the Vertex values (related to VBOs)

    //vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0); //0 is the location of our vertices

    //color attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*) (3*sizeof(float)));
    glEnableVertexAttribArray(1);

    /**
    SHADERS COMPILATION
    */

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

    //debug and console for shader compilation
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


    /**
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


    /**
    WINDOW FUNCTIONS
    */

    double i=0;

    if (wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //WIREFRAME MODE ON
    }


    while(!glfwWindowShouldClose(window)) //checks if window is closed
    {
        //i+=0.01;
        if (i > 1) { i = 0; }
        processInput(window);
        //std::cout << i << std::endl;
        glClearColor(0.2f, i, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); //we clear the color buffer


        float timeValue = glfwGetTime(); //to change value of color using our time
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); //not used since we're not using any uniform type data
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); //not used since we're not using any uniform type data


        glBindVertexArray(VAO); //binding to a vao automatically also binds every vbos or ebos in it
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) 0);

        //glDrawArrays(GL_TRIANGLES, 0, 3); //not used when using EBOs


        glfwSwapBuffers(window); // we are using double buffers to display the image to avoid flickeringS
        glfwPollEvents();        // checks if any events are triggered
    }



    //deleting shaders after we have used it
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    glfwTerminate(); //deallocates all resources
    return 0;
}
