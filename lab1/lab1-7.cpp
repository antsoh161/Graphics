#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"
#include "my_shader.hpp"
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// You can store the rotation angles here, for example
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
my_shader shader0;

void checkShaderCompileError(GLint shaderID)
{
  GLint isCompiled = 0;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
 
  if(isCompiled == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::string errorLog;
    errorLog.resize(maxLength);
    glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

    std::cout << "shader compilation failed:" << std::endl;
    std::cout << errorLog << std::endl;
    return;
  }
  else
    std::cout << "shader compilation success." << std::endl;
    
  return;
}


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
      //implement reloading of the shaders on the fly
    } 

//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Update rotation angle here, for example
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  
    if ((key == GLFW_KEY_RIGHT) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) )
      shader0.x_rot_angle += M_PI/12;
    if ((key == GLFW_KEY_LEFT) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) )
      shader0.x_rot_angle -= M_PI/12;
    if ((key == GLFW_KEY_UP) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) )   
      shader0.y_rot_angle += M_PI/12;
    if ((key == GLFW_KEY_DOWN) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) ) 
      shader0.y_rot_angle -= M_PI/12;
    if((key == GLFW_KEY_T) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT)){
      shader0.y_rot_angle +=M_PI/12;
      shader0.x_rot_angle = shader0.y_rot_angle;
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
  
  
  int w_height = 600;
  int w_width = 800;

  glfwSetErrorCallback(error_callback);
  if( !glfwInit() )
    exit(EXIT_FAILURE);
  
  GLFWwindow* window = glfwCreateWindow (w_width, w_height, "Hello Icosahedron", NULL, NULL);
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

  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable (GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
  
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Set up geometry, VBO, EBO, VAO
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  float sL = 0.5;   //Side length
  GLfloat points[] = {
       -sL, -sL, sL,
        sL, -sL, sL,
        sL, sL,  sL,
       -sL, sL, sL,
       -sL, -sL, -sL,
       sL, -sL, -sL,
       sL, sL, -sL,
       -sL, sL, -sL    
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
// load and compile shaders  "../lab1-7_vs.glsl" and "../lab1-7_fs.glsl"
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  shader0.update("../lab1-7_vs.glsl","../lab1-7_fs.glsl");
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// attach and link vertex and fragment shaders into a shader program
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  
 
  const float n = 1.0f;
  const float f = 100.0f;
  

  while (!glfwWindowShouldClose (window)) 
  {


    glfwGetFramebufferSize(window, &w_width , &w_height );

//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// use glm::perspective to create a Projection matrix
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f),800.0f/600.0f,n,f); // W/H instead
  
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// use glm::translate, glm::rotate and glm::inverse to create the model and view matrices
//---------------------------------------------------------------------------------------------vmat--------------------------------------------------------------//
  
  glm::mat4 modelMatrix = glm::mat4(1.0f);  //init to identity matrix
  modelMatrix = glm::rotate(modelMatrix,shader0.x_rot_angle,glm::vec3(1.0f,0.0f,0.0f)); //x-rotation
  modelMatrix = glm::rotate(modelMatrix,shader0.y_rot_angle,glm::vec3(0.0f,1.0f,0.0f)); //y-rotation
  
  glm::mat4 viewMatrix = glm::mat4(1.0f);
  viewMatrix = glm::translate(viewMatrix,glm::vec3(0.0f,0.0f,2.0f));
    
  glm::mat4 inverseViewMatrix = glm::inverse(viewMatrix);
    
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// multiply your matrices in the right order to get a modelViewProjection matrix and upload it to the appropriate uniform variable in vertex shader
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    glm::mat4 modelViewMatrix = inverseViewMatrix * modelMatrix;
    glm::mat4 projectionModelViewMatrix = projectionMatrix * modelViewMatrix;
    
    
    GLuint transformLoc = glGetUniformLocation(shader0.shader_program,"transform");
    glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(projectionModelViewMatrix));

    // update other events like input handling 
    glfwPollEvents ();
    
    // clear the drawing surface
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

