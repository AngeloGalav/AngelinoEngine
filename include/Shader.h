#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

class Shader
{
    public:
        // the program ID
        unsigned int ID;

        // constructor reads and builds the shader from fragment and vertex
        Shader(const char* vertexPath, const char* fragmentPath);

        // use/activate the shader
        void use();

        // utility uniform functions
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setMat4(const std::string &name, glm::mat4 value) const;

    protected:

    private:
};

#endif // SHADER_H
