#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

layout (location = 0) out vec2 vertexTexCoord;

layout (location = 0) uniform mat4 M;
layout (location = 1) uniform mat4 V;
layout (location = 2) uniform mat4 P;

void main()
{
	gl_Position = P * V * M * vec4(position, 1.0f);
	vertexTexCoord = texCoord;
}