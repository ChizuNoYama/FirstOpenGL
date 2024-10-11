#version 330
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoords;

out vec3 ourColor;
out vec2 texCoords;

void main()
{
    gl_Position = vec4(inPosition.x, inPosition.y, inPosition.z, 1.0);
    ourColor = inColor;
    texCoords = inTexCoords;
}