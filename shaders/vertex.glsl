#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 0) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 ourColor;   // output a color to the fragment shader
out vec2 TexCoord;

uniform mat4 transform;

// coordinate matrices, set up in main script
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = aTexCoord;
}