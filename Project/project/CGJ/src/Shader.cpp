#include "Shader.h"

void engine::Shader::createShaderProgram(const std::string& vertexShaderF, const std::string& fragmentShaderF)
{
	std::string source;

	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	source = loadShader(vertexShaderF);
	const char* VertexShader = source.c_str();
	glShaderSource(VertexShaderId, 1, &VertexShader, 0);
	glCompileShader(VertexShaderId);
	checkCompilationError(VertexShaderId);

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	source = loadShader(fragmentShaderF);
	const char* FragmentShader = source.c_str();
	glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
	glCompileShader(FragmentShaderId);
	checkCompilationError(FragmentShaderId);

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);

	glBindAttribLocation(ProgramId, VERTICES, "in_Position");
	glBindAttribLocation(ProgramId, COLORS, "in_Color");

	glLinkProgram(ProgramId);
	checkLinkageError();
	UniformId = glGetUniformLocation(ProgramId, "ModelMatrix");
	UboId = glGetUniformBlockIndex(ProgramId, "SharedMatrices");
	glUniformBlockBinding(ProgramId, UboId, UBO_BP);

	glDetachShader(ProgramId, VertexShaderId);
	glDeleteShader(VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);
	glDeleteShader(FragmentShaderId);

}

std::string engine::Shader::loadShader(const std::string& path)
{
	std::ifstream stream(path);
	std::string shader="", line;
	while (std::getline(stream, line)) {
		shader += line + "\n";
	}
	return shader;
}

void engine::Shader::destroyShaderProgram()
{
	glUseProgram(0);
	glDeleteProgram(ProgramId);
}

void engine::Shader::checkCompilationError(GLuint shader)
{
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		char infolog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, infolog);
		std::cout << "The vertex shader failed to compile with the error:" << infolog << std::endl;
	}
}

void engine::Shader::checkLinkageError()
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
