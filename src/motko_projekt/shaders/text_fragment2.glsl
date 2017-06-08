#version 150
// A texture is expected as program attribute
uniform sampler2D Texture;

// The vertex shader fill feed this input
in vec2 FragTexCoord;

// Wordspace normal
in vec4 normal;

// The final color
out vec4 FragmentColor;

void main() {

  vec2 modCoord = vec2(FragTexCoord[1],FragTexCoord[0]);

  // Lookup the color in Texture on coordinates given by fragTexCoord and apply diffuse lighting
  FragmentColor = texture(Texture, modCoord);
}
