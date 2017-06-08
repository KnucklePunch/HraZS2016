//
// Created by HORIZON on 5. 10. 2016.
//

#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "mesh.h"
#include "gl_texture_frag.h"
#include "gl_texture_vert.h"

// Size of the framebuffer
const unsigned int SIZE = 512;

// A simple RGB struct will represent a pixel in the framebuffer
// Note: unsigned char is range <0, 255>
//       signed char is range <-128, 127>
struct Pixel {
    unsigned char r, g, b;
};

// Load a new image from a raw RGB file directly into OpenGL memory
GLuint LoadImage(const std::string &image_file, unsigned int width, unsigned int height) {
    // Create new texture object
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Set mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Read raw data
    std::ifstream image_stream(image_file, std::ios::binary);

    // Setup buffer for pixels (r,g,b bytes), since we will not manipulate the image just keep it as char
    std::vector<char> buffer(width*height*3);
    image_stream.read(buffer.data(), buffer.size());
    image_stream.close();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());

    return texture_id;
}

typedef struct{
    float x;
    float y;
}Point;

Pixel obraz[512][512];

void nakresliHviezdu(float x1, float x2, float y1, float y2){
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if(steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if(x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const float dx = x2 - x1;
    const float dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;

    const int maxX = (int)x2;

    for(int x=(int)x1; x<maxX; x++)
    {
        if(steep)
        {
            obraz[y][x].r = (unsigned char)0;
            obraz[y][x].g = (unsigned char)0;
            obraz[y][x].b = (unsigned char)0;
        }
        else
        {
            obraz[x][y].r = (unsigned char)0;
            obraz[x][y].g = (unsigned char)0;
            obraz[x][y].b = (unsigned char)0;
        }

        error -= dy;
        if(error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
}


int main() {

    std::vector<Point> points;



    int i,j;
    float PI = 3.14159;

    for(i=0; i<512; i++){//bielenie pozadia
        for(j=0;j<512; j++){
            obraz[i][j].r = (unsigned char)255;
            obraz[i][j].g = (unsigned char)255;
            obraz[i][j].b = (unsigned char)255;
        }
    }


    Point bod,n_bod,center;
    bod.x = 512/7;
    bod.y = 512/2;
    center.x = 512/2;
    center.y = 512/2;
    points.push_back(bod);
    for(i=0;i<4;i++){
        bod.x -= center.x;
        bod.y -= center.y;

        n_bod.x =(float) (bod.x*std::cos(0.8*PI) - bod.y*std::sin(0.8*PI));
        n_bod.y =(float) (bod.x*std::sin(0.8*PI) + bod.y*std::cos(0.8*PI));

        bod.x = n_bod.x + center.x;
        bod.y = n_bod.y + center.y;
        points.push_back(bod);
    }
    for(i=0;i<5;i++){
        std::cout << "Bod: " << points[i].y << " " << points[i].x << "\n";
        if(i < 4){nakresliHviezdu(points[i].x,points[i+1].x,points[i].y,points[i+1].y);}
        else{nakresliHviezdu(points[4].x,points[0].x,points[4].y,points[0].y);}
    }

    //nakresliHviezdu(points[0].x,points[1].x,points[0].y,points[1].y);
    //nakresliHviezdu(points[1].x,points[2].x,points[1].y,points[2].y);
    //nakresliHviezdu(points[2].x,points[3].x,points[2].y,points[3].y);
    //nakresliHviezdu(points[3].x,points[4].x,points[3].y,points[4].y);
    //nakresliHviezdu(points[4].x,points[0].x,points[4].y,points[0].y);

    //nakresliHviezdu(511/2,511/4,0,511);




    std::ofstream outfile("ciary.rgb", std::ofstream::binary);
    outfile.write((char *) obraz, 512*512*3);
    outfile.close();


//=================Tuto ide otvorenie okna aj s tym obrazkom==========================================
    Pixel framebuffer[SIZE][SIZE];
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return EXIT_FAILURE;
    }

    //Zobrazenie obrazka
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Vytvorenie okna
    auto window = glfwCreateWindow( 512, 512, "PPGSO3WEEK", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to open GLFW window, your graphics card is probably only capable of OpenGL 2.1" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    //Toto ma byt okno pre tuto thread
    glfwMakeContextCurrent(window);

    // Inicializacia GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    if (!glewIsSupported("GL_VERSION_3_3")) {
        std::cerr << "Failed to initialize GLEW with OpenGL 3.3!" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Nacitaj shaders
    auto program = ShaderPtr(new Shader{gl_texture_vert, gl_texture_frag});
    program->Use();

    auto quad = Mesh{program, "quad.obj"};

    // Load and bind texture
    auto texture_id = LoadImage("ciary.rgb", SIZE, SIZE);
    auto texture_attrib = program->GetUniformLocation("Texture");
    glUniform1i(texture_attrib, 0);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Main execution loop
    while (!glfwWindowShouldClose(window)) {
        // Set gray background
        glClearColor(.5f,.5f,.5f,0);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        quad.Render();

        // Display result
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glfwTerminate();


    // Save the raw image to a file
    std::cout << "Generating result.rgb file ..." << std::endl;
    std::ofstream raw("result.rgb", std::ios::binary);
    raw.write(reinterpret_cast<char *>(framebuffer), sizeof(framebuffer));
    raw.close();

    std::cout << "Done." << std::endl;
    return EXIT_SUCCESS;
}