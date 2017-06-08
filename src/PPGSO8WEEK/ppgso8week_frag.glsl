#version 330 core
// the vertex shader will feed this input
in vec2 FragTexCoord;

//The final color
out vec4 color;

// A texture is expected as program attribute
uniform sampler2D ourTexture1;


void main()
{
    // Lookup the color in Texture on coordinates given by fragTexCoord
    color = texture(ourTexture1, FragTexCoord);
}