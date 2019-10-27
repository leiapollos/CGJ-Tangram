///////////////////////////////////////////////////////////////////////////////
//
// Drawing two instances of a triangle in Modern OpenGL.
// A "hello world" of Modern OpenGL.
//
// Assignment : Create Shader Abstraction 
//					(e.g. check compilation/linkage for errors, read from file) 
//			  : Manage Multiple Drawable Entities (using your vertex and matrix classes)
//              Draw a set of 7 TANs (i.e. TANGRAM shapes) of different colors: 
//              (1) 3 different TAN geometric shapes at the origin:
//					- right triangle
//					- square
//					- parallelogram
//              (2) 7 TANs of different colors put together to form a shape of your choice:
//					- 2 big right triangles
//					- 1 medium right triangle
//					- 2 small right triangles
//					- 1 square
//					- 1 parallelogram;
//
// (c)2013-19 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Vector.h"
#include "Matrix.h"
#include "Shader.h"

#define VERTICES 0
#define COLORS 1

using namespace engine;

GLuint VaoId, VboId[2];
GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UniformId;
Shader shader;

#define ERROR_CALLBACK
#ifdef  ERROR_CALLBACK

////////////////////////////////////////////////// ERROR CALLBACK (OpenGL 4.3+)

static const std::string errorSource(GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:				return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "window system";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "shader compiler";
	case GL_DEBUG_SOURCE_THIRD_PARTY:		return "third party";
	case GL_DEBUG_SOURCE_APPLICATION:		return "application";
	case GL_DEBUG_SOURCE_OTHER:				return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorType(GLenum type)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:				return "error";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "deprecated behavior";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "undefined behavior";
	case GL_DEBUG_TYPE_PORTABILITY:			return "portability issue";
	case GL_DEBUG_TYPE_PERFORMANCE:			return "performance issue";
	case GL_DEBUG_TYPE_MARKER:				return "stream annotation";
	case GL_DEBUG_TYPE_PUSH_GROUP:			return "push group";
	case GL_DEBUG_TYPE_POP_GROUP:			return "pop group";
	case GL_DEBUG_TYPE_OTHER_ARB:			return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorSeverity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:			return "high";
	case GL_DEBUG_SEVERITY_MEDIUM:			return "medium";
	case GL_DEBUG_SEVERITY_LOW:				return "low";
	case GL_DEBUG_SEVERITY_NOTIFICATION:	return "notification";
	default:								exit(EXIT_FAILURE);
	}
}

static void error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam)
{
	std::cerr << "GL ERROR:" << std::endl;
	std::cerr << "  source:     " << errorSource(source) << std::endl;
	std::cerr << "  type:       " << errorType(type) << std::endl;
	std::cerr << "  severity:   " << errorSeverity(severity) << std::endl;
	std::cerr << "  debug call: " << std::endl << message << std::endl;
	std::cerr << "Press <return>.";
	//std::cin.ignore(); //TODO uncomment this to not get error flooding
}

void setupErrorCallback()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(error, 0);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
	// params: source, type, severity, count, ids, enabled
}

#else

///////////////////////////////////////////////// ERROR HANDLING (All versions)

static const std::string errorString(GLenum error)
{
	switch (error) {
	case GL_NO_ERROR:
		return "No error has been recorded.";
	case GL_INVALID_ENUM:
		return "An unacceptable value is specified for an enumerated argument.";
	case GL_INVALID_VALUE:
		return "A numeric argument is out of range.";
	case GL_INVALID_OPERATION:
		return "The specified operation is not allowed in the current state.";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "The framebuffer object is not complete.";
	case GL_OUT_OF_MEMORY:
		return "There is not enough memory left to execute the command.";
	case GL_STACK_UNDERFLOW:
		return "An attempt has been made to perform an operation that would cause an internal stack to underflow.";
	case GL_STACK_OVERFLOW:
		return "An attempt has been made to perform an operation that would cause an internal stack to overflow.";
	default: exit(EXIT_FAILURE);
	}
}

static bool isOpenGLError()
{
	bool isError = false;
	GLenum errCode;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		std::cerr << "OpenGL ERROR [" << errorString(errCode) << "]." << std::endl;
	}
	return isError;
}

static void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

#endif // ERROR_CALLBACK

/////////////////////////////////////////////////////////////////////// SHADERs


/////////////////////////////////////////////////////////////////////// VAOs & VBOs

typedef struct
{
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

vec4 vblue = vec4(0.0f, 0.0f, 1.0f, 1.0f);
float* blue = vblue.getData();
vec4 vred = vec4(1.0f, 0.0f, 0.0f, 1.0f);
float* red = vred.getData();
vec4 vgreen = vec4(0.0f, 1.0f, 0.0f, 1.0f);
float* green = vgreen.getData();
vec4 vyellow = vec4(1.0f, 1.0f, 0.0f, 1.0f);
float* yellow = vyellow.getData();
vec4 vorange = vec4(0.8f, 0.4f, 0.0f, 1.0f);
float* orange = vorange.getData();
vec4 vcyan = vec4(0.0f, 1.0f, 1.0f, 1.0f);
float* cyan = vcyan.getData();
vec4 vmagenta = vec4(1.0f, 0.0f, 1.0f, 1.0f);
float* magenta = vmagenta.getData();

vec4 vv1 = vec4(0.0f, 0.0f, 0.0f, 1.0f);
vec4 vv2 = vec4(0.5f, 0.0f, 0.0f, 1.0f);
vec4 vv3 = vec4(0.5f, 0.5f, 0.0f, 1.0f);
vec4 vv4 = vec4(0.0f, 0.5f, 0.0f, 1.0f);
float* v1 = vv1.getData();
float* v2 = vv2.getData();
float* v3 = vv3.getData();
float* v4 = vv4.getData();

vec4 vv5 = vec4(0.0f, 0.0f, 0.0f, 1.0f);
vec4 vv6 = vec4(0.0f, 0.5f, 0.0f, 1.0f);
vec4 vv7 = vec4(0.5f, 0.25f, 0.0f, 1.0f);
vec4 vv8 = vec4(0.5f, 0.75f, 0.0f, 1.0f);
float* v5 = vv5.getData();
float* v6 = vv6.getData();
float* v7 = vv7.getData();
float* v8 = vv8.getData();

vec4 vv9 = vec4(0.0f, 0.0f, 0.0f, 1.0f);
vec4 vv10 = vec4(0.25f, 0.0f, 0.0f, 1.0f);
vec4 vv11 = vec4(0.25f, 0.25f, 0.0f, 1.0f);
float* v9 = vv9.getData();
float* v10 = vv10.getData();
float* v11 = vv11.getData();


const Vertex Vertices[] =
{
	{{  v1[0], v1[1], v1[2], v1[3]}, { blue[0], blue[1], blue[2], blue[3] } }, //0
	{{  v2[0], v2[1], v2[2], v2[3]}, { blue[0], blue[1], blue[2], blue[3] }}, //1
	{{ v3[0], v3[1], v3[2], v3[3] },{ blue[0], blue[1], blue[2], blue[3] }}, //2
	{{ v4[0], v4[1], v4[2], v4[3] }, { blue[0], blue[1], blue[2], blue[3] }}, //3


	{{ v5[0], v5[1], v5[2], v5[3] }, { red[0], red[1], red[2], red[3] }}, //4
	{{ v6[0], v6[1], v6[2], v6[3] }, { red[0], red[1], red[2], red[3] }}, //5
	{{ v7[0], v7[1], v7[2], v7[3] }, { red[0], red[1], red[2], red[3] }}, //6
	{{ v8[0], v8[1], v8[2], v8[3] }, { red[0], red[1], red[2], red[3] }}, //7


	{{ v9[0], v9[1], v9[2], v9[3] }, { green[0], green[1], green[2], green[3] }}, //8
	{{ v10[0], v10[1], v10[2], v10[3] }, { green[0], green[1], green[2], green[3] }}, //9
	{{ v11[0], v11[1], v11[2], v11[3] }, { green[0], green[1], green[2], green[3] }} //10
};

const GLubyte Indices[] =
{
	0,1,2,
	2,3,0, //6

	4,5,7,
	4,6,7, //12

	8,9,10  //15
};

void createBufferObjects()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(2, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(VERTICES);
			glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glEnableVertexAttribArray(COLORS);
			glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vertices[0].XYZW));
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
#endif
}

void destroyBufferObjects()
{
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDeleteBuffers(2, VboId);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
#endif
}

/////////////////////////////////////////////////////////////////////// SCENE

mat4 I = MatrixFactory::createIdentityMat4();
mat4 BlueParallelogram;
mat4 RedTriangle;
mat4 MagentaTriangle;
mat4 CyanTriangle;
mat4 GreenTriangle;
mat4 YellowTriangle;
mat4 OrangeSquare;

void drawScene()
{
	glBindVertexArray(VaoId);
	glUseProgram(shader.ProgramId);

	YellowTriangle =  MatrixFactory::createTranslationMat4(vec3(-0.5f, 0.25f,0.0f));
	YellowTriangle = YellowTriangle * MatrixFactory::createScaleMat4(vec3(0.5f, 0.5f, 1.0f));
	YellowTriangle = YellowTriangle * MatrixFactory::createRotationMat4(90.0f, vec3(0.0f,0.0f,1.0f));
	GreenTriangle = MatrixFactory::createTranslationMat4(vec3(-0.75f, 0.5f, 0.0f));
	GreenTriangle = GreenTriangle * MatrixFactory::createScaleMat4(vec3(0.5f, 0.5f, 1.0f));

	CyanTriangle = MatrixFactory::createTranslationMat4(vec3(-0.625f,0.376f,0.0f));
	CyanTriangle = CyanTriangle * MatrixFactory::createRotationMat4(45.0f, vec3(0.0f,0.0f,1.0f));
	CyanTriangle = CyanTriangle * MatrixFactory::createScaleMat4(vec3(0.71f, 0.71f, 1.0f));

	RedTriangle = MatrixFactory::createTranslationMat4(vec3(-0.625f, 0.376f,0.0f));

	MagentaTriangle = MatrixFactory::createTranslationMat4(vec3(-0.125f, 0.626f, 0.0f));
	MagentaTriangle = MagentaTriangle * MatrixFactory::createRotationMat4(180.0f, vec3(0.0f,0.0f,1.0f));

	OrangeSquare = MatrixFactory::createTranslationMat4(vec3(-0.5f, 0.25f, 0.0f));
	OrangeSquare = OrangeSquare * MatrixFactory::createScaleMat4(vec3(0.25f,0.25f,1.0f));


	BlueParallelogram = MatrixFactory::createTranslationMat4(vec3(-0.50f,0.25f,0.0f));
	BlueParallelogram = BlueParallelogram * MatrixFactory::createScaleMat4(vec3(0.25f,0.32f,1.0f));
	BlueParallelogram = BlueParallelogram * MatrixFactory::createRotationMat4(-90.0f, vec3(0.0f,0.0f,1.0f));



	glUniformMatrix4fv(shader.MUniformId, 1, GL_FALSE, OrangeSquare.getData()); //SQUARE
	glUniform4fv(shader.CUniformId, 1, orange);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
	
	glUniformMatrix4fv(shader.MUniformId, 1, GL_FALSE, BlueParallelogram.getData()); //PARALLELOGRAM
	glUniform4fv(shader.CUniformId, 1, blue);
	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, (GLvoid*)6);

	glUniformMatrix4fv(shader.MUniformId, 1, GL_FALSE, CyanTriangle.getData()); //CYAN
	glUniform4fv(shader.CUniformId, 1, cyan);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)12);

	glUniformMatrix4fv(shader.MUniformId, 1, GL_FALSE, RedTriangle.getData()); //RED
	glUniform4fv(shader.CUniformId, 1, red);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)12);

	glUniformMatrix4fv(shader.MUniformId, 1, GL_FALSE, MagentaTriangle.getData()); //MAGENTA
	glUniform4fv(shader.CUniformId, 1, magenta);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)12);

	glUniformMatrix4fv(shader.MUniformId, 1, GL_FALSE, GreenTriangle.getData()); //GREEN
	glUniform4fv(shader.CUniformId, 1, green);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)12);

	glUniformMatrix4fv(shader.MUniformId, 1, GL_FALSE, YellowTriangle.getData()); //YELLOW
	glUniform4fv(shader.CUniformId, 1, yellow);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)12);

	glUseProgram(0);
	glBindVertexArray(0);

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not draw scene.");
#endif
}

///////////////////////////////////////////////////////////////////// CALLBACKS

void window_close_callback(GLFWwindow* win)
{
	shader.destroyShaderProgram();
	destroyBufferObjects();
}

void window_size_callback(GLFWwindow* win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
}

///////////////////////////////////////////////////////////////////////// SETUP

void glfw_error_callback(int error, const char* description)
{
	std::cerr << "GLFW Error: " << description << std::endl;
}

GLFWwindow* setupWindow(int winx, int winy, const char* title,
	int is_fullscreen, int is_vsync)
{
	GLFWmonitor* monitor = is_fullscreen ? glfwGetPrimaryMonitor() : 0;
	GLFWwindow* win = glfwCreateWindow(winx, winy, title, monitor, 0);
	if (!win)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(win);
	glfwSwapInterval(is_vsync);
	return win;
}

void setupCallbacks(GLFWwindow* win)
{
	glfwSetWindowCloseCallback(win, window_close_callback);
	glfwSetWindowSizeCallback(win, window_size_callback);
}

GLFWwindow* setupGLFW(int gl_major, int gl_minor,
	int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	GLFWwindow* win = setupWindow(winx, winy, title, is_fullscreen, is_vsync);
	setupCallbacks(win);

#if _DEBUG
	std::cout << "GLFW " << glfwGetVersionString() << std::endl;
#endif
	return win;
}

void setupGLEW()
{
	glewExperimental = GL_TRUE;
	// Allow extension entry points to be loaded even if the extension isn't 
	// present in the driver's extensions string.
	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	// You might get GL_INVALID_ENUM when loading GLEW.
}

void checkOpenGLInfo()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL(int winx, int winy)
{
#if _DEBUG
	checkOpenGLInfo();
#endif
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glViewport(0, 0, winx, winy);
}

GLFWwindow* setup(int major, int minor,
	int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	GLFWwindow* win =
		setupGLFW(major, minor, winx, winy, title, is_fullscreen, is_vsync);
	setupGLEW();
	setupOpenGL(winx, winy);
#ifdef ERROR_CALLBACK
	setupErrorCallback();
#endif
	shader = Shader();
	shader.createShaderProgram("shaders/vertex.shader", "shaders/fragment.shader");
	createBufferObjects();
	return win;
}

////////////////////////////////////////////////////////////////////////// RUN

void display(GLFWwindow* win, double elapsed_sec)
{
	drawScene();
}

void run(GLFWwindow* win)
{
	double last_time = glfwGetTime();
	while (!glfwWindowShouldClose(win))
	{
		double time = glfwGetTime();
		double elapsed_time = time - last_time;
		last_time = time;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		display(win, elapsed_time);
		glfwSwapBuffers(win);
		glfwPollEvents();
#ifndef ERROR_CALLBACK
		checkOpenGLError("ERROR: MAIN/RUN");
#endif
	}
	glfwDestroyWindow(win);
	glfwTerminate();
}

////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[])
{
	int gl_major = 4, gl_minor = 3;
	int is_fullscreen = 0;
	int is_vsync = 1;
	GLFWwindow* win = setup(gl_major, gl_minor,
		480, 480, "Hello Modern 2D World", is_fullscreen, is_vsync); //TODO change back to 640
	run(win);
	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END