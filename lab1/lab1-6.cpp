  // GL Extension Wrangler
#include <GL/glew.h>
#include <cmath>
// http://www.glfw.org/faq.html#what-is-glfw
//OpenGL doesn't have a way of creating window, this does it in an OS-independent way
#include <GLFW/glfw3.h>
#include <cstdlib>  
#include <iostream>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "readfile.hpp"
#include "my_shader.hpp"

my_shader shader0;

//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// You can store the rotation angles here, for example
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
float g_rotation[2];

void MUL_4x4 (float (*C)[4], const float (*A)[4], const float (*B)[4])
{
  int i,j,k;
  for(i =0;i < 4;i++){
    for (j=0;j < 4;j++){
      C[i][j] = 0;
        for (k=0;k<4;k++){
          C[i][j] += A[i][k] * B[k][j]; 
        }
    }
  }
  //computes C = A x B
}

void invertMatrix (float (*C)[4], float (*A)[4])
{   
  int n = sizeof(*A)/sizeof(*A[0]);
  GLfloat A_aug[n][2*n];
  
  //Create an augmented matrix for gaussian elimination
  for(int i=0; i<n;i++){
    for(int j=0;j < n;j++){
          A_aug[i][j] = A[i][j];
    }
  }
 
  //Add identify matrix to A_aug
  for(int i=0;i<n;i++){
    for(int j=n;j<2*n;j++){
      if(j ==(i+n))
        A_aug[i][j] = 1;
      else
        A_aug[i][j] = 0;
    }
  }
  
  //Swap
  for(int i = n-1; i>0;i--){
    if(A_aug[i-1][0] < A_aug[i][0]) {
      for(int j=0;j<2*n;j++){
        GLfloat tmp = A_aug[i][j];
        A_aug[i][j] = A_aug[i-1][j];
        A_aug[i-1][j] = tmp;
      }
    }
  }
  
  //Multiply away values above or below diagonal values
  for(int i = 0;i<n;i++){
    for(int j=0;j<n;j++){
      if(j != i){
        float tmp = A_aug[j][i]/A_aug[i][i];
        for(int k=0;k<2*n;k++){
          A_aug[j][k] -= A_aug[i][k]*tmp;
        }
      }
    }
  }
  //Divide each diagonal for identity matrix
  for(int i=0;i<n;i++){
    float tmp = A_aug[i][i];
    for(int j=0;j<2*n;j++){
      A_aug[i][j] = A_aug[i][j]/tmp; 
    }
  }
  //Set the last 4 columns as C
  for(int i=0;i<n;i++){
    for(int j=n;j<2*n;j++){
        C[i][j-n] = A_aug[i][j];   
    }
  }
  
}




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
      shader0.update("../lab1-6_vs.glsl","../lab1-6_fs.glsl");
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
  GLFWwindow* window = glfwCreateWindow (640, 480, "Hello Icosahedron", NULL, NULL);
  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  
  int w_height = 600;
  int w_width = 800;

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
// load and compile shaders  "../lab1-6_vs.glsl" and "../lab1-6_fs.glsl"
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  shader0.update("../lab1-6_vs.glsl","../lab1-6_fs.glsl");
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// attach and link vertex and fragment shaders into a shader program
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//

 
  float n=1.0;
  float f=100.0;
  float a = -(f+n)/(f-n);
  float b = -(2*f*n)/(f-n);
  
  
  
  while (!glfwWindowShouldClose (window)) 
  {
    
    glfwGetFramebufferSize(window, &w_width , &w_height ); //you might need this for correcting the aspect ratio

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    // Define the projection matrix, rotation matrices, model matrix, etc. The variable names and code structure is a simple suggestion, you may improve on it!
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    
    GLfloat projectionMatrix[4][4] = {{600.0f/800.0f,0,0,0},{0,1,0,0},{0,0,a,b},{0,0,-1,0}};
    GLfloat rotate_y[4][4] = {{std::cos(shader0.y_rot_angle), 0, std::sin(shader0.y_rot_angle), 0}, {0,1,0,0}, {(-1)*std::sin(shader0.y_rot_angle), 0, std::cos(shader0.y_rot_angle),0}, {0,0,0,1}};
    GLfloat rotate_x[4][4] = {{1,0,0,0},{0,std::cos(shader0.x_rot_angle),(-1)*std::sin(shader0.x_rot_angle),0}, {0, std::sin(shader0.x_rot_angle), std::cos(shader0.x_rot_angle), 0}, {0,0,0,1} }; 
    
    
    GLfloat modelMatrix[4][4];
    MUL_4x4(modelMatrix, rotate_x, rotate_y);
    
    GLfloat viewMatrix[4][4] = {{1,0,0,0},{0,1,0,0}, {0,0,1,2}, {0,0,0,1} };
    
    GLfloat inverseViewMatrix[4][4];
    invertMatrix(inverseViewMatrix, viewMatrix);
    
    GLfloat modelViewMatrix[4][4];
    MUL_4x4(modelViewMatrix, inverseViewMatrix, modelMatrix);
    
    
    GLfloat modelViewProjectionMatrix[4][4];
    MUL_4x4(modelViewProjectionMatrix, projectionMatrix, modelViewMatrix);
    
    GLfloat modelViewProjectionMatrix2[4][4];
    
    
    for(int i=0;i<4;i++){
      for(int j=0;j<4;j++){
        modelViewProjectionMatrix2[j][i] = modelViewProjectionMatrix[i][j];
        
      }
    }
    
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    // Send your modelViewProjection matrix to your vertex shader as a uniform varable
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    GLuint transformLoc = glGetUniformLocation(shader0.shader_program,"transform");
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    // modelViewProjectionMatrix uses [row][column] ordering while GLM uses [column][row]
    // this version spins the entire cube around and x or y-axis and it's easier to see the depth change
    // To use [column][row] order and spin around the cube's own axis, use glUniformMatrix4fv(transformLoc,1,GL_FALSE,*modelViewProjectionMatrix2)
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    glUniformMatrix4fv(transformLoc,1,GL_FALSE,*modelViewProjectionMatrix);
    //glUniformMatrix4fv(transformLoc,1,GL_FALSE,*modelViewProjectionMatrix2);
    
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

