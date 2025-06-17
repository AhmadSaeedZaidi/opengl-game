#version 330 core

in vec2 TexCoord;
in float TexID;

out vec4 FragColor;

uniform sampler2D sidesTexture;
uniform sampler2D capsTexture;
uniform sampler2D sphereTexture;
uniform bool hasSidesTexture;
uniform bool hasCapsTexture;
uniform bool hasTexture;

void main()
{
    if (TexID < 0.5) {
        // Side faces (cylinder)
        if (hasSidesTexture) {
            FragColor = texture(sidesTexture, TexCoord);
        } else {
            FragColor = vec4(0.8, 0.6, 0.4, 1.0); // Brown fallback for sides
        }
    } else if (TexID < 1.5) {
        // Cap faces (cylinder)
        if (hasCapsTexture) {
            FragColor = texture(capsTexture, TexCoord);
        } else {
            FragColor = vec4(0.6, 0.4, 0.2, 1.0); // Darker brown fallback for caps
        }
    } else {
        // Sphere
        if (hasTexture) {
            FragColor = texture(sphereTexture, TexCoord);
        } else {
            FragColor = vec4(0.7, 0.7, 0.9, 1.0); // Light blue fallback for spheres
        }
    }
}