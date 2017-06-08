

#include <iostream>
#include <vector>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include "shader.h"
#include "mesh.h"
#include <ppgso6week_vert.h>
#include <ppgso6week_frag.h>





class Shape{
public:
    glm::vec2 position = {0,0};
    glm::vec2 scale = {1,1};
    float rotate = 0;
    void render(Shader &program);
    void transform();
    void Move();
    Shape(Shader &program);
private:
    GLuint vao;
    glm::mat3 model_matrix;
    std::vector<glm::vec2> points = {//dokonci
            {0.2f, 0.2f},
            {0.2f, -0.4f},
            {-0.4f, -0.4f},

            {-0.2f, -0.2f},
            {-0.2f, 0.4f},
            {0.4f, 0.4f}
    };
    std::vector<GLuint> indicies = {
            0, 1, 2,
            3, 4, 5
    };
};

Shape::Shape(Shader &program){
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // Generate a vertex buffer object, this will feed data to the vertex shader
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec2),points.data(), GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(GLuint),indicies.data(), GL_STATIC_DRAW);

    // Setup vertex array lookup, this tells the shader how to pick data for the "Position" input
    auto position_attrib = program.GetAttribLocation("Position");
    glVertexAttribPointer(position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(position_attrib);

}



void Shape::transform(){
    float time = glfwGetTime();
    rotate = time;
    // identity
    model_matrix = glm::translate(glm::mat3(), position) * glm::scale(glm::mat3(), scale) * glm::rotate(glm::mat3(), rotate);
}

void Shape::Move(){
    model_matrix = glm::translate(glm::mat3(), position) * glm::scale(glm::mat3(), scale) * glm::rotate(glm::mat3(), rotate);
}

void Shape::render(Shader &program){
    program.Use();
    program.SetMatrix(model_matrix, "Transform");
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLuint) indicies.size(), GL_UNSIGNED_INT, 0);
}


int main(){
    //Inicialization GLFW
    if(!glfwInit()){
        std::cerr << "Flailed to inicialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    //Setup opengl context
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "PPGSO6WEEK", nullptr, nullptr);
    if(window == nullptr){
        std::cerr << "Failed to create GLFW window " << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    //Inicialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    if(!glewIsSupported("GL_VERSION_3_3")){
        std::cerr << "Failed to inicialize GLEW with OpenGL 3.3!" << std::endl;
        return EXIT_FAILURE;
    }

    auto program = Shader{ppgso6week_vert, ppgso6week_frag};
    Shape shape=Shape(program);

    GLfloat time = 0;
    //
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        // Set gray background
        glClearColor(0.6f,0.2f,1.0f,0);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //rychlost transformacie - mensie cisla = mensia rychlost
        //SetTransformation(program, time+=0.001f);




        // ------------------------------------------------
        shape.position={sin(time)/1.5,cos(time)/1.5};
        shape.rotate=10*(sin(time)-cos(time));
        shape.scale={(sin(time)/3),cos(time)/3};
        shape.Move();
        time += 0.005f;
        shape.render(program);
        // ------------------------------------------------

        //  glDrawArrays(GL_TRIANGLES, 0, (GLuint) vertex_buffer.size());
        // Display result
        glfwSwapBuffers(window);

    }



}



