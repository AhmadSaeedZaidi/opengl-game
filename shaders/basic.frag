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
    vec4 texColor = vec4(1.0, 1.0, 1.0, 1.0);
    
    if (TexID < 0.5) {
        // Side faces
        if (hasSidesTexture) {
            texColor = texture(sidesTexture, TexCoord);
        }
    } else {
        // Cap faces
        if (hasCapsTexture) {
            texColor = texture(capsTexture, TexCoord);
        }
    }
    
    FragColor = texColor;
}