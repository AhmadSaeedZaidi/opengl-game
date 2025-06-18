#version 330 core

in vec2 TexCoord;
in float TexID;

out vec4 FragColor;

uniform sampler2D sidesTexture;
uniform sampler2D capsTexture;
uniform bool hasSidesTexture;
uniform bool hasCapsTexture;

void main()
{
    if (TexID < 0.5) {
        // Side faces (cylinder)
        if (hasSidesTexture) {
            FragColor = texture(sidesTexture, TexCoord);
        } else {
            FragColor = vec4(0.8, 0.6, 0.4, 1.0); // Brown fallback for sides
        }
    } else {
        // Cap faces (cylinder) OR Sphere - both use capsTexture now
        if (hasCapsTexture) {
            FragColor = texture(capsTexture, TexCoord);
        } else {
            FragColor = vec4(0.6, 0.4, 0.2, 1.0); // Darker brown fallback
        }
    }
}