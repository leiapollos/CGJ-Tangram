#pragma once
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <cstring>
#include <fstream>

#define VERTICES 0
#define COLORS 1

namespace engine
{
	class Shader
	{
	public:
		GLuint VertexShaderId, FragmentShaderId, ProgramId = 0;
	public:
		GLint MUniformId, CUniformId;
		void createShaderProgram(const std::string& vertexShaderF, const std::string& fragmentShaderF);
		std::string loadShader(const std::string& file);
		void destroyShaderProgram();
		void checkCompilationError(GLuint shader);
		void checkLinkageError();
	};
}
