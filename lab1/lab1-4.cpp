#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
    
    if ((key == GLFW_KEY_R) && action == GLFW_PRESS) {
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Reload shaders
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//    
    shader0.update("../lab1-4_vs.glsl","../lab1-4_fs.glsl");
    } 

//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Update some parameter for the vertex shader here.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    if ((key == GLFW_KEY_LEFT) && ( (action == GLFW_PRESS) || (action == GLFW_REPEAT))){
        shader0.x_offset = shader0.x_offset-0.1;
        
    }
    if ((key == GLFW_KEY_RIGHT) && ( (action == GLFW_PRESS) || (action == GLFW_REPEAT))){
        shader0.x_offset = shader0.x_offset+0.1;
    }
    if ((key == GLFW_KEY_UP) && ( (action == GLFW_PRESS) || (action == GLFW_REPEAT))){
        shader0.y_offset = shader0.y_offset+0.1;
    }
    if ((key == GLFW_KEY_DOWN) && ( (action == GLFW_PRESS) || (action == GLFW_REPEAT))){
        shader0.y_offset = shader0.y_offset-0.1;
    }
}

static void scroll_callback(GLFWwindow* window, double scroll_v, double scroll_h)
{
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Update some parameter for the fragment shader here.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    if(scroll_h > 0){
        if(shader0.modifier>0)
            shader0.modifier = shader0.modifier-0.1;
        
    }
    else{
        if(shader0.modifier<5)
            shader0.modifier = shader0.modifier+0.1;
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
  
  GLFWwindow* window = glfwCreateWindow (640, 480, "Hello Triangle", NULL, NULL);
  glfwSetKeyCallback( window, key_callback);
  glfwSetScrollCallback( window, scroll_callback); 
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
// Set up geometry, VBO, VAO
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
GLfloat vertices[] = {-0.5f,-0.5f   ,0.0f,
                        0.5f,-0.5f,0.0f,
                        0.0f,0.5f,0.0f};
  GLuint VAO,VBO;
  //VAO
  glGenVertexArrays(1,&VAO);
  glBindVertexArray(VAO);
  //VBO
  glGenBuffers(1,&VBO);
  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0);
  glEnableVertexAttribArray(0);
        
    
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// load and compile shaders  "../lab1-4_vs.glsl" and "../lab1-4_fs.glsl"
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  shader0.update("../lab1-4_vs.glsl","../lab1-4_fs.glsl");
    
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// attach and link vertex and fragment shaders into a shader program // Done in shader0.update()
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//

  while (!glfwWindowShouldClose (window)) 
  {

//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Update uniform variables in your shader_program
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    shader0.position_offset_loc = glGetUniformLocation(shader0.shader_program,"position_offset");
    shader0.modifier_loc = glGetUniformLocation(shader0.shader_program,"modifier");
    glUniform1f(shader0.modifier_loc,shader0.modifier);
    glUniform2f(shader0.position_offset_loc,shader0.x_offset,shader0.y_offset);
      
      
    // update other events like input handling 
    glfwPollEvents ();
    // clear the drawing surface
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Issue an appropriate glDraw*() command.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    glDrawArrays(GL_TRIANGLES,0,3);   
    glfwSwapBuffers (window);
  }

  // close GL context and any other GLFW resources
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

