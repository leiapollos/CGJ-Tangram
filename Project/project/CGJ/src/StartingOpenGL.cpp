///////////////////////////////////////////////////////////////////////////////
//
// Assignment consists in the following:
//
// - Create the following changes to your scene:
//   - Make your TANs double-faced, so they can be seen from both sides.
//   - The new face of each TAN should share the same hue as the original top
//     face color but have a different level of saturation and brightness.
//
// (c) 2013-19 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Matrix.h"
#include "Vector.h"
#include "Shader.h"
#include "Camera.h"

#define VERTICES 0
#define COLORS 1

using namespace engine;

GLuint VaoId, VboId[3];
Shader shader;
Camera camera;
bool firstMouse = true;
float lastX, lastY;

float blue[4], orange[4], red[4], yellow[4], cyan[4], magenta[4], green[4];
float nblue[4], norange[4], nred[4], nyellow[4], ncyan[4], nmagenta[4], ngreen[4];

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
	std::cerr << "  debug call: " << std::endl << message << std::endl << std::endl;
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

/////////////////////////////////////////////////////////////////////// SHADERs



/////////////////////////////////////////////////////////////////// VAOs & VBOs

typedef struct {
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

typedef GLfloat Matrix[16];


const Vertex Vertices[] =
{
	{{ 0.0f, 0.0f, 0.0f, 1.0f }, { blue[0], blue[1], blue[2], blue[3] } }, //0
	{{ 0.5f, 0.0f, 0.0f, 1.0f }, { blue[0], blue[1], blue[2], blue[3] }}, //1
	{{ 0.5f, 0.5f, 0.0f, 1.0f },{ blue[0], blue[1], blue[2], blue[3] }}, //2
	{{ 0.0f, 0.5f, 0.0f, 1.0f }, { blue[0], blue[1], blue[2], blue[3] }}, //3


	{{ 0.0f, 0.0f, 0.0f, 1.0f }, { red[0], red[1], red[2], red[3] }}, //4
	{{ 0.0f, 0.5f, 0.0f, 1.0f }, { red[0], red[1], red[2], red[3] }}, //5
	{{ 0.5f, 0.25f, 0.0f, 1.0f }, { red[0], red[1], red[2], red[3] }}, //6
	{{ 0.5f, 0.75f, 0.0f, 1.0f }, { red[0], red[1], red[2], red[3] }}, //7


	{{ 0.0f, 0.0f, 0.0f, 1.0f }, { green[0], green[1], green[2], green[3] }}, //8
	{{ 0.25f, 0.0f, 0.0f, 1.0f }, { green[0], green[1], green[2], green[3] }}, //9
	{{ 0.25f, 0.25f, 0.0f, 1.0f }, { green[0], green[1], green[2], green[3] }} //10
};

const GLubyte Indices[] =
{
	0,1,2,
	2,3,0, //6

	6,5,4,
	5,6,7, //12

	8,9,10,  //15
	
	2,1,0,
	0,3,2, //21

	4,5,7,
	7,6,4, //27

	10,9,8 //30

};

/*const Vertex Vertices[] = // no indices?
{
	{{ 0.0f, 0.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f }}, // 0 - FRONT
	{{ 1.0f, 0.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f }}, // 1
	{{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f }}, // 2
	{{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f }}, // 2	
	{{ 0.0f, 1.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f }}, // 3
	{{ 0.0f, 0.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f }}, // 0

	{{ 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f }}, // 1 - RIGHT
	{{ 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f }}, // 5
	{{ 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f }}, // 6
	{{ 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f }}, // 6	
	{{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f }}, // 2
	{{ 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f }}, // 1

	{{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f }}, // 2 - TOP
	{{ 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f }}, // 6
	{{ 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f }}, // 7
	{{ 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f }}, // 7	
	{{ 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f }}, // 3
	{{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f }}, // 2

	{{ 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.9f, 1.0f }}, // 5 - BACK
	{{ 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.9f, 1.0f }}, // 4
	{{ 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.9f, 1.0f }}, // 7
	{{ 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.9f, 1.0f }}, // 7	
	{{ 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.9f, 1.0f }}, // 6
	{{ 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.9f, 1.0f }}, // 5

	{{ 0.0f, 0.0f, 0.0f, 1.0f }, { 0.9f, 0.0f, 0.9f, 1.0f }}, // 4 - LEFT
	{{ 0.0f, 0.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.9f, 1.0f }}, // 0
	{{ 0.0f, 1.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.9f, 1.0f }}, // 3
	{{ 0.0f, 1.0f, 1.0f, 1.0f }, { 0.9f, 0.0f, 0.9f, 1.0f }}, // 3	
	{{ 0.0f, 1.0f, 0.0f, 1.0f }, { 0.9f, 0.0f, 0.9f, 1.0f }}, // 7
	{{ 0.0f, 0.0f, 0.0f, 1.0f }, { 0.9f, 0.0f, 0.9f, 1.0f }}, // 4

	{{ 0.0f, 0.0f, 1.0f, 1.0f }, { 0.9f, 0.9f, 0.0f, 1.0f }}, // 0 - BOTTOM
	{{ 0.0f, 0.0f, 0.0f, 1.0f }, { 0.9f, 0.9f, 0.0f, 1.0f }}, // 4
	{{ 1.0f, 0.0f, 0.0f, 1.0f }, { 0.9f, 0.9f, 0.0f, 1.0f }}, // 5
	{{ 1.0f, 0.0f, 0.0f, 1.0f }, { 0.9f, 0.9f, 0.0f, 1.0f }}, // 5	
	{{ 1.0f, 0.0f, 1.0f, 1.0f }, { 0.9f, 0.9f, 0.0f, 1.0f }}, // 1
	{{ 0.0f, 0.0f, 1.0f, 1.0f }, { 0.9f, 0.9f, 0.0f, 1.0f }}  // 0
};*/

void createBufferObjects()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(3, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(VERTICES);
			glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glEnableVertexAttribArray(COLORS);
			glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vertices[0].XYZW));
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[2]);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
	{
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrix) * 2, 0, GL_STREAM_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, shader.UBO_BP, VboId[1]);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void destroyBufferObjects()
{
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDeleteBuffers(3, VboId);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindVertexArray(0);
}

/////////////////////////////////////////////////////////////////////// SCENE

const Matrix I = {
	1.0f,  0.0f,  0.0f,  0.0f,
	0.0f,  1.0f,  0.0f,  0.0f,
	0.0f,  0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
};

const Matrix ModelMatrix = {
	1.0f,  0.0f,  0.0f,  0.0f,
	0.0f,  1.0f,  0.0f,  0.0f,
	0.0f,  0.0f,  1.0f,  0.0f,
   -0.5f, -0.5f, -0.5f,  1.0f
}; // Column Major

// Eye(5,5,5) Center(0,0,0) Up(0,1,0)

// Eye(-5,-5,-5) Center(0,0,0) Up(0,1,0)

// Orthographic LeftRight(-2,2) BottomTop(-2,2) NearFar(1,10)

// Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(10)

mat4 BlueParallelogram;
mat4 RedTriangle;
mat4 MagentaTriangle;
mat4 CyanTriangle;
mat4 GreenTriangle;
mat4 YellowTriangle;
mat4 OrangeSquare;

void drawScene()
{

	camera.createAndSetViewMatrix(camera.eye, camera.eye + camera.direction, camera.up);
	mat4 viewMatrix = camera.getViewMatrix();
	mat4 projectionMatrix = camera.getProjectionMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix), viewMatrix.getData());
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix), sizeof(Matrix), projectionMatrix.getData());
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindVertexArray(VaoId);
	glUseProgram(shader.ProgramId);

	//FRONT
	glUniformMatrix4fv(shader.MUniformId, 1, GL_FALSE, OrangeSquare.getData()); //SQUARE
	glUniform4fv(shader.CUniformId, 1, orange);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glUniformMatrix4fv(shader.MUniformId, 1, GL_FALSE, BlueParallelogram.getData()); //PARALLELOGRAM
	glUniform4fv(shader.CUniformId, 1, blue);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)6);

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

	//BACK
	glUniformMatrix4fv(shader.MUniformId, 1, GL_FALSE, OrangeSquare.getData()); //SQUARE
	glUniform4fv(shader.CUniformId, 1, norange);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)15);

	glUniformMatrix4fv(shader.MUniformId, 1, GL_FALSE, BlueParallelogram.getData()); //PARALLELOGRAM
	glUniform4fv(shader.CUniformId, 1, nblue);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)21);

	glUniformMatrix4fv(shader.MUniformId, 1, GL_FALSE, CyanTriangle.getData()); //CYAN
	glUniform4fv(shader.CUniformId, 1, ncyan);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)27);

	glUniformMatrix4fv(shader.MUniformId, 1, GL_FALSE, RedTriangle.getData()); //RED
	glUniform4fv(shader.CUniformId, 1, nred);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)27);

	glUniformMatrix4fv(shader.MUniformId, 1, GL_FALSE, MagentaTriangle.getData()); //MAGENTA
	glUniform4fv(shader.CUniformId, 1, nmagenta);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)27);

	glUniformMatrix4fv(shader.MUniformId, 1, GL_FALSE, GreenTriangle.getData()); //GREEN
	glUniform4fv(shader.CUniformId, 1, ngreen);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)27);

	glUniformMatrix4fv(shader.MUniformId, 1, GL_FALSE, YellowTriangle.getData()); //YELLOW
	glUniform4fv(shader.CUniformId, 1, nyellow);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)27);


	glUseProgram(0);
	glBindVertexArray(0);
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

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	//std::cout << "key: " << key << " " << scancode << " " << action << " " << mods << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(win, GLFW_TRUE);
		window_close_callback(win);
	}
	if (key == GLFW_KEY_P && action == 1) {
		camera.switchProjectionMatrix();
	}
	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
		camera.eye += camera.direction * camera.speed;
	}
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
		camera.eye -= camera.direction * camera.speed;
	}
	if (key == GLFW_KEY_A && (action == 2 || action == 1)) {
		camera.eye -= normalize(cross(camera.direction, camera.up)) * camera.speed;
	}
	if (key == GLFW_KEY_D && (action == 2 || action == 1)) {
		camera.eye += normalize(cross(camera.direction,camera.up)) * camera.speed;
	}

	//RESET CAMERA
	if (key == GLFW_KEY_R) {
		camera = Camera();
	}

	if (key == GLFW_KEY_F) {
		std::cout << camera.eye << camera.direction << camera.yaw << " " <<camera.pitch << std::endl;
	}
}

void mouse_callback(GLFWwindow* win, double xpos, double ypos)
{
	//std::cout << "mouse: " << xpos << " " << ypos << std::endl;
	camera.mouseCallBack((float)xpos,(float)ypos);
}

void mouse_button_callback(GLFWwindow* win, int button, int action, int mods)
{
	std::cout << "button: " << button << " " << action << " " << mods << std::endl;
}

void scroll_callback(GLFWwindow* win, double xoffset, double yoffset)
{
	std::cout << "scroll: " << xoffset << " " << yoffset << std::endl;
}

void joystick_callback(int jid, int event)
{
	std::cout << "joystick: " << jid << " " << event << std::endl;
}


///////////////////////////////////////////////////////////////////////// SETUP

void setUpDraw() {

	YellowTriangle = MatrixFactory::createTranslationMat4(vec3(-0.5f, 0.25f, 0.0f));
	YellowTriangle = YellowTriangle * MatrixFactory::createScaleMat4(vec3(0.5f, 0.5f, 1.0f));
	YellowTriangle = YellowTriangle * MatrixFactory::createRotationMat4(90.0f, vec3(0.0f, 0.0f, 1.0f));

	GreenTriangle = MatrixFactory::createTranslationMat4(vec3(-0.75f, 0.5f, 0.0f));
	GreenTriangle = GreenTriangle * MatrixFactory::createScaleMat4(vec3(0.5f, 0.5f, 1.0f));

	CyanTriangle = MatrixFactory::createTranslationMat4(vec3(-0.625f, 0.376f, 0.0f));
	CyanTriangle = CyanTriangle * MatrixFactory::createRotationMat4(45.0f, vec3(0.0f, 0.0f, 1.0f));
	CyanTriangle = CyanTriangle * MatrixFactory::createScaleMat4(vec3(0.71f, 0.71f, 1.0f));

	RedTriangle = MatrixFactory::createTranslationMat4(vec3(-0.625f, 0.376f, 0.0f));

	MagentaTriangle = MatrixFactory::createTranslationMat4(vec3(-0.125f, 0.626f, 0.0f));
	MagentaTriangle = MagentaTriangle * MatrixFactory::createRotationMat4(180.0f, vec3(0.0f, 0.0f, 1.0f));

	OrangeSquare = MatrixFactory::createTranslationMat4(vec3(-0.5f, 0.25f, 0.0f));
	OrangeSquare = OrangeSquare * MatrixFactory::createScaleMat4(vec3(0.25f, 0.25f, 1.0f));


	BlueParallelogram = MatrixFactory::createTranslationMat4(vec3(-0.50f, 0.25f, 0.0f));
	BlueParallelogram = BlueParallelogram * MatrixFactory::createScaleMat4(vec3(0.25f, 0.32f, 1.0f));
	BlueParallelogram = BlueParallelogram * MatrixFactory::createRotationMat4(-90.0f, vec3(0.0f, 0.0f, 1.0f));


	vec4 vblue = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	vblue.getData(blue);
	vec4 vred = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vred.getData(red);
	vec4 vgreen = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	vgreen.getData(green);
	vec4 vyellow = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	vyellow.getData(yellow);
	vec4 vorange = vec4(0.8f, 0.4f, 0.0f, 1.0f);
	vorange.getData(orange);
	vec4 vcyan = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	vcyan.getData(cyan);
	vec4 vmagenta = vec4(1.0f, 0.0f, 1.0f, 1.0f);
	vmagenta.getData(magenta);

	vblue = vec4(0.5f, 0.5f, 1.0f, 1.0f);
	vblue.getData(nblue);
	vred = vec4(1.0f, 0.5f, 0.5f, 1.0f);
	vred.getData(nred);
	vgreen = vec4(0.6f, 1.0f, 0.6f, 1.0f);
	vgreen.getData(ngreen);
	vyellow = vec4(1.0f, 1.0f, 0.5f, 1.0f);
	vyellow.getData(nyellow);
	vorange = vec4(0.8f, 0.4f, 0.5f, 1.0f);
	vorange.getData(norange);
	vcyan = vec4(0.5f, 1.0f, 1.0f, 1.0f);
	vcyan.getData(ncyan);
	vmagenta = vec4(1.0f, 0.5f, 1.0f, 1.0f);
	vmagenta.getData(nmagenta);
}

void setupCamera()
{
	camera = Camera();
}

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
	glfwSetKeyCallback(win, key_callback);
	glfwSetWindowCloseCallback(win, window_close_callback);
	glfwSetWindowSizeCallback(win, window_size_callback);
	glfwSetCursorPosCallback(win, mouse_callback);
	glfwSetMouseButtonCallback(win, mouse_button_callback);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	setupErrorCallback();
	shader.createShaderProgram("shaders/vertex.shader", "shaders/fragment.shader");
	createBufferObjects();
	setupCamera();
	setUpDraw();
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
		640, 480, "Hello Modern 3D World", is_fullscreen, is_vsync);
	run(win);
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////////////
