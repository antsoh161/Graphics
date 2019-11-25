#ifndef MY_SHADER
#define MY_SHADER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h> 

#include <cstdlib>
#include <iostream>
#include <readfile.hpp>

//Simple shader class holding fragment and vertice shader
//read from .glsl-files using readfile.hpp

class my_shader{
public:
		GLuint shader_program; 			// ID
		GLuint count;					//Counts updates for debugging
		GLuint position_offset_loc;		//Used in lab1-4
		GLfloat x_offset,y_offset;		//Used in lab1-4
		GLfloat modifier_loc; 			
		GLfloat modifier;	
		GLfloat x_rot_angle;
		GLfloat y_rot_angle;
	
        my_shader();
		~my_shader(){};
        void update(const char* vsPath, const char* fsPath);
		void checkShaderCompileError(GLint shaderID);
};


my_shader::my_shader(){
	count = 0;
	x_offset = 0; y_offset = 0;
	modifier = 0.5;
	x_rot_angle = 0; y_rot_angle = 0;
}

//Read from files and update
//shader_program is created on the first update
void my_shader::update(const char* vsPath, const char* fsPath){
	
	this->modifier = 0.5;
	
	std::string vertex_shader_str = readFile(vsPath);
	std::string fragment_shader_str = readFile(fsPath);
	const char *vertex_shader_src = vertex_shader_str.c_str();
	const char *fragment_shader_src = fragment_shader_str.c_str();
	GLuint vs = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vs,1,&vertex_shader_src,NULL);
	glCompileShader (vs);
	GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fs, 1, &fragment_shader_src, NULL);
	glCompileShader (fs);
	
	std::cout <<"fs ";
	checkShaderCompileError(fs);
	std::cout <<"vs ";
	checkShaderCompileError(vs);
	
	shader_program = glCreateProgram();
	
	glAttachShader (this->shader_program, fs);
	glAttachShader (shader_program, vs);
	glLinkProgram (shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	
	glUseProgram(shader_program);
}

void my_shader::checkShaderCompileError(GLint shaderID){
     GLint isCompiled = 0;
     glGetShaderiv(shaderID,GL_COMPILE_STATUS,&isCompiled);

     if(isCompiled == GL_FALSE){
		   GLint maxLength = 0;
		   glGetShaderiv(shaderID,GL_INFO_LOG_LENGTH,&maxLength);

		   //maxlength includes null
		   std::string errorLog;
		   errorLog.resize(maxLength);
		   glGetShaderInfoLog(shaderID,maxLength,&maxLength,&errorLog[0]);

		   std::cout <<"shader compilation failed" <<std::endl;
		   std::cout << errorLog <<std::endl;
		   return;
     }
     else
	std::cout << "shader compilation success" <<std::endl;
	return;

}

#endif