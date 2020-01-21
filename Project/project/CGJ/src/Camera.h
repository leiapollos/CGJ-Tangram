#pragma once
#include <iostream>
#include <GL/glew.h>
#include "Matrix.h"
#include "MatrixFactory.h"
#include "Quaternions.h"
#include "Constants.h"
#include <cmath>

namespace engine
{
	class Camera
	{
	protected:
		mat4 viewMatrix, orthographicMatrix, perspectiveMatrix, projectionMatrix;
		enum TYPE { PERSPECTIVE, ORTHOGRAPHIC };
		TYPE type;
		enum GIMBLELOCK { ON, OFF };
		GIMBLELOCK state;
		float lastX, lastY;
		mat4 T;
		mat4 R;
		qtrn q;
	public:	
		vec3 up, eye, direction;
		bool firstMouseMovement;
	protected:
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
		void switchGimbleLock();
		void srollCallBack(float offset);
		void mouseCallBack(float xpos, float ypos);
	};
}