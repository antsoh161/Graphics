
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
		shader0.update("../lab2-1_vs.glsl","../lab2-1_fs.glsl");
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
	
	
      float points[] = {
	// A cube has 8 vertices, but now we have three copies of each one:
	-0.5, -0.5, -0.5, //0 0
	-0.5, -0.5, -0.5, //0 1
	-0.5, -0.5, -0.5, //0 2
	//
	-0.5, -0.5,  0.5, //1 3
	-0.5, -0.5,  0.5, //1 4
	-0.5, -0.5,  0.5, //1 5
	//
	-0.5,  0.5, -0.5, //2 6
	-0.5,  0.5, -0.5, //2 7
	-0.5,  0.5, -0.5, //2 8
	//
	-0.5,  0.5,  0.5, //3 9
	-0.5,  0.5,  0.5, //3 10
	-0.5,  0.5,  0.5, //3 11
	//
	0.5, -0.5, -0.5, //4 12
	0.5, -0.5, -0.5, //4 13
	0.5, -0.5, -0.5, //4 14
	//
	0.5, -0.5,  0.5, //5 15
	0.5, -0.5,  0.5, //5 16
	0.5, -0.5,  0.5, //5 17
	//
	0.5,  0.5, -0.5, //6 18
	0.5,  0.5, -0.5, //6 19
	0.5,  0.5, -0.5, //6 20
	//
	0.5,  0.5,  0.5, //7 21
	0.5,  0.5,  0.5, //7 22
	0.5,  0.5,  0.5, //7 23
	};
	
	unsigned short faces[]= {
	// ... and 12 triangular faces, 
	// defined by the following vertex indices:
	0, 9, 6, // 0 3 2
	0, 3, 9, // 0 1 3
	//
	1, 7, 18, // 0 2 6
	1, 18, 12, // 0 6 4
	//
	13, 19, 15, // 4 6 5
	15, 19, 21, // 5 6 7
	//
	16, 22, 10, // 5 7 3
	16, 10, 4, // 5 3 1
	//
	8, 11, 23, // 2 3 7
	8, 23, 20, // 2 7 6
	//
	2, 14, 5, // 0 4 1
	5, 14, 17 // 1 4 5
	};
	
  float normals[3*3*8] = {0}; //4 elements, 3 copies, 8 rows
	std::vector<glm::vec4> nmVector;
  
  for(int i = 0; i < 36; i=i+3){
    int p1_index = faces[i];  //Get index for each vertices forming the triangle
    int p2_index = faces[i+1];
    int p3_index = faces[i+2];
    
    //Compute normal for the triangle
    glm::vec4 u = glm::vec4(points[3*p2_index],points[3*p2_index+1],points[3*p2_index+2],0) - glm::vec4(points[3*p1_index],points[3*p1_index+1],points[3*p1_index+2],0);
    glm::vec4 v = glm::vec4(points[3*p3_index],points[3*p3_index+1],points[3*p3_index+2],0) - glm::vec4(points[3*p1_index],points[3*p1_index+1],points[3*p1_index+2],0);

    glm::vec4 nm = glm::vec4(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x,0 );
    nmVector.insert(nmVector.end(),nm);
    //Build normals array
    /*
    normals[4*p1_index] = nm.x;
    normals[4*p1_index+1] = nm.y;
    normals[4*p1_index+2] = nm.z;
    normals[4*p1_index+3] = nm.w;
    
    normals[4*p2_index] = nm.x;
    normals[4*p2_index+1] = nm.y;
    normals[4*p2_index+2] = nm.z;
    normals[4*p2_index+3] = nm.w;
    
    normals[4*p3_index] = nm.x;
    normals[4*p3_index+1] = nm.y;
    normals[4*p3_index+2] = nm.z;
    normals[4*p3_index+3] = nm.w;
    */
    normals[3*p1_index] = nm.x;
    normals[3*p1_index+1] = nm.y;
    normals[3*p1_index+2] = nm.z;
    
    normals[3*p2_index] = nm.x;
    normals[3*p2_index+1] = nm.y;
    normals[3*p2_index+2] = nm.z;
    
    normals[3*p3_index] = nm.x;
    normals[3*p3_index+1]=nm.y;
    normals[3*p3_index+2]=nm.z;
    
  }
    int x = 0;
    int count = 0;
    for(int i = 0; i<sizeof(normals)/sizeof(normals[0]);i++){
      std::cout<<normals[i]<<" ";
      if(count==2){
        std::cout<<"\t  |"<<x<<std::endl;
        x++;
        count=0;
      }
      else
        count++;
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
    glBufferData(GL_ARRAY_BUFFER,sizeof(points),points,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*  sizeof(GLfloat),(GLvoid*)0);
    //Normals
    glBindBuffer(GL_ARRAY_BUFFER,nmVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(normals),normals,GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(void*)0); //(void*)(3*sizeof(GLfloat) offset?
    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(faces),faces,GL_STATIC_DRAW);
    
  
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// load and compile shaders  "../lab1-7_vs.glsl" and "../lab1-7_fs.glsl"
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  shader0.update("../lab2-1_vs.glsl","../lab2-1_fs.glsl");

  
 
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
    //glm::mat4 modelViewMatrix = inverseViewMatrix * modelMatrix;
    GLuint modelLoc = glGetUniformLocation(shader0.shader_program,"model");
    glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(modelMatrix));
    
    GLuint viewLoc = glGetUniformLocation(shader0.shader_program,"view");
    glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(inverseViewMatrix));
    
    //GLuint modelViewLoc = glGetUniformLocation(shader0.shader_program,"modelview");
    //glUniformMatrix4fv(modelViewLoc,1,GL_FALSE,glm::value_ptr(modelViewMatrix));
    
    //glm::mat4 projectionModelViewMatrix = projectionMatrix * modelViewMatrix;
    
    
    GLuint transformLoc = glGetUniformLocation(shader0.shader_program,"projection");
    glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(projectionMatrix));

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


