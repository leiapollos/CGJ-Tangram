#include "MatrixFactory.h"

namespace engine {
	const mat2 MatrixFactory::createZeroMat2()
	{
		return mat2(0.0f);
	}
	const mat2 MatrixFactory::createIdentityMat2()
	{
		return mat2(1.0f, 0.0f,
			0.0f, 1.0f);
	}
	const mat2 MatrixFactory::createScaleMat2(const vec2& v)
	{
		return mat2(v.x, 0.0f,
			0.0f, v.y);
	}
	const mat2 MatrixFactory::createRotationMat2(const float angle)
	{
		return mat2(cos(angle * PI / 180.0f),
			-sin(angle * PI / 180.0f),
			sin(angle * PI / 180.0f),
			cos(angle * PI / 180.0f));
	}


	const mat3 MatrixFactory::createZeroMat3()
	{
		return mat3(0.0f);
	}
	const mat3 MatrixFactory::createIdentityMat3()
	{
		return mat3(1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f);
	}
	const mat3 MatrixFactory::createDualMat3(const vec3& v)
	{
		return mat3(0.0f, -v.z, v.y,
			v.z, 0.0f, -v.x,
			-v.y, v.x, 0.0f);
	}
	const mat3 MatrixFactory::createScaleMat3(const vec3& v)
	{
		return mat3(v.x, 0.0f, 0.0f,
			0.0f, v.y, 0.0f,
			0.0f, 0.0f, v.z);
	}
	const mat3 MatrixFactory::createRotationMat3(const float angle, const vec3& axis)
	{
		mat3 A = createDualMat3(axis);
		mat3 I = createIdentityMat3();
		mat3 rotation = I + (sin(angle * PI / 180.0f)) * A + (1 - cos(angle * PI / 180.0f)) * (A * A);
		return rotation;
	}
	const mat3 MatrixFactory::createMat3FromMat4(const mat4& m)
	{
		return mat3(m.data[0][0], m.data[0][1], m.data[0][2],
			m.data[1][0], m.data[1][1], m.data[1][2],
			m.data[2][0], m.data[2][1], m.data[2][2]);
	}


	const mat4 MatrixFactory::createZeroMat4()
	{
		return mat4(0.0f);
	}
	const mat4 MatrixFactory::createIdentityMat4()
	{
		return mat4(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	const mat4 MatrixFactory::createMat4FromMat3(const mat3& m)
	{
		return mat4(m.data[0][0], m.data[0][1], m.data[0][2], 0.0f,
			m.data[1][0], m.data[1][1], m.data[1][2], 0.0f,
			m.data[2][0], m.data[2][1], m.data[2][2], 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	const mat4 MatrixFactory::createMat4FromGLMatrix(const GLMatrix& m)
	{
		return mat4(); //TODO
	}

	const mat4 MatrixFactory::createScaleMat4(const vec3& v)
	{
		return mat4(v.x, 0.0f, 0.0f, 0.0f,
			0.0f, v.y, 0.0f, 0.0f,
			0.0f, 0.0f, v.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	const mat4 MatrixFactory::createRotationMat4(const float angle, const vec3& axis)
	{
		return createMat4FromMat3(createRotationMat3(angle, axis));
	}
	const mat4 MatrixFactory::createTranslationMat4(const vec3& v)
	{
		return mat4(1.0f, 0.0f, 0.0f, v.x,
			0.0f, 1.0f, 0.0f, v.y,
			0.0f, 0.0f, 1.0f, v.z,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	void MatrixFactory::createColumnMajorMatrix(const mat4& m, float* mat)
	{
		mat4 t = transpose(m);

		int k = 0;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) {
				mat[k++] = t.data[i][j];
			}
	}

	const mat4 MatrixFactory::createMat4FromQtrn(const qtrn& q)
	{
		mat4 matrix = mat4();
		qtrn qn = qNormalize(q);

		float xx = qn.x * qn.x;
		float xy = qn.x * qn.y;
		float xz = qn.x * qn.z;
		float xt = qn.x * qn.t;
		float yy = qn.y * qn.y;
		float yz = qn.y * qn.z;
		float yt = qn.y * qn.t;
		float zz = qn.z * qn.z;
		float zt = qn.z * qn.t;

		matrix.data[0][0] = 1.0f - 2.0f * (yy + zz);
		matrix.data[0][1] = 2.0f * (xy + zt);
		matrix.data[0][2] = 2.0f * (xz - yt);
		matrix.data[0][3] = 0.0f;

		matrix.data[1][0] = 2.0f * (xy - zt);
		matrix.data[1][1] = 1.0f - 2.0f * (xx + zz);
		matrix.data[1][2] = 2.0f * (yz + xt);
		matrix.data[1][3] = 0.0f;

		matrix.data[2][0] = 2.0f * (xz + yt);
		matrix.data[2][1] = 2.0f * (yz - xt);
		matrix.data[2][2] = 1.0f - 2.0f * (xx + yy);
		matrix.data[2][3] = 0.0f;

		matrix.data[3][0] = 0.0f;
		matrix.data[3][1] = 0.0f;
		matrix.data[3][2] = 0.0f;
		matrix.data[3][3] = 1.0f;

		//matrix = transpose(matrix);

		matrix.clean();
		return matrix;
	}

	const mat4 MatrixFactory::createViewMatrix(const vec3& eye, const vec3& center, const vec3& up)
	{
		vec3 view = center - eye;
		vec3 v = normalize(view);
		vec3 side = cross(v, up);
		vec3 s = normalize(side);
		vec3 u = cross(s, v);
		mat4 R = mat4(s.x, s.y, s.z, 0.0f,
			u.x, u.y, u.z, 0.0f,
			-v.x, -v.y, -v.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
		mat4 T = mat4(1.0f, 0.0f, 0.0f, -eye.x,
			0.0f, 1.0f, 0.0f, -eye.y,
			0.0f, 0.0f, 1.0f, -eye.z,
			0.0f, 0.0f, 0.0f, 1.0f);
		mat4 M = R * T;
		return M;
	}

	const mat4 MatrixFactory::createOrthographicProjectionMatrix(const float left, const float right, const float bottom, const float top, const float near, const float far)
	{
		return mat4((float)(2 / (right - left)), 0, 0, (float)((left + right) / (left - right)),
			0, (float)(2 / (top - bottom)), 0, (float)((bottom + top) / (bottom - top)),
			0, 0, (float)(2 / (near - far)), (float)((near + far) / (near - far)),
			0, 0, 0, 1);
	}

	const mat4 MatrixFactory::createPerspectiveProjectionMatrix(const float fovy, const float aspect, const float nearZ, const float farZ)
	{
		float teta = fovy / 2.0f;
		float d = 1.0f / tan(teta * PI / 180.0f);

		return mat4(d / aspect, 0, 0, 0,
			0, d, 0, 0,
			0, 0, (nearZ + farZ) / (nearZ - farZ), 2 * farZ * nearZ / (nearZ - farZ),
			0, 0, -1.0f, 0);
	}
}