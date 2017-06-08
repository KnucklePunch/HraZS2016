#version 150
// The final color
out vec4 FragmentColor;
in vec4 VertexColor;

void main() {
  // Lookup the color in Texture on coordinates given by fragTexCoord
  FragmentColor = VertexColor;

}
