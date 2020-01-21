#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Matrix.h"
#include "Vector.h"
#include "Shader.h"
#include "Camera.h"
#include "Quaternions.h"
#include "MatrixFactory.h"
#include "Mesh.h"
#include "SceneNode.h"
#include "SceneGraph.h"

using namespace engine;

GLuint VaoId, VboId[3];
Shader shader;
Camera* camera;
SceneGraph scene;
Mesh mesh;
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

typedef GLfloat Matrix[16];


/////////////////////////////////////////////////////////////////////// SCENE

void drawScene()
{
	scene.draw();
}

void createScene()
{
	std::string mesh_dir = "../../assets/models/blender_2.80/";
	
	//Floor
	std::string mesh_file = "plane.obj";
	std::string mesh_fullname = mesh_dir + mesh_file;
	Mesh* squareMesh = new Mesh();
	squareMesh->createMesh(mesh_fullname);

	Shader* shader01 = new Shader();
	shader01->mesh = squareMesh;
	shader01->createShaderProgram("shaders/vertex.shader", "shaders/fragment.shader");
	
	squareMesh->createBufferObjects();

	SceneNode* ground = new SceneNode(nullptr, 0.0f); //Can set translation, rotation and color as well
	ground->setMesh(squareMesh);
	ground->setColor(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	qtrn q = q.qFromAngleAxis(0.0f, vec4(1.0f, 0.0f, 0.0f, 1.0f));
	ground->setQuaternion(q);
	ground->setScaleMatirx(MatrixFactory::createScaleMat4(vec3(0.6f)));
	ground->setShader(shader01);

	//Square
	SceneNode* square = new SceneNode(ground, 1.0f); //Can set translation, rotation and color as well
	square->setMesh(squareMesh);
	square->setColor(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	square->setAnimationTranslation(
		vec3(0.2f, 0.1f, 0.0f),
		vec3(0.0f, 0.1f, -0.4f)
	);
	qtrn qSquare = qSquare.qFromAngleAxis(45.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	square->setQuaternion(qSquare);
	square->setScaleMatirx(MatrixFactory::createScaleMat4(vec3(0.08f,0.2f,0.08f)));
	square->setShader(shader01);

	//Triangle
	mesh_file = "triangle.obj";
	mesh_fullname = mesh_dir + mesh_file;
	Mesh* triangleMesh = new Mesh();
	triangleMesh->createMesh(mesh_fullname);

	Shader* triangleShader = new Shader();
	triangleShader->mesh = triangleMesh;
	triangleShader->createShaderProgram("shaders/vertex.shader", "shaders/fragment.shader");

	triangleMesh->createBufferObjects();

	//Triangle Red
	SceneNode* triangle = new SceneNode(ground, 2.0f); //Can set translation, rotation and color as well
	triangle->setMesh(triangleMesh);
	triangle->setColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	triangle->setTranslation(vec3(-0.4f, 0.1f, 0.8f));
	triangle->setAnimationTranslation(
		vec3(0.0f, 0.1f, 0.0f),
		vec3(-0.4f, 0.1f, 0.8f)
	);
	qtrn qRed = qRed.qFromAngleAxis(90.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	qtrn qRed1 = qRed.qFromAngleAxis(135.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	triangle->setAnimationQuaternion(qRed,qRed1);
	triangle->setScaleMatirx(MatrixFactory::createScaleMat4(vec3(0.4f, 0.2f, 0.4f)));
	triangle->setShader(triangleShader);

	//Triangle Green
	SceneNode* triangle2 = new SceneNode(ground, 3.0f); //Can set translation, rotation and color as well
	triangle2->setMesh(triangleMesh);
	triangle2->setColor(vec4(0.0f, 1.0f, 0.0f, 1.0f));
	triangle2->setAnimationTranslation(
		vec3(0.0f, 0.1f, 0.0f),
		vec3(-0.5f, 0.1f, 0.1f)
	);
	qtrn qGreen = qGreen.qFromAngleAxis(0.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	qtrn qGreen1 = qGreen.qFromAngleAxis(180.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	triangle2->setAnimationQuaternion(qGreen,qGreen1);
	triangle2->setScaleMatirx(MatrixFactory::createScaleMat4(vec3(0.4f, 0.2f, 0.4f)));
	triangle2->setShader(triangleShader);

	//Triangle Orange
	SceneNode* triangle3 = new SceneNode(ground, 4.0f); //Can set translation, rotation and color as well
	triangle3->setMesh(triangleMesh);
	triangle3->setColor(vec4(1.0f, 0.5f, 0.0f, 1.0f));
	triangle3->setAnimationTranslation(
		vec3(0.4f, 0.1f, 0.4f),
		vec3(0.2f, 0.1f, -0.0f)
	);
	qtrn qOrange = qOrange.qFromAngleAxis(45.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	qtrn qOrange1 = qOrange.qFromAngleAxis(0.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	triangle3->setAnimationQuaternion(qOrange,qOrange1);
	triangle3->setScaleMatirx(MatrixFactory::createScaleMat4(vec3(0.3f,0.2f,0.3f)));
	triangle3->setShader(triangleShader);

	//Triangle Brown
	SceneNode* triangle4 = new SceneNode(ground, 5.0f); //Can set translation, rotation and color as well
	triangle4->setMesh(triangleMesh);
	triangle4->setColor(vec4(0.5f, 0.0f, 0.0f, 1.0f));
	triangle4->setAnimationTranslation(
		vec3(0.2f, 0.1f, -0.2f), 
		vec3(0.0f, 0.1f, -0.6f)
	);
	qtrn qBrown = qBrown.qFromAngleAxis(180.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	triangle4->setQuaternion(qBrown);
	triangle4->setScaleMatirx(MatrixFactory::createScaleMat4(vec3(0.2f)));
	triangle4->setShader(triangleShader);

	//Triangle Purple
	SceneNode* triangle5 = new SceneNode(ground, 6.0f); //Can set translation, rotation and color as well
	triangle5->setMesh(triangleMesh);
	triangle5->setColor(vec4(0.5f, 0.0f, 0.5f, 1.0f));
	triangle5->setAnimationTranslation(
		vec3(0.0f, 0.1f, 0.0f), 
		vec3(0.0f, 0.1f, -0.6f)
	);
	qtrn qPurple = qPurple.qFromAngleAxis(-90.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	qtrn qPurple1 = qPurple.qFromAngleAxis(0.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	triangle5->setAnimationQuaternion(qPurple,qPurple1);
	triangle5->setScaleMatirx(MatrixFactory::createScaleMat4(vec3(0.2f)));
	triangle5->setShader(triangleShader);

	//Parallelogram
	mesh_file = "parallelogram.obj";
	mesh_fullname = mesh_dir + mesh_file;
	Mesh* parallelogramMesh = new Mesh();
	parallelogramMesh->createMesh(mesh_fullname);

	Shader* parallelogramShader = new Shader();
	parallelogramShader->mesh = parallelogramMesh;
	parallelogramShader->createShaderProgram("shaders/vertex.shader", "shaders/fragment.shader");

	parallelogramMesh->createBufferObjects();

	SceneNode* parallelogram = new SceneNode(ground, 7.0f); //Can set translation, rotation and color as well
	parallelogram->setMesh(parallelogramMesh);
	parallelogram->setColor(vec4(0.0f, 1.0f, 1.0f, 1.0f));
	parallelogram->setAnimationTranslation(
		vec3(-0.12f, 0.1f, 0.31f), 
		vec3(-0.67f, 0.1f, 0.67f)
	);
	qtrn qParallelogram = qParallelogram.qFromAngleAxis(45.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	qtrn qParallelogram1 = qParallelogram.qFromAngleAxis(90.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	parallelogram->setAnimationQuaternion(qParallelogram, qParallelogram1);
	parallelogram->setScaleMatirx(MatrixFactory::createScaleMat4(vec3(0.13f,0.2f,0.13f)));
	parallelogram->setShader(parallelogramShader);

	scene.setRoot(ground);
	scene.setCamera(camera);
}

///////////////////////////////////////////////////////////////////// CALLBACKS

void window_close_callback(GLFWwindow* win)
{
	scene.destroy();
}

void window_size_callback(GLFWwindow* win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	std::cout << "key: " << key << " " << scancode << " " << action << " " << mods << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(win, GLFW_TRUE);
		window_close_callback(win);
	}
	if (key == GLFW_KEY_P && action == 1) {
		camera->switchProjectionMatrix();
	}

	//RESET CAMERA
	if (key == GLFW_KEY_T) {
		camera = new Camera();
		scene.setCamera(camera);
	}

	if (key == GLFW_KEY_G && action == 1) {
		camera->switchGimbleLock();
	}

	if (glfwGetKey(win, GLFW_KEY_F) == GLFW_PRESS) {
		scene.animate();
	}

	if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) {
		qtrn q = scene.getRoot()->localQuaternion * q.qFromAngleAxis(0.5f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
		scene.getRoot()->setQuaternion(q);
	}

	if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) {
		qtrn q = scene.getRoot()->localQuaternion * q.qFromAngleAxis(-0.5f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
		scene.getRoot()->setQuaternion(q);
	}

	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
		vec3 T = vec3(0.0f, 0.0f, -0.02f) + scene.getRoot()->localTranslationVec;
		scene.getRoot()->setTranslation(T);
	}

	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
		vec3 T = vec3(0.0f, 0.0f, 0.02f) + scene.getRoot()->localTranslationVec;
		scene.getRoot()->setTranslation(T);
	}

	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
		vec3 T = vec3(-0.02f, 0.0f, 0.0f) + scene.getRoot()->localTranslationVec;
		scene.getRoot()->setTranslation(T);
	}

	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
		vec3 T = vec3(0.02f, 0.0f, 0.0f) + scene.getRoot()->localTranslationVec;
		scene.getRoot()->setTranslation(T);
	}
}

void mouse_callback(GLFWwindow* win, double xpos, double ypos)
{
	//std::cout << "mouse: " << xpos << " " << ypos << std::endl;
	int state = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
		camera->mouseCallBack((float)xpos, (float)ypos);
	else
		camera->firstMouseMovement = true;
}

void mouse_button_callback(GLFWwindow* win, int button, int action, int mods)
{
	std::cout << "button: " << button << " " << action << " " << mods << std::endl;
}

void scroll_callback(GLFWwindow* win, double xoffset, double yoffset)
{
	//std::cout << "scroll: " << xoffset << " " << yoffset << std::endl;
	camera->srollCallBack((float)yoffset);
}

void joystick_callback(int jid, int event)
{
	std::cout << "joystick: " << jid << " " << event << std::endl;
}


///////////////////////////////////////////////////////////////////////// SETUP

void setupCamera()
{
	camera = new Camera();
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
	glfwSetScrollCallback(win, scroll_callback);
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

	setupCamera();
	
	createScene();
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
		(int)WIDTH, (int)HEIGHT, "Hello Modern 3D World", is_fullscreen, is_vsync);
	run(win);
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////////////
