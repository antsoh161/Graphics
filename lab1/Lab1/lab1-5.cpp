#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"
#include "my_shader.hpp"

my_shader shader0;

static void error_callback(int error, const char* description)
{
    std::cerr << description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    if ((key == GLFW_KEY_R) && action == GLFW_PRESS)
        
    {
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Reload shaders
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
        shader0.update("../lab1-5_vs.glsl","../lab1-5_fs.glsl");
    } 
}
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int argc, char const *argv[])
{
  // start GL context and O/S window using the GLFW helper library
  
  glfwSetErrorCallback(error_callback);
  if( !glfwInit() )
    exit(EXIT_FAILURE);
  
  GLFWwindow* window = glfwCreateWindow (640, 480, "Hello Icosahedron", NULL, NULL);
  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
  if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
	}
  glfwMakeContextCurrent (window);
                                  
  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit ();

//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Set up geometry, VBO, EBO, VAO
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
   GLfloat points[] = {
       -0.5, -0.5, 0.5,
        0.5, -0.5, 0.5,
        0.5, 0.5,  0.5,
       -0.5, 0.5, 0.5,
       -0.5, -0.5, -0.5,
       0.5, -0.5, -0.5,
       0.5, 0.5, -0.5,
       -0.5, 0.5, -0.5    
   };
    
    unsigned short faces[] = {
        0,1,2,
        2,3,0,
        1,5,6,
        6,2,1,
        7,6,5,
        5,4,7,
        4,0,3,
        3,7,4,
        4,5,1,
        1,0,4,
        3,2,6,
        6,7,3
        };
            
            
    GLuint VAO,VBO,EBO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    
    //VBO
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(points),points,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(faces),faces,GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*  sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// load and compile shaders  "../lab1-5_vs.glsl" and "../lab1-5_fs.glsl"
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    shader0.update("../lab1-5_vs.glsl","../lab1-5_fs.glsl"); //Load shaders
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// attach and link vertex and fragment shaders into a shader program // Done in shader0.update()
//   
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  while (!glfwWindowShouldClose (window)) 
  {

    // update other events like input handling 
    glfwPollEvents ();
    
    // clear the drawing surface../lab1-5_vs.glsl","../lab1-5_fs.glsl")
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Issue an appropriate glDraw*() command.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,0);
    glfwSwapBuffers (window);
  }

  // close GL context and any other GLFW resources
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

