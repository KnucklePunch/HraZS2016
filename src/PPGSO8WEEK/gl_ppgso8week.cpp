//
// Created by HORIZON on 9. 11. 2016.
//

#include <iostream>
#include <vector>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

// Other includes
#include "Shader.h"
#include "Texture.h"
#include <ppgso8week_vert.h>
#include <ppgso8week_frag.h>

#include <omp.h>

float zoom = 10.0f;
float tilt = 0.0f;
bool keys[1044];
class Shape{
public:
    void inicialization();

    glm::vec3 BrezierSurface( float u, float v);

    glm::vec3 lerp(glm::vec3 P0, glm::vec3 P1, float t);

    glm::vec3 cubicBrezier(glm::vec3 CP[4], float t);

    void generate_Control_Points();

    void alter_Control_Points(double offset);

    void generate_Points();

    Shape();

    void SetTexture(Texture texture);
    Texture textureID=Texture(512,512);

    std::vector<glm::vec3> Generovane;
    std::vector<glm::vec2> TextCoord;
    std::vector<GLuint> Indexy;

    GLuint vao,ibo, vbo, vto;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 rotation;
    /*
    glm::vec3 points[4][4] = {
            {{-4.5f, 3.25f, 0.1f}, {-4.45f, 3.5f, 2.0f}, {4.45f, 3.5f, -4.0f}, {4.5f, 3.25f, 5.0f}},
            {{-4.5f, 3.0f, 0.2f},  {-4.45f, 3.25f, -1.0f}, {4.45f, 3.25f, 3.0f}, {4.5f, 3.0f, -2.0f}},
            {{-4.5f, -3.25f, 0.3f}, {-4.45f, 3.0f, 1.8f}, {4.45f, 3.0f, -2.0f}, {4.5f, -3.25f, -2.0f}},
            {{-4.5f, -3.5f, 0.1}, {-4.45f, -3.25f, 0.2f}, {4.45f, -3.25f, 3.0f}, {4.5f,-3.5f, 0.0f}}
    };
    */
    glm::vec3 points[4][4];
    bool points_orientation[4][4];

    void render(ShaderPtr program);
};


Shape::Shape()
{

}

void Shape::generate_Points() {
    Generovane.clear();
    TextCoord.clear();
    Indexy.clear();

    GLuint indeces = 0;
    float uk = 0.05f;
    float vk = 0.05f;
    for (float u = 0; u < (1.0f - uk); u += uk)
    {
        for (float v = 0; v <(1.0f - vk); v += vk)
        {
            //u,v   uk+u,v   u,vk+v    u+uk,v+vk
            //text u,v  u+uk,v  u,v+vk
            //text2 u+uk,v uk+u,vk+v,    u,vk+v

            glm::vec3 P00 = BrezierSurface( u, v);
            glm::vec3 P01 = BrezierSurface( u + uk, v);
            glm::vec3 P10 = BrezierSurface( u, v + vk);
            glm::vec3 P11 = BrezierSurface( u + uk, v + vk);

            //first triangle
            Generovane.push_back(P00);
            TextCoord.push_back(glm::vec2(u, v));
            Generovane.push_back(P10);
            TextCoord.push_back(glm::vec2(u, v + vk));
            Generovane.push_back(P01);
            TextCoord.push_back(glm::vec2(u + uk, v));
            Generovane.push_back(P11);
            TextCoord.push_back(glm::vec2(u + uk, v + vk));

            Indexy.push_back(indeces * 4);
            Indexy.push_back(indeces * 4 + 1);
            Indexy.push_back(indeces * 4 + 2);

            Indexy.push_back(indeces * 4 + 1);
            Indexy.push_back(indeces * 4 + 3);
            Indexy.push_back(indeces * 4 + 2);

            indeces++;
        }
    }

    //std::cout << indeces << "\n";
}


void Shape::generate_Control_Points() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                points[i][j] = glm::vec3(float(i - 2), float(j - 2), ((double) rand() / (RAND_MAX)));
                if (i < 2) { points_orientation[i][j] = true; }
                else { points_orientation[i][j] = false; }
            }

        }

}

void Shape::alter_Control_Points(double offset){
    std::cout << offset << "\n";
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if(points[i][j].z >= 1){points_orientation[i][j] = false;}
            if(points[i][j].z <= -1){points_orientation[i][j] = true;}

            if(points_orientation[i][j]){points[i][j] = glm::vec3(points[i][j].x , points[i][j].y, points[i][j].z+0.01);}
            else{points[i][j] = glm::vec3(points[i][j].x , points[i][j].y, points[i][j].z-0.01);}

        }

    }

}

void Shape::SetTexture(Texture texture) {
    this->textureID=texture;
}

glm::vec3 Shape::lerp(glm::vec3 P0, glm::vec3 P1, float t){
    return P0*(1-t) + P1*t;
}

glm::vec3 Shape::cubicBrezier(glm::vec3 *CP, float t) {
    //std::cout << CP[0].x << CP[1].x << CP[2].x << CP[3].x << "\n";
    glm::vec3 P12 = lerp(CP[0], CP[1], t);
    glm::vec3 P23 = lerp(CP[1], CP[2], t);
    glm::vec3 P34 = lerp(CP[2], CP[3], t);
    glm::vec3 P1223 = lerp(P12, P23, t);
    glm::vec3 P2334 = lerp(P23, P34, t);

    return P1223*(1-t) + P2334*t;
}

glm::vec3 Shape::BrezierSurface( float u, float v){
    glm::vec3 curve[4];

    for(int i = 0; i < 4; i++){
        curve[i] = cubicBrezier(points[i], u);
    }

    return cubicBrezier(curve, v);

}

void Shape::inicialization() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate a vertex buffer object, this will feed data to the vertex shader
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &vto);
    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, Generovane.size() * sizeof(glm::vec3), Generovane.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vto);
    glBufferData(GL_ARRAY_BUFFER, TextCoord.size() * sizeof(glm::vec2), TextCoord.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, sizeof(glm::vec2),(GLvoid*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indexy.size() * sizeof(GLuint), Indexy.data(), GL_STATIC_DRAW);


    projection = glm::perspective(45.0f,1.0f,0.1f,1000.0f);
    view = glm::lookAt(glm::vec3(0.f, 7.0f, 3.0f),glm::vec3(0.0f,0.2f,0.0f),glm::vec3(0.f,1.f,0.5f));
    rotation = glm::mat4(1.0f);

    glBindTexture(GL_TEXTURE_2D, textureID.GetTexture());




}

void Shape::render(ShaderPtr program) {
    program->Use();
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate a vertex buffer object, this will feed data to the vertex shader
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &vto);
    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, Generovane.size() * sizeof(glm::vec3), Generovane.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vto);
    glBufferData(GL_ARRAY_BUFFER, TextCoord.size() * sizeof(glm::vec2), TextCoord.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, sizeof(glm::vec2),(GLvoid*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indexy.size() * sizeof(GLuint), Indexy.data(), GL_STATIC_DRAW);


}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if(key == GLFW_KEY_W && action == GLFW_PRESS)
        zoom -= 0.2f;
    if(key == GLFW_KEY_S && action == GLFW_PRESS)
        zoom += 0.2f;
    if(key == GLFW_KEY_D && action == GLFW_PRESS)
        tilt += 0.2f;
    if(key == GLFW_KEY_A && action == GLFW_PRESS)
        tilt -= 0.2f;
}

int main() {
    // Init GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return EXIT_FAILURE;
    }
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    auto window = glfwCreateWindow(800, 800, "PPGSO8WEEK", nullptr, nullptr);
    if (!window) {
        std::cerr << "Neprepol si graficku kartu" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    if (!glewIsSupported("GL_VERSION_3_3")) {
        std::cerr << "Failed to initialize GLEW with OpenGL 3.3!" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }


    // Define the viewport dimensions



    glfwSetKeyCallback(window, key_callback);

    // Build and compile our shader program
    auto program = ShaderPtr(new Shader{ppgso8week_vert, ppgso8week_frag});
    glUseProgram(program->GetProgram());
    Shape shape=Shape();
    shape.generate_Control_Points();
    auto texture = Texture("wall_rgb.rgb", 512, 512);

    //shape.SetTexture(texture);

    //std::cout << sizeof(glm::vec3) << " " << 3* sizeof(GLfloat);
    shape.generate_Points();


    shape.inicialization();

    glBindTexture(GL_TEXTURE_2D, texture.GetTexture());
    glActiveTexture(GL_TEXTURE0);

    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    float time=0;
    while (!glfwWindowShouldClose(window))
    {

        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        //SetTransformation(program, time+=0.01f);
        //shape.rotation = glm::rotate(shape.rotation,0.01f,glm::vec3(1.5f,0.2f,0.0f));

        shape.view = glm::lookAt(glm::vec3(tilt, zoom, 3.0f),glm::vec3(0.0f,0.2f,0.0f),glm::vec3(0.f,1.f,0.0f));

        glBindVertexArray(shape.vao);
        glUniformMatrix4fv(glGetUniformLocation(program->GetProgram(),"view"),1,GL_FALSE,glm::value_ptr(shape.view));
        glUniformMatrix4fv(glGetUniformLocation(program->GetProgram(),"projection"),1,GL_FALSE,glm::value_ptr(shape.projection));
        glUniformMatrix4fv(glGetUniformLocation(program->GetProgram(),"rotation"),1,GL_FALSE,glm::value_ptr(shape.rotation));

        glDrawElements(GL_TRIANGLES,shape.Indexy.size(),GL_UNSIGNED_INT,0);

        // Display result
        glfwSwapBuffers(window);
        glfwPollEvents();

        //std::cout << time << "\n";
        time += 0.2;
        shape.alter_Control_Points(sin(time));
        shape.generate_Points();
        shape.render(program);
        /*
        glBindTexture(GL_TEXTURE_2D, texture.GetTexture());
        glActiveTexture(GL_TEXTURE0);
        */
        glEnable(GL_DEPTH_TEST);


    }


    glfwTerminate();

    return 0;


}