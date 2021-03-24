#include "../include/Shader.h"
#define STB_IMAGE_IMPLEMENTATION //this header is used to compile only the functions that we need for image preprocessing.
#include "../include/image_loader/stb_image.h"
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int wHeight = 600;
const unsigned int wWidth = 800;

bool wireframe = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) //when windows is resized, call this function
{
    glViewport(0, 0, width, height);    //window size
}

void processInput(GLFWwindow *window) //pretty much self explanatory
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
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

    Shader shader_with_texture("shaders/vertex.glsl", "shaders/fragment.glsl");

    float vertices[] = {    //for every attribute that we add, we need to update the stride...
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    /*
        3   0
        2   1
    */
    unsigned int indices[] = {
        0,1,3, //first triangle
        1,2,3 //second triangle                     
    };

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0); //0 is the location of our vertices

    //color attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*) (3*sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture attributes added
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    /**
    WINDOW FUNCTIONS
    */
    if (wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //WIREFRAME MODE ON
    }

    /*
    Texture loading.
    */
    int width, height, nrChannels;
    unsigned char* data = stbi_load("textures_placeholder/container.jpg", &width, &height, &nrChannels, 0);
    stbi_set_flip_vertically_on_load(true);

    unsigned int texture, texture2;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //MIPMAPS Filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture. File not found." << std::endl;
    }

    stbi_image_free(data);  //free data after texture generation.

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("textures_placeholder/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    shader_with_texture.use(); // don't forget to activate/use the shader before setting uniforms!

    // either set it manually like so:
    glUniform1i(glGetUniformLocation(shader_with_texture.ID, "texture1"), 0);
    // or set it via the texture class
    shader_with_texture.setInt("texture2", 1);


    while(!glfwWindowShouldClose(window)) //checks if window is closed
    {
        processInput(window);
    
        glClearColor(0.2f, 0, 0.3f, 1.0f); //background color.
        glClear(GL_COLOR_BUFFER_BIT); //we clear the color buffer

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);


        /*
            rotation over time:
        */
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        shader_with_texture.use();


        //code for activating the rotating and passing it onto the shader
        unsigned int transformLoc = glGetUniformLocation(shader_with_texture.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));


        glBindVertexArray(VAO); //binding to a vao automatically also binds every vbos or ebos in it
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) 0);

        glfwSwapBuffers(window); // we are using double buffers to display the image to avoid flickeringS
        glfwPollEvents();        // checks if any events are triggered
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate(); //deallocates all resources
    return 0;
}
