#include <iostream>
#include <GL/glew.h>
#include "Matrix.h"
#include "Constants.h"

namespace engine
{
	class Camera
	{
	protected:
		mat4 viewMatrix, orthographicMatrix, perspectiveMatrix, projectionMatrix;
		enum TYPE { PERSPECTIVE, ORTHOGRAPHIC };
		TYPE type;

		float yaw, pitch;
		float lastX, lastY;
		bool firstMouseMovement;
	public:	
		vec3 up, eye, direction;
		float speed;
	public:
		Camera();
		void createAndSetViewMatrix(const vec3& eye, const vec3& center, const vec3& up);
		void createAndSetOrthographicMatrix(float left, float right, float bottom, float top, float near, float far);
		void createAndSetPerspectiveMatrix(float fovy, float aspect, float nearZ, float farZ);
		const mat4& getViewMatrix();
		const mat4& getOrthographicMatrix();
		const mat4& getPerspectiveMatrix();
		const mat4& getProjectionMatrix();
		void switchProjectionMatrix();
		void mouseCallBack(float xpos, float ypos);
	protected:
		void getYawPitchFromDirectionVector(const vec3& direction);
	};
}