#version 330 core
// The inputs will be fed by the vertex buffer objects
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

// this will be passed to fragment shader
out vec2 FragTexCoord;

//matrices as program attributes
uniform mat4 rotation;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    //Copy the texture coordinates to fragment shader
    FragTexCoord = texCoord;

    //Apply transformation matrix using homogeneous coordinates
    vec4 transformedPosition = projection * view * rotation * vec4(position, 1.0);

    //Calculate the final position on screen, this needs to be a 3D homogenous vector
    gl_Position = transformedPosition;
}
