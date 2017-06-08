#version 150
// A texture is expected as program attribute
uniform sampler2D Texture;

// Lights
uniform int LightsCount = 0;
uniform vec3[10] Lights;
uniform vec3 Camera;

// The vertex shader fill feed this input
in vec2 FragTexCoord;

in vec3 PixelCoord;

// Wordspace normal
in vec4 normal;

// The final color
out vec4 FragmentColor;

// Light direction in world coordinates
vec4 lightDirection = normalize(vec4(0.0f,0.5f,0.0f,0.0f));

void main() {
  // Compute diffuse lighting
  float diffuse = max(dot(normal, lightDirection), 0.0f);
  float specular = 0;

  for (int i = 0; i < LightsCount; i++) {
     vec4 lightDir = normalize(vec4(Lights[i] - PixelCoord,0.0f));
     float this_diffuse = max(dot(normal, lightDir),0.0f);
     diffuse += this_diffuse;

     if (this_diffuse > 0.0f) {
         vec3 h = normalize(vec3(lightDir) + Camera);
         float intSpec = max(dot(h,vec3(normal)), 0.0);
         specular += pow(intSpec,3);
     }
  }

  diffuse /= 4;

  float ambient = 0.3 + LightsCount * 0.05;

  float lightCompound = diffuse + ambient * 1.5 + specular;
  lightCompound = lightCompound / 3;

  // Lookup the color in Texture on coordinates given by fragTexCoord and apply diffuse lighting
  FragmentColor = texture(Texture, FragTexCoord) * lightCompound ;
}
