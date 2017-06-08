// Example gl_transform
// - This example demonstrates basic 2D transformations
// - Notice that the glm library uses column major order when declaring matrices
// - Try to combilne multiple transformation using multiplication

#include <iostream>
#include <vector>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "mesh.h"
#include "ppgso7week_vert.h"
#include "ppgso7week_frag.h"


const unsigned int SIZE = 1024;

using point = glm::vec3;

point lerp(point p,point p2,float t)
{
    return p*(1-t) + p2*t;
}

point bezier (point *p,float t){

    point p01 = lerp(p[0],p[1],t);
    point p12 = lerp(p[1],p[2],t);
    point p23 = lerp(p[2],p[3],t);

    point p01_12 = lerp(p01,p12,t);
    point p12_23 = lerp(p12,p23,t);

    return lerp(p01_12,p12_23,t);
}


point BezierSurface(point p[4][4],float v,float u)
{
    point temp[4];

    for(int y = 0;y<4;y++)
    {
        temp[y] = bezier(p[y],v);
    }

    return bezier(temp,u);
}




int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return EXIT_FAILURE;
    }

    glm::vec3 body[4][4] = {
            {{-10.0f, 5.25f, 9.1f}, {-10.5f, 1.5f, 0.5f}, {1.5f, 1.5f, -0.6f}, {2.0f, 1.25f, 0.0f}},
            {{-10.0f, 0.0f, 10.2f},  {-10.5f, 1.25f, 5.6f}, {1.5f, 1.25f, -1.0f}, {2.0f, 1.0f, 0.0f}},
            {{-10.0f, -3.25f, -3.3f}, {-10.5f, 1.0f, -8.8f}, {1.5f, 1.0f, -0.3f}, {2.0f, -1.25f, 0.0f}},
            {{-10.0f, -15.5f, 0.1}, {-10.5f, -1.25f, 2.1f}, {1.5f, -1.25f, 0.5f}, {2.0f,-1.5f, 0.0f}}
    };


    // Setup OpenGL context
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Try to create a window
    auto window = glfwCreateWindow( SIZE, SIZE, "PPGSO gl_transform", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to open GLFW window, your graphics card is probably only capable of OpenGL 2.1" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Finalize window setup
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    if (!glewIsSupported("GL_VERSION_3_3")) {
        std::cerr << "Failed to initialize GLEW with OpenGL 3.3!" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Load shaders
    auto program = ShaderPtr(new Shader{ppgso7week_vert, ppgso7week_frag});
    program->Use();

    auto texture = Texture{"lena.rgb", 512, 512};
    auto quad = Mesh{program, "quad.obj"};

    float time = 0;

    float dV = 0.01f;
    float dU = 0.01f;
    GLushort indices = 0;

    //point -> glm::vec3
    std::vector<point> gen_points;
    std::vector<glm::vec2> texture_coords;
    std::vector<GLushort> indexes;

    for(float U = 0.f; U <= (1.0f - dU);U+=dU)
    {
        for(float V = 0.f; V <= (1.0f - dV);V+=dV)
        {
            point p00 = BezierSurface(body,U,V);
            point p10 = BezierSurface(body,U + dU,V);
            point p01 = BezierSurface(body,U,V + dV);
            point p11 = BezierSurface(body,U + dU,V + dV);


            gen_points.push_back(p00);
            texture_coords.push_back(glm::vec2(U,V));


            gen_points.push_back(p10);
            texture_coords.push_back(glm::vec2(U + dU,V));


            gen_points.push_back(p01);
            texture_coords.push_back(glm::vec2(U,V + dV));


            gen_points.push_back(p11);
            texture_coords.push_back(glm::vec2(U + dU,V + dV));


            indexes.push_back(indices*4);
            indexes.push_back(indices*4 + 1);
            indexes.push_back(indices*4 + 2);

            indexes.push_back(indices*4 + 1);
            indexes.push_back(indices*4 + 3);
            indexes.push_back(indices*4 + 2);



            indices++;
        }
    }

    GLuint VAO;

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    GLuint VTO;
    GLuint IBO;

    glGenBuffers(1,&VBO);
    glGenBuffers(1,&VTO);
    glGenBuffers(1,&IBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, gen_points.size() * 3*sizeof(GLfloat),&gen_points[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VTO);
    glBufferData(GL_ARRAY_BUFFER, texture_coords.size() * sizeof(glm::vec2),&texture_coords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,2*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() *sizeof(GLushort),&indexes[0], GL_STATIC_DRAW);


    glm::mat4 projection = glm::perspective(10.0f,1.0f,10.0f,1000.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.f,0.6f,16.0f),glm::vec3(0.0f,0.2f,0.0f),glm::vec3(0.f,1.f,0.f));
    glm::mat4 rotation = glm::mat4(1.0f);

    glBindTexture(GL_TEXTURE_2D, texture.GetTexture());
    glActiveTexture(GL_TEXTURE0);

    glEnable(GL_DEPTH_TEST);

    // Main execution loop
    while (!glfwWindowShouldClose(window)) {
        // Set gray background
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //SetTransformation(program, time+=0.01f);
        rotation = glm::rotate(rotation,0.01f,glm::vec3(1.5f,0.2f,0.0f));

        glUniformMatrix4fv(glGetUniformLocation(program->GetProgram(),"view"),1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(program->GetProgram(),"projection"),1,GL_FALSE,glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(program->GetProgram(),"rotation"),1,GL_FALSE,glm::value_ptr(rotation));

        glDrawElements(GL_TRIANGLES,indexes.size(),GL_UNSIGNED_SHORT,0);

        // Display result
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glfwTerminate();

    return EXIT_SUCCESS;
}
