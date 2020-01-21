#include "Camera.h"

engine::Camera::Camera()
{
	//DEFAULT INITIALIZATION

	eye = vec3(0.0f, 0.0f, 5.00f);
	up = vec3(0.0f, 1.0f, 0.0f);
	direction = vec3(0.0f, 0.0f, -1.0f);
	direction = normalize(direction);

	createAndSetPerspectiveMatrix(30.0f, WIDTH / HEIGHT, 1, 10);
	createAndSetOrthographicMatrix(-2, 2, -2, 2, 1, 10);
	type = PERSPECTIVE;
	state = OFF;
	projectionMatrix = perspectiveMatrix;

	T = MatrixFactory::createTranslationMat4(vec3(0.0f,0.0f,-5.0f));
	R = MatrixFactory::createIdentityMat4();
	q = qtrn();
	mat4 qR = MatrixFactory::createMat4FromQtrn(q);
	viewMatrix = T * qR;

	firstMouseMovement = true;
}

void engine::Camera::createAndSetViewMatrix(const vec3& eye, const vec3& center, const vec3& up)
{
	viewMatrix = MatrixFactory::createViewMatrix(eye,center,up);
}

void engine::Camera::createAndSetOrthographicMatrix(float left, float right, float bottom, float top, float near, float far)
{
	orthographicMatrix = MatrixFactory::createOrthographicProjectionMatrix(left, right, bottom, top, near, far);
	type = ORTHOGRAPHIC;
	projectionMatrix = orthographicMatrix;
}

void engine::Camera::createAndSetPerspectiveMatrix(float fovy, float aspect, float nearZ, float farZ)
{
	perspectiveMatrix = MatrixFactory::createPerspectiveProjectionMatrix(fovy, aspect, nearZ, farZ);
	type = PERSPECTIVE;
	projectionMatrix = perspectiveMatrix;
}

const engine::mat4& engine::Camera::getViewMatrix()
{
	return viewMatrix;
}

const engine::mat4& engine::Camera::getOrthographicMatrix()
{
	return orthographicMatrix;
}

const engine::mat4& engine::Camera::getPerspectiveMatrix()
{
	return perspectiveMatrix;
}

const engine::mat4& engine::Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

void engine::Camera::switchProjectionMatrix()
{
	if (type == PERSPECTIVE) {
		type = ORTHOGRAPHIC;
		projectionMatrix = orthographicMatrix;
	}
	else if (type == ORTHOGRAPHIC) {
		type = PERSPECTIVE;
		projectionMatrix = perspectiveMatrix;
	}
}

void engine::Camera::switchGimbleLock()
{
	if (state == ON) {
		state = OFF;
		std::cout << "GIMBLE-LOCK: OFF" << std::endl;
	}
	else if (state == OFF) {
		state = ON;
		std::cout << "GIMBLE-LOCK: ON" << std::endl;
	}
}

void engine::Camera::srollCallBack(float offset)
{
	mat4 NT = MatrixFactory::createTranslationMat4(vec3(0.0f, 0.0f, offset*CAMERA_ZOOM_SPEED));
	T = T * NT;
	if (state == ON) {
		viewMatrix = T * R;
	}
	else if (state == OFF) {
		mat4 qR = MatrixFactory::createMat4FromQtrn(q);
		viewMatrix = T * qR;
	}
}

void engine::Camera::mouseCallBack(float xpos, float ypos)
{
	if (firstMouseMovement)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouseMovement = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	xoffset *= CAMERA_SENSITIVITY;
	yoffset *= CAMERA_SENSITIVITY;

	if (state == ON) {
		yoffset *= -1.0f;
		mat4 rx = MatrixFactory::createRotationMat4(xoffset, vec3(0.0f, 1.0f, 0.0f));
		mat4 ry = MatrixFactory::createRotationMat4(yoffset, vec3(1.0f, 0.0f, 0.0f));
		R = R * rx * ry;
		viewMatrix = T * R;
	}
	else if (state == OFF) {
		xoffset *= -1.0f;
		qtrn qx = qx.qFromAngleAxis(xoffset, vec4(0.0f, 1.0f, 0.0f, 1.0f));
		qtrn qy = qy.qFromAngleAxis(yoffset, vec4(1.0f, 0.0f, 0.0f, 1.0f));
		q = q * qx * qy;		
		mat4 qR = MatrixFactory::createMat4FromQtrn(q);
		viewMatrix = T * qR;
	}
}