#pragma once
#include "Matrix.h"
#include "Vector.h"
#include "Quaternions.h"

namespace engine {
	struct MatrixFactory {
		static const mat2 createZeroMat2();
		static const mat2 createIdentityMat2();
		static const mat2 createScaleMat2(const vec2& v);
		static const mat2 createRotationMat2(const float angle);

		static const mat3 createZeroMat3();
		static const mat3 createIdentityMat3();
		static const mat3 createDualMat3(const vec3& v);
		static const mat3 createScaleMat3(const vec3& v);
		static const mat3 createRotationMat3(const float angle, const vec3& axis);
		static const mat3 createMat3FromMat4(const mat4& m);

		static const mat4 createZeroMat4();
		static const mat4 createIdentityMat4();
		static const mat4 createMat4FromMat3(const mat3& m);
		static const mat4 createMat4FromGLMatrix(const GLMatrix& m);
		static const mat4 createScaleMat4(const vec3& v);
		static const mat4 createRotationMat4(const float angle, const vec3& axis);
		static const mat4 createTranslationMat4(const vec3& v);
		static void createColumnMajorMatrix(const mat4& m, float* mat);
		static const mat4 createMat4FromQtrn(const qtrn& q);
		static const mat4 createViewMatrix(const vec3& eye, const vec3& center, const vec3& up);
		static const mat4 createOrthographicProjectionMatrix(const float left, const float right, const float bottom, const float top, const float near, const float far);
		static const mat4 createPerspectiveProjectionMatrix(const float fovy, const float aspect, const float nearZ, const float farZ);
	};
}