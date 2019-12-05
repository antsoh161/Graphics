#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <cstdlib>  
#include <iostream>
#include <vector>
#include "readfile.hpp"
#include "my_shader.hpp"
#include "lodepng.h"
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// You can store the rotation angles here, for example
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
my_shader shader0;
float roughness_factor = 0.5;
int light_count = 2;

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
		shader0.update("../lab3-1_vs.glsl","../lab3-1_fs.glsl");
    } 

//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Update rotation angle here, for example
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  
    if ((key == GLFW_KEY_RIGHT) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) )
      shader0.x_rot_angle += M_PI/100;
    if ((key == GLFW_KEY_LEFT) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) )
      shader0.x_rot_angle -= M_PI/100;
    if ((key == GLFW_KEY_UP) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) )   
      shader0.y_rot_angle += M_PI/100;
    if ((key == GLFW_KEY_DOWN) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT) ) 
      shader0.y_rot_angle -= M_PI/100;
    if((key == GLFW_KEY_T) && ( (action == GLFW_PRESS) || action==GLFW_REPEAT)){
      shader0.y_rot_angle +=M_PI/100;
      shader0.x_rot_angle = shader0.y_rot_angle;
    }
  	//Smoothness factor
	if ((key == GLFW_KEY_O) && (action == GLFW_PRESS || action==GLFW_REPEAT) && roughness_factor < 1.0)
    {
      //implement reloading of the shaders on the fly
		roughness_factor += 0.1;
		printf("roughness Factor: %f\n",roughness_factor);
    } 
	if ((key == GLFW_KEY_P) && (action == GLFW_PRESS || action==GLFW_REPEAT) && roughness_factor > 0.1)
    {
		roughness_factor -= 0.1;
		printf("Roughness Factor: %f\n",roughness_factor);
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
	
  float t = (1.0f + sqrtf(5.0f))*0.25f;
  int num_faces = 20;
  int num_verts = 60;
  
  float points[] = {
    // An icosahedron without shared vertices has 20*3 vertices
    -0.5,  t,  0, 
    -0.5,  t,  0, 
    -0.5,  t,  0,
    -0.5,  t,  0,
    -0.5,  t,  0,
    
    0.5,  t,  0,
    0.5,  t,  0,
    0.5,  t,  0, 
    0.5,  t,  0, 
    0.5,  t,  0,
    
    -0.5, -t,  0, 
    -0.5, -t,  0, 
    -0.5, -t,  0,
    -0.5, -t,  0,
    -0.5, -t,  0,
    
    0.5, -t,  0,  
    0.5, -t,  0,  
    0.5, -t,  0, 
    0.5, -t,  0, 
    0.5, -t,  0,
    
    0, -0.5,  t,  
    0, -0.5,  t,  
    0, -0.5,  t, 
    0, -0.5,  t, 
    0, -0.5,  t,
    
    0,  0.5,  t,  
    0,  0.5,  t,  
    0,  0.5,  t, 
    0,  0.5,  t, 
    0,  0.5,  t,
    
    0, -0.5, -t,  
    0, -0.5, -t,  
    0, -0.5, -t, 
    0, -0.5, -t, 
    0, -0.5, -t,
    
    0,  0.5, -t,  
    0,  0.5, -t,  
    0,  0.5, -t, 
    0,  0.5, -t, 
    0,  0.5, -t,
    
    t,  0, -0.5,  
    t,  0, -0.5,  
    t,  0, -0.5, 
    t,  0, -0.5, 
    t,  0, -0.5,
    
    t,  0,  0.5,  
    t,  0,  0.5,  
    t,  0,  0.5, 
    t,  0,  0.5, 
    t,  0,  0.5,
    
    -t,  0, -0.5, 
    -t,  0, -0.5, 
    -t,  0, -0.5,
    -t,  0, -0.5,
    -t,  0, -0.5,
    
    -t,  0,  0.5, 
    -t,  0,  0.5, 
    -t,  0,  0.5,
    -t,  0,  0.5,
    -t,  0,  0.5
  };
	
  float uv_coords[]=
  {  
    0.072405, 0.144686,
    0.27133,  0.10033,
    0.481007, 0.129901,
    0.681948, 0.115116,
    0.928587, 0.107723,
    0.29888,  0.70651,
    0.449421, 0.736084,
    0.108695, 0.129901,
    0.847270, 0.699121,
    0.868776, 0.728691,
    0.576437, 0.721299,
    0.319044, 0.750869,
    0.548883, 0.736084,
    0.388265, 0.115116,
    0.397674, 0.713906,
    0.149690, 0.122508,
    0.347942, 0.115116,
    0.583157, 0.107723,
    0.780738, 0.100331,
    0.229663, 0.107723,
    0.33436,  0.68882,   
    0.181276, 0.721299,
    0.379528, 0.721299,
    0.190684, 0.122508,
    0.358023, 0.736084,
    0.103319, 0.713906,
    0.24512,  0.70651,
    0.083157, 0.699121,
    0.31006,  0.10251,
    0.222942, 0.721299,
    0.772001, 0.728691,
    0.618776, 0.721299,
    0.745120, 0.721299,
    0.433292, 0.115116,
    0.598614, 0.713906,
    0.520657, 0.721299,
    0.650362, 0.736084,
    0.700765, 0.728691,
    0.878184, 0.107723,
    0.671867, 0.743476,
    0.914475, 0.743476,
    0.819717, 0.736084, 
    0.201437, 0.721299,
    0.633561, 0.122508,
    0.796195, 0.743476,
    0.140953, 0.728691,
    0.122136, 0.706514,
    0.262593, 0.721299,
    0.157754, 0.728691,
    0.825765, 0.115116,
    0.723614, 0.728691,
    0.897001, 0.721299,
    0.499152, 0.736084,
    0.731007, 0.115116,
    0.469582, 0.721299,
    0.045523, 0.691729,
    0.967566, 0.721299,
    0.28341,  0.67157,
    0.532754, 0.122508,
    0.418507, 0.706514,
  };
	
  
	
  int num_indices = num_faces*3;
  unsigned short faces[]= {
    // ... and 20 triangular faces, 
    // defined by the following vertex indices:
    0,  55, 25,
    1,  26,  5,
    2,  6, 35,
    3,  36, 50,
    4,  51, 56,
    7,  27, 45,
    28, 57, 20,
    58, 52, 10,
    53, 37, 30,
    38,  8, 40,    
    15, 46, 21,
    16, 22, 11,
    17, 12, 31,
    18, 32, 41,
    19, 42, 47,
    23, 48, 29,
    13, 24, 59,
    33, 14, 54,
    43, 34, 39,
    49, 44, 9
  };
	
  
  float normals[3*num_verts] = {0}; //4 elements, 3 copies, 8 rows
	std::vector<glm::vec4> nmVector;
  
  for(int i = 0; i < num_indices; i=i+3){
    int p1_index = faces[i];  //Get index for each vertices forming the triangle
    int p2_index = faces[i+1];
    int p3_index = faces[i+2];
    
    //Compute normal for the triangle
    glm::vec4 u = glm::vec4(points[3*p2_index],points[3*p2_index+1],points[3*p2_index+2],0) - glm::vec4(points[3*p1_index],points[3*p1_index+1],points[3*p1_index+2],0);
    glm::vec4 v = glm::vec4(points[3*p3_index],points[3*p3_index+1],points[3*p3_index+2],0) - glm::vec4(points[3*p1_index],points[3*p1_index+1],points[3*p1_index+2],0);

    glm::vec4 nm = glm::vec4(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x,0 );
    nmVector.insert(nmVector.end(),nm);
   
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
  
  
    GLuint VAO,VBO,EBO,nmVBO, uvVBO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&nmVBO);
    glGenBuffers(1,&uvVBO);
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
    //uv_coords
    glBindBuffer(GL_ARRAY_BUFFER,uvVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(uv_coords),uv_coords,GL_STATIC_DRAW);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2*sizeof(GLfloat),(void*)0);
  
    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(faces),faces,GL_STATIC_DRAW);
    
  
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    // Loading textures--------------------------------------------------------------------------------------------------------------------------------------//
  
    GLuint texture_handle;
    glGenTextures(1,&texture_handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture_handle);
  
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
    
    unsigned char* image_data;
    unsigned image_w;
    unsigned image_h;
  
    unsigned image_file = 
      lodepng_decode32_file(&image_data,&image_w, &image_h,
                            "../../common/data/numberline_hires.png");
    std::cout << "Read " << image_h << " x " << image_w << " image\n";                          
      glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
      glGenerateMipmap(GL_TEXTURE_2D);
    
    glUniform1i(glGetUniformLocation(shader0.shader_program, "tex_sampler"),0);
     
    
    //-------------------------------------------------------------------------------------------------------------------------------------------------------//
  
  
  
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
// load and compile shaders  "../lab1-7_vs.glsl" and "../lab1-7_fs.glsl"
//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
  shader0.update("../lab3-1_vs.glsl","../lab3-1_fs.glsl");

  
 
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
    
     //Send light
    GLuint light_countLoc = glGetUniformLocation(shader0.shader_program,"light_count");
    glUniform1i(light_countLoc,light_count);
    
    //light position
    glm::vec4 light_position[4] = { glm::vec4(0, 0, 5.0f, 0),
                                    glm::vec4(0, 5.0f, 0, 0),
                                    glm::vec4(-5.0f,0 , 0, 0),
                                    glm::vec4(5.0f,5.0f,5.0f,0) };
    
    GLuint light_positionLoc = glGetUniformLocation(shader0.shader_program,"light_position");
    glUniform4fv(light_positionLoc,4,glm::value_ptr(light_position[0]));
   
    //Light colour
    glm::vec4 light_colour[4] = { glm::vec4(1.0f,1.0f,1.0f,0),
                                  glm::vec4(1.0f,1.0f,1.0f,0),
                                  glm::vec4(1.0f,1.0f,1.0f,0),
                                  glm::vec4(1.0f,1.0f,1.0f,0) };
    GLuint light_colourLoc = glGetUniformLocation(shader0.shader_program,"light_colour");
    glUniform4fv(light_colourLoc,4,glm::value_ptr(light_colour[0]));
    
	  //Roughness factor
  	glUniform1f(glGetUniformLocation(shader0.shader_program,"roughness_factor"),roughness_factor);
    
    //Viewer position
  	glm::vec3 view_pos(0.0f,0.0f,2.0f);
  	glUniform3fv(glGetUniformLocation(shader0.shader_program,"view_pos"),1,glm::value_ptr(view_pos));
	  
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
    glDrawElements(GL_TRIANGLES,num_indices,GL_UNSIGNED_SHORT,0);
    //glDrawArrays(GL_TRIANGLES,0,shapes[0].mesh.positions.size());
    glfwSwapBuffers (window);
    
  }

  // close GL context and any other GLFW resources
  glfwTerminate();
  exit(EXIT_SUCCESS);
}




