#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <cstdlib>  
#include <iostream>
#include <vector>
#include "readfile.hpp"
#include "my_shader.hpp"
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// You can store the rotation angles here, for example
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
my_shader shader0;
GLuint smoothness_factor = 1;
int light_count = 1;

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
		shader0.update("../lab2-3_vs2.glsl","../lab2-3_fs2.glsl");
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
	
	//Smoothness factor
	if ((key == GLFW_KEY_O) && (action == GLFW_PRESS || action==GLFW_REPEAT) && smoothness_factor < 1000)
    {
      //implement reloading of the shaders on the fly
		smoothness_factor = smoothness_factor+10;
		printf("Smoothness Factor: %d\n",smoothness_factor);
    } 
	if ((key == GLFW_KEY_P) && (action == GLFW_PRESS || action==GLFW_REPEAT) && smoothness_factor > 1)
    {
		
		smoothness_factor = smoothness_factor-10;
		printf("Smoothness Factor: %d\n",smoothness_factor);
    } 
    if ((key == GLFW_KEY_U) &&  (action == GLFW_PRESS))
      if(light_count > 1){
        light_count--;
        printf("Lights decreased to %d\n",light_count);
      }
  if ((key == GLFW_KEY_I) &&  (action == GLFW_PRESS))
      if(light_count < 4){
        light_count++;
        printf("Lights increased to %d\n",light_count);
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
  std::string inputfile = "../../common/data/bunny.obj";
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::string err;
  int flags = 0; // see load_flags_t enum for more information.

  // Load the object
  bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(), 0, flags);

  // Check for errors
  if (!err.empty()) { // `err` may contain warning message.
    std::cerr << err << std::endl;
  }

  if (!ret) {
    exit(1);
  }
  else
  {
    std::cout << "Loaded " << inputfile
              << " ("
              << shapes.size() << " shapes"
              << ")"
              << "\n";
    if (shapes.size() > 0)
    {
      std::cout << shapes[0].mesh.positions.size() << " points "
                << "(" << shapes[0].mesh.positions.size()*sizeof(float) << " bytes), "
                << shapes[0].mesh.indices.size() << " indices "
                << "(" << shapes[0].mesh.indices.size()*sizeof(unsigned int) << " bytes), "
                << shapes[0].mesh.normals.size() << " normals.\n"
        ;
    }
  }

   
    GLuint VAO,VBO,EBO,nmVBO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&nmVBO);
    glGenBuffers(1,&EBO);
    
    //VBO
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glVertexPointer(3,GL_FLOAT,sizeof(float)*3,NULL);
    glBufferData(GL_ARRAY_BUFFER,shapes[0].mesh.positions.size() * sizeof(float) ,&shapes[0].mesh.positions.at(0),GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*  sizeof(GLfloat),(GLvoid*)0);
    //Normals
    glBindBuffer(GL_ARRAY_BUFFER,nmVBO);
    glBufferData(GL_ARRAY_BUFFER,shapes[0].mesh.normals.size() * sizeof(float),&shapes[0].mesh.normals.at(0),GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0); //(void*)(3*sizeof(GLfloat) offset?
	
    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,shapes[0].mesh.indices.size() * sizeof(unsigned int),&shapes[0].mesh.indices.at(0),GL_STATIC_DRAW);
    
  
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// load and compile shaders  "../lab1-7_vs.glsl" and "../lab1-7_fs.glsl"
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  shader0.update("../lab2-3_vs.glsl","../lab2-3_fs.glsl");  
 
  const float n = 1.0f;
  const float f = 100.0f;
  
  std::cout<<sizeof(unsigned int) << " "<< sizeof(float) << std::endl;;
  
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
    
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0,-1.0f,0));
    modelMatrix = glm::scale(modelMatrix,glm::vec3(6.0f,6.0f,6.0f));
    
    modelMatrix = glm::rotate(modelMatrix,shader0.x_rot_angle,glm::vec3(1.0f,0.0f,0.0f)); //x-rotation
    modelMatrix = glm::rotate(modelMatrix,shader0.y_rot_angle,glm::vec3(0.0f,1.0f,0.0f)); //y-rotation  
    
    
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix,glm::vec3(0.0f,0.0f,2.0f));
    
    glm::mat4 inverseViewMatrix = glm::inverse(viewMatrix);
    
     //Send light
    GLuint light_countLoc = glGetUniformLocation(shader0.shader_program,"light_count");
    glUniform1i(light_countLoc,light_count);
    
    //light position
    glm::vec4 light_position[4] = { glm::vec4(0, 0, 5.0f, 0),
                                    glm::vec4(-5.0f, 0, 0, 0),
                                    glm::vec4(0, 5.0f, 0, 0),
                                    glm::vec4(0) };
    GLuint light_positionLoc = glGetUniformLocation(shader0.shader_program,"light_position");
    glUniform4fv(light_positionLoc,4,glm::value_ptr(light_position[0]));
   
    //Light colour
    glm::vec4 light_colour[4] = { glm::vec4(1.0f,1.0f,1.0f,0),
                                  glm::vec4(1.0f,1.0f,1.0f,0),
                                  glm::vec4(1.0f,1.0f,1.0f,0),
                                  glm::vec4(1.0f,1.0f,1.0f,0) };
    GLuint light_colourLoc = glGetUniformLocation(shader0.shader_program,"light_colour");
    glUniform4fv(light_colourLoc,4,glm::value_ptr(light_colour[0]));
    
	//Smoothness factor
  	glUniform1i(glGetUniformLocation(shader0.shader_program,"smoothness_factor"),smoothness_factor);
    
    //
    //Send model
    GLuint modelLoc = glGetUniformLocation(shader0.shader_program,"model");
    glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(modelMatrix));
    
    //Send view
    GLuint viewLoc = glGetUniformLocation(shader0.shader_program,"view");
    glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(inverseViewMatrix));
    
    //Send projetion    
    GLuint transformLoc = glGetUniformLocation(shader0.shader_program,"projection");
    glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(projectionMatrix));

    // update other events like input handling 
    glfwPollEvents ();
    
    // clear the drawing surface
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Issue an appropriate glDraw*() command.
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    glDrawElements(GL_TRIANGLES,shapes[0].mesh.indices.size(),GL_UNSIGNED_INT,0);
    //glDrawArrays(GL_TRIANGLES,0,shapes[0].mesh.positions.size());
    glfwSwapBuffers (window);
    
  }

  // close GL context and any other GLFW resources
  glfwTerminate();
  exit(EXIT_SUCCESS);
}




