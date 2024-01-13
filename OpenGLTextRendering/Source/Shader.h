#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<GL/glew.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include <GLM/ext/matrix_float4x4.hpp>

namespace Echo
{
	std::string get_file_contents(const char* filename);

	class Shader
	{
	public:
		// Reference ID of the Shader Program
		GLuint ID;
		// Constructor that build the Shader Program from 2 different shaders
		Shader();
		Shader(const char* vertexFile, const char* fragmentFile);

		// Activates the Shader Program
		void Activate();
		// Deletes the Shader Program
		void Delete();

		void SetShaderUniform4V(const char* name, glm::mat4 value);

	private:
		// Checks if the different Shaders have compiled properly
		void compileErrors(unsigned int shader, const char* type);
	};
}

#endif