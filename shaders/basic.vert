#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out float TexID;

void main()
{
    TexCoord = aTexCoord;

    // For cylinders: X-axis normals = caps, Y/Z-axis normals = sides
    // For spheres: use TexID = 2.0 to indicate sphere texture
    TexID = (abs(aNormal.x) > 0.9) ? 1.0 : 0.0;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}