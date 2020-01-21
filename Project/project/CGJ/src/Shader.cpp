#include "Shader.h"

void engine::Shader::createShaderProgram(const std::string& vs_file, const std::string& fs_file)
{
	ProgramId = glCreateProgram();

	GLuint VertexShaderId = loadShader(ProgramId, GL_VERTEX_SHADER, vs_file);
	GLuint FragmentShaderId = loadShader(ProgramId, GL_FRAGMENT_SHADER, fs_file);

	glBindAttribLocation(ProgramId, VERTICES, "inPosition");
	if (mesh->TexcoordsLoaded)
		glBindAttribLocation(ProgramId, TEXCOORDS, "inTexcoord");
	if (mesh->NormalsLoaded)
		glBindAttribLocation(ProgramId, NORMALS, "inNormal");

	glLinkProgram(ProgramId);
	checkLinkageError(ProgramId);

	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);
	glDeleteShader(VertexShaderId);
	glDeleteShader(FragmentShaderId);

	ModelMatrix_UId = glGetUniformLocation(ProgramId, "ModelMatrix");
	ViewMatrix_UId = glGetUniformLocation(ProgramId, "ViewMatrix");
	ProjectionMatrix_UId = glGetUniformLocation(ProgramId, "ProjectionMatrix");
	Color_UId = glGetUniformLocation(ProgramId, "u_Color");

}

const GLuint engine::Shader::loadShader(const GLuint program_id, const GLenum shader_type, const std::string& filename)
{
	const GLuint shader_id = glCreateShader(shader_type);
	const std::string scode = read(filename);
	const GLchar* code = scode.c_str();
	glShaderSource(shader_id, 1, &code, 0);
	glCompileShader(shader_id);
	checkCompilationError(shader_id, filename);
	glAttachShader(program_id, shader_id);
	return shader_id;
}

const std::string engine::Shader::read(const std::string& filename)
{
	std::ifstream ifile(filename);
	std::string shader_string, line;
	while (std::getline(ifile, line))
	{
		shader_string += line + "\n";
	}
	return shader_string;
}

void engine::Shader::destroyShaderProgram()
{
	glUseProgram(0);
	glDeleteProgram(ProgramId);
}

const GLuint engine::Shader::checkCompilationError(const GLuint shader_id, const std::string& filename)
{
	GLint compiled;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
		GLchar* const log = new char[length];
		glGetShaderInfoLog(shader_id, length, &length, log);
		std::cerr << "[" << filename << "] " << std::endl << log;
		delete[] log;
	}
	return compiled;
}

void engine::Shader::checkLinkageError(const GLuint program_id)
{
	GLint linked;
	glGetProgramiv(ProgramId, GL_LINK_STATUS, &linked);
	if (linked == GL_FALSE)
	{
		char infolog[1024];
		glGetProgramInfoLog(ProgramId, 1024, NULL, infolog);
		std::cout << "The program failed to link with the error:" << infolog << std::endl;
	}
}
