#pragma once
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <cstring>
#include <fstream>
#include "Mesh.h"

#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2
#define COLORS 3

namespace engine
{
	class Shader
	{
	public:
		GLuint VertexShaderId, FragmentShaderId, ProgramId = 0;
		GLint ModelMatrix_UId, ViewMatrix_UId, ProjectionMatrix_UId, Color_UId;
		const GLuint UBO_BP = 0;
		Mesh* mesh;
	public:
		void createShaderProgram(const std::string& vertexShaderF, const std::string& fragmentShaderF);
		void destroyShaderProgram();
	protected:
		const GLuint loadShader(const GLuint program_id, const GLenum shader_type, const std::string& filename);
		const std::string read(const std::string& filename);
		const GLuint checkCompilationError(const GLuint shader_id, const std::string& filename);
		void checkLinkageError(const GLuint program_id);
	};
}
