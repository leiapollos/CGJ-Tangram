#include "Camera.h"

engine::Camera::Camera()
{
	//DEFAULT INITIALIZATION

	eye = vec3(5.0f, 5.0f, 5.0f);
	up = vec3(0.0f, 1.0f, 0.0f);
	direction = vec3(-1.0f, -1.0f, -1.0f);
	direction = normalize(direction);
	speed = CAMERA_SPEED;

	createAndSetPerspectiveMatrix(30.0f, 640.0f / 480.0f, 1, 10);
	createAndSetOrthographicMatrix(-2, 2, -2, 2, 1, 10);
	createAndSetViewMatrix(eye, eye + direction, up);
	type = PERSPECTIVE;
	projectionMatrix = perspectiveMatrix;

	getYawPitchFromDirectionVector(direction);

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

	float sensitivity = CAMERA_SENSITIVITY;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.9f)
		pitch = 89.9f;
	if (pitch < -89.9f)
		pitch = -89.9f;

	direction.x = cos(yaw * PI / 180.0f) * cos(pitch * PI / 180.0f);
	direction.y = sin(pitch * PI / 180.0f);
	direction.z = sin(yaw * PI / 180.0f) * cos(pitch * PI / 180.0f);

	direction = normalize(direction);
}

void engine::Camera::getYawPitchFromDirectionVector(const vec3& direction)
{
	yaw = atan2(direction.x, direction.z);
	pitch = atan2(direction.y, direction.length());
	yaw = yaw * 180.0f / PI;
	pitch = pitch * 180.0f / PI;
}
