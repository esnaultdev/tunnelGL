/* Include headers*//*{{{*/
#include <stdlib.h>   //random
#include <time.h>     //time()
#include <string>
#include <iostream>
#include <streambuf>
#include <fstream>
#include <stdlib.h>
#include <stack>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// matrix and vectors
// to declare before including glm.hpp, to use the swizzle operators
#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include "utils/utils.h"
#include "utils/shaders.h"
#include "utils/textures.h"
#include "utils/objloader.h"

#include "glfwMinimal.h"
#include <GL/glew.h>  // must be included before gl.h
#include <GLFW/glfw3.h>

//loading stuffs
int vertexCount = 3;
int vertexIndexCount = 3;
int indexCount = 3;
GLuint vao;

GLuint TunnelBuffer;
GLuint positionBuffer;
GLuint indexBuffer;
GLuint programId;

int matrice_heigth = 30;
int matrice_seg = 12;
int matrice_size = matrice_heigth*matrice_seg;


int* Creat_Random_Matrice_12by30()
{

    srand(time(NULL));

    int *matrice = new int[matrice_size];
    for(int i=0;i<matrice_size;i++)
    {
        matrice[i] = (rand()%100 +1 >95) ? 0 : 1 ;
    }

    return matrice;
}




void make_ressources()
{
    int *matrice = Creat_Random_Matrice_12by30();
    int i=0;
    while(i<12*30)
    {
        if((i)%12==0 && i!=0)
        {
            std::cout<<std::endl;
        }
        std::cout<<matrice[i];

        i++;
    }


    std::vector<glm::vec3>tube_position;
    double theta;
    double angle = 3.14/6;
    double R=0.5;
    for(int j=0;j<30;j++)
    {
        theta = 0;
        for(i= 0; i < 12; i++)
        {
            tube_position.push_back(glm::vec3(glm::cos(theta)*R,glm::sin(theta)*R,j));
            theta = theta + angle;
        }

    }

    glGenBuffers(1, &TunnelBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, TunnelBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexCount* matrice_size * sizeof(float), &tube_position[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    unsigned int *indices = new unsigned int[matrice_heigth*6];
    int index = 0;
    i=0;
    while(i<(12*29))
    {
        if(matrice[i] == 1)
        {
            if((i+1)%12 == 0)
            {
                indices[index] = i;
                indices[index+1] = i+12;
                indices[index+2] = i-11;

                indices[index+3] = i-11;
                indices[index+4] = i+12;
                indices[index+5] = i+1;
            }
            else
            {
                indices[index] = i;
                indices[index+1] = i+12;
                indices[index+2] = i+1;

                indices[index+3] = i+1;
                indices[index+4] = i+12;
                indices[index+5] = i+1+12;
            }
        index= index+6;
        }
        i++;
    }


    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*12 * 29 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    //--------- Creation and activation
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //--------- Encapsulation and setting of VBOs
    glBindBuffer(GL_ARRAY_BUFFER, TunnelBuffer);
    GLint positionsIndex = glGetAttribLocation(programId, "position");//here is the index and the associated GLSL attribute "position" (more on that later)
    glEnableVertexAttribArray(positionsIndex);
    glVertexAttribPointer(positionsIndex,3 , GL_FLOAT, GL_FALSE, 0, 0);
    //--------- Encapsulation of an IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    //--------- Deactivation (clean OpenGl state!!)
    glBindVertexArray(0);


}







void shutDown(int i)
{
    glDeleteBuffers(1, &positionBuffer);
    glDeleteVertexArrays(1, &vao);
    exit(i);
}


void initGLEW(void)
{
    // GLEW Initialization
    // Do not forget to use glewExperimental so that glewInit works with core
    // profile: http://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();
    if (GlewInitResult != GLEW_OK) {
        std::cerr<<"ERROR: "<< glewGetErrorString(GlewInitResult)<<std::endl;
        shutDown(1);
    }
}


void initGLFW(void)
{
    if (!glfwInit()) shutDown(1);
  // Create OpenGL 4.4 Core Profile and Forward Compatible Context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Create a window of given size
    const int window_width = 1024, window_height = 800;
    GLFWwindow* window =glfwCreateWindow(window_width, window_height,
        "Minimal OpenGL application with GLFW", NULL, NULL);
    if (!window)
    {
        std::cerr<<"Could not open a window"<<std::endl;
        shutDown(1);
    }
  // Make the context of the specified window current
    glfwMakeContextCurrent(window);

}

void init(void)
{



    // GLFW initialization
    initGLFW();
    initGLEW();
    make_ressources();
    std::cout<<"Seems we made it "<<std::endl;
    std::cout<<"OpenGL version: "<<glGetString(GL_VERSION)<<std::endl;
    std::cout<<"GLSL version: "<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<std::endl;/*}}}*/


}

int main(void)
{
    init();
    while(1)
    {
        renderFrame();
    }
    shutDown(0);
}

void renderFrame(void)
{

    // clear the buffer
    glClear(GL_COLOR_BUFFER_BIT );
    glClear(GL_DEPTH_BUFFER_BIT );

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glUseProgram(programId);

    //--------- Activate the VAO and call the drawing routine

    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES,matrice_size, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES,0,12*12*6);

    //--------- Clean state again
    glBindVertexArray(0);
    glUseProgram(0);


    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);



    GLFWwindow* window = glfwGetCurrentContext();
    glfwSwapBuffers(window);
    glfwPollEvents();


}

