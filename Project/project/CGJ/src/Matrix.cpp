#include "Matrix.h"

namespace engine {
	mat2::mat2()
	{
		for (int i = 0; i < 2; i++) {
			for(int j = 0; j < 2; j++)
				data[i][j] = 0.0f;
		}
	}
	mat2::mat2(const float k)
	{
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++)
				data[i][j] = k;
		}
	}
	mat2::mat2(const float m0, const float m1, const float m2, const float m3)
	{
		data[0][0] = m0; data[0][1] = m1;
		data[1][0] = m2; data[1][1] = m3;
	}
	mat2::mat2(const mat2& m)
	{
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++)
				data[i][j] = m.data[i][j];
		}
	}
	void mat2::clean()
	{
		//TODO
	}
	float mat2::determinant() const
	{
		return data[0][0] * data[1][1] - data[0][1] * data[1][0];
	}
	mat2& mat2::operator=(const mat2& m)
	{
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++)
				data[i][j] = m.data[i][j];
		}
		return *this;
	}
	mat2& mat2::operator+=(const mat2& v)
	{
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++)
				data[i][j] += v.data[i][j];
		}
		return *this;
	}
	mat2& mat2::operator-=(const mat2& v)
	{
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++)
				data[i][j] = v.data[i][j];
		}
		return *this;
	}
	mat2& mat2::operator*=(const float k)
	{
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++)
				data[i][j] *= k;
		}
		return *this;
	}
	mat2 transpose(const mat2& m)
	{
		return mat2(m.data[0][0], m.data[0][1], m.data[1][0], m.data[1][1]);
	}
	mat2 inverse(const mat2& m)
	{
		float det = m.determinant();
		if (det == 0) {
			return mat2(0); //Matrix cant have det == 0 to be inverseble. Make a better Error return TODO
		}

		return (1 / det) * mat2(m.data[1][1], -m.data[0][1], -m.data[1][0], m.data[0][0]);
	}
	const mat2 operator+(const mat2& m0, const mat2& m1)
	{
		mat2 res = mat2();
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++)
				res.data[i][j] = m0.data[i][j] + m1.data[i][j];
		}
		return res;
	}
	const mat2 operator-(const mat2& m0, const mat2& m1)
	{
		mat2 res = mat2();
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++)
				res.data[i][j] = m0.data[i][j] - m1.data[i][j];
		}
		return res;
	}
	const mat2 operator*(const float k, const mat2& m)
	{
		mat2 res = mat2();
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++)
				res.data[i][j] = m.data[i][j] * k;
		}
		return res;
	}
	const mat2 operator*(const mat2& m, const float k)
	{
		mat2 res = mat2();
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++)
				res.data[i][j] = m.data[i][j] * k;
		}
		return res;
	}
	const vec2 operator*(const mat2& m, const vec2& v)
	{
		vec2 res = vec2();
		res.x = m.data[0][0] * v.x + m.data[0][1] * v.y;
		res.y = m.data[1][0] * v.x + m.data[1][1] * v.y;
		return res;
	}
	const mat2 operator*(const mat2& m0, const mat2& m1)
	{
		mat2 res = mat2(0.0f);
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 2; k++)
					res.data[i][j] += m0.data[i][k] * m1.data[k][j];
		return res;
	}
	const bool operator==(const mat2& m0, const mat2& m1)
	{
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++)
				if (m0.data[i][j] != m1.data[i][j])//May need to set interval (m0-m1<0.0001) instead of ==
					return false;
		}
		return true;
	}
	const bool operator!=(const mat2& m0, const mat2& m1)
	{
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++)
				if (m0.data[i][j] != m1.data[i][j])//May need to set interval (m0-m1<0.0001) instead of ==
					return true;
		}
		return false;
	}
	std::ostream& operator<<(std::ostream& os, const mat2& m)
	{
		os << "[ " << m.data[0][0] << " , " << m.data[0][1]  << std::endl 
		   << "  " << m.data[1][0] << " , " << m.data[1][1] << " ]";
		return os;
	}

	////////////////////////////////////////////////////////////////////////////////////////////

	mat3::mat3()
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				data[i][j] = 0.0f;
		}
	}
	mat3::mat3(const float k)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				data[i][j] = k;
		}
	}
	mat3::mat3(const float m0, const float m1, const float m2, const float m3, const float m4, const float m5, const float m6, const float m7, const float m8)
	{
		data[0][0] = m0; data[0][1] = m1; data[0][2] = m2;
		data[1][0] = m3; data[1][2] = m5; data[1][1] = m4;
		data[2][0] = m6; data[2][1] = m7; data[2][2] = m8;
	}
	mat3::mat3(const mat3& m)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				data[i][j] = m.data[i][j];
		}
	}
	void mat3::clean()
	{
		//TODO
	}
	float mat3::determinant() const
	{
		float det = data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1])
				  - data[0][1] * (data[1][0] * data[2][2] - data[1][2] * data[2][0])
				  + data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);
		return det;
	}
	mat3& mat3::operator=(const mat3& m)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				data[i][j] = m.data[i][j];
		}
		return *this;
	}
	mat3& mat3::operator+=(const mat3& v)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				data[i][j] += v.data[i][j];
		}
		return *this;
	}
	mat3& mat3::operator-=(const mat3& v)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				data[i][j] -= v.data[i][j];
		}
		return *this;
	}
	mat3& mat3::operator*=(const float k)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				data[i][j] *= k;
		}
		return *this;
	}
	mat3 transpose(const mat3& m)
	{
		return mat3(m.data[0][0], m.data[1][0], m.data[2][0], m.data[0][1], m.data[1][1], m.data[2][1], m.data[0][2], m.data[1][2], m.data[2][2]);
	}
	mat3 inverse(const mat3& m)
	{
		float det = m.determinant();
		if (det == 0) {
			return mat3(0); //Matrix cant have det == 0 to be inverseble. Make a better Error return TODO
		}

		mat3 mTranspose = transpose(m);

		mat3 adj = mat3();
		int sign = 1;

		for (int p = 0; p < 3; p++) {
			for (int q = 0; q < 3; q++) {
				mat2 m2 = mat2();
				int k = 0, u = 0;
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						if (i != p && j != q) {
							m2.data[k][u++] = mTranspose.data[i][j];
							if (u == 2) {
								u = 0;
								k++;
							}
						}
					}
				}
				adj.data[p][q] = sign * m2.determinant();
				sign = -sign;
			}
		}

		return adj*=(1/det);
	}
	const mat3 operator+(const mat3& m0, const mat3& m1)
	{
		mat3 res = mat3();
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				res.data[i][j] = m0.data[i][j] + m1.data[i][j];
		return res;
	}
	const mat3 operator-(const mat3& m0, const mat3& m1)
	{
		mat3 res = mat3();
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				res.data[i][j] = m0.data[i][j] - m1.data[i][j];
		return res;
	}
	const mat3 operator*(const float k, const mat3& m)
	{
		mat3 res = mat3();
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				res.data[i][j] = m.data[i][j] * k;
		return res;
	}
	const mat3 operator*(const mat3& m, const float k)
	{
		mat3 res = mat3();
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				res.data[i][j] = m.data[i][j] * k;
		return res;
	}
	const vec3 operator*(const mat3& m, const vec3& v)
	{
		vec3 res = vec3();
		res.x = m.data[0][0] * v.x + m.data[0][1] * v.y + m.data[0][2] * v.z;
		res.y = m.data[1][0] * v.x + m.data[1][1] * v.y + m.data[1][2] * v.z;
		res.z = m.data[2][0] * v.x + m.data[2][1] * v.y + m.data[2][2] * v.z;
		return res;
	}
	const mat3 operator*(const mat3& m0, const mat3& m1)
	{
		mat3 res = mat3();
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
					res.data[i][j] += m0.data[i][k] * m1.data[k][j];
		return res;
	}
	const bool operator==(const mat3& m0, const mat3& m1)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				if (m0.data[i][j] - m1.data[i][j]>0.0001f) //May need to set interval (m0-m1<0.0001) instead of ==
					return false;
		}
		return true;
	}
	const bool operator!=(const mat3& m0, const mat3& m1)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				if (m0.data[i][j] != m1.data[i][j]) //May need to set interval (m0-m1<0.0001) instead of ==
					return true;
		}
		return false;
	}
	std::ostream& operator<<(std::ostream& os, const mat3& m)
	{
		os << "[ " << m.data[0][0] << " , " << m.data[0][1] << " , " << m.data[0][2] << std::endl <<
			  "  " << m.data[1][0] << " , " << m.data[1][1] << " , " << m.data[1][2] << std::endl <<
			  "  " << m.data[2][0] << " , " << m.data[2][1] << " , " << m.data[2][2] << " ]";
		return os;
	}

	////////////////////////////////////////////////////////////////////////////////////////////

	mat4::mat4()
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				data[i][j] = 0.0f;
		}
	}
	mat4::mat4(const float k)
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				data[i][j] = k;
		}
	}
	mat4::mat4(const float m0, const float m1, const float m2, const float m3, const float m4, const float m5, const float m6, const float m7, const float m8, const float m9, const float m10, const float m11, const float m12, const float m13, const float m14, const float m15)
	{
		data[0][0] = m0;  data[0][1] = m1;  data[0][2] = m2;  data[0][3] = m3;
		data[1][0] = m4;  data[1][1] = m5;  data[1][2] = m6;  data[1][3] = m7;
		data[2][0] = m8;  data[2][1] = m9;  data[2][2] = m10; data[2][3] = m11;
		data[3][0] = m12; data[3][1] = m13; data[3][2] = m14; data[3][3] = m15;
	}
	mat4::mat4(const mat4& m)
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				data[i][j] = m.data[i][j];
		}
	}
	float* mat4::getData()
	{
		mat4 t = transpose(*this);

		int k = 0;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) {
				dataGL[k++] = t.data[i][j];
			}

		return dataGL;
	}
	void mat4::clean()
	{
		//TODO
	}
	/*float mat4::determinant() const
	{
		return 0.0f;//TODO
	}*/
	mat4& mat4::operator=(const mat4& m)
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				data[i][j] = m.data[i][j];
		}
		return *this;
	}
	mat4 transpose(const mat4& m)
	{
		mat4 res = mat4();
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) {
				res.data[j][i] = m.data[i][j];
			}
		return res;
	}
	/*mat4 inverse(const mat4& m)
	{
		return mat4();//TODO
	}*/
	const mat4 operator+(const mat4& m0, const mat4& m1)
	{
		mat4 res = mat4();
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				res.data[i][j] = m0.data[i][j] + m1.data[i][j];
		return res;
	}
	const mat4 operator-(const mat4& m0, const mat4& m1)
	{
		mat4 res = mat4();
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				res.data[i][j] = m0.data[i][j] - m1.data[i][j];
		return res;
	}
	const mat4 operator*(const float k, const mat4& m)
	{
		mat4 res = mat4();
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				res.data[i][j] = m.data[i][j] * k;
		return res;
	}
	const mat4 operator*(const mat4& m, const float k)
	{
		mat4 res = mat4();
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				res.data[i][j] = m.data[i][j] * k;
		return res;
	}
	const vec4 operator*(const mat4& m, const vec4& v)
	{
		vec4 res = vec4();
		res.x = m.data[0][0] * v.x + m.data[0][1] * v.y + m.data[0][2] * v.z + m.data[0][3] * v.w;
		res.y = m.data[1][0] * v.x + m.data[1][1] * v.y + m.data[1][2] * v.z + m.data[1][3] * v.w;
		res.z = m.data[2][0] * v.x + m.data[2][1] * v.y + m.data[2][2] * v.z + m.data[2][3] * v.w;
		res.w = m.data[3][0] * v.x + m.data[3][1] * v.y + m.data[3][2] * v.z + m.data[3][3] * v.w;
		return res;
	}
	const mat4 operator*(const mat4& m0, const mat4& m1)
	{
		mat4 res = mat4();
		int i = 0;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				for (int k = 0; k < 4; k++)
					res.data[i][j] += m0.data[i][k] * m1.data[k][j];
		return res;
	}
	const bool operator==(const mat4& m0, const mat4& m1)
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				if (m0.data[i][j] - m1.data[i][j] > EPSILON) //May need to set interval (m0-m1<0.0001) instead of ==
					return false;
		}
		return true;
	}
	const bool operator!=(const mat4& m0, const mat4& m1)
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				if (m0.data[i][j] - m1.data[i][j] > EPSILON) //May need to set interval (m0-m1<0.0001) instead of ==
					return true;
		}
		return false;
	}
	std::ostream& operator<<(std::ostream& os, const mat4& m)
	{
		os << "[ " << m.data[0][0]  << " , " << m.data[0][1]  << " , " << m.data[0][2]   << " , " << m.data[0][3] << std::endl <<
			  "  " << m.data[1][0]  << " , " << m.data[1][1]  << " , " << m.data[1][2]   << " , " << m.data[1][3] << std::endl <<
			  "  " << m.data[2][0]  << " , " << m.data[2][1]  << " , " << m.data[2][2]   << " , " << m.data[2][3] << std::endl <<
			  "  " << m.data[3][0]  << " , " << m.data[3][1]  << " , " << m.data[3][2]   << " , " << m.data[3][3] << " ]";
		return os;
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
		return mat2(cos( angle * PI / 180.0f),
					-sin(angle * PI / 180.0f),
					sin( angle * PI / 180.0f),
					cos( angle * PI / 180.0f));
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
					  v.z,  0.0f, -v.x,
					  -v.y, v.x,  0.0f);
	}
	const mat3 MatrixFactory::createScaleMat3(const vec3& v)
	{
		return mat3(v.x,  0.0f, 0.0f,
					0.0f, v.y,  0.0f,
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
					0.0f,		  0.0f,		    0.0f,		  1.0f);
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

	const float* MatrixFactory::createColumnMajorMatrix(const mat4& m)
	{
		mat4 t = transpose(m);
		
		int k = 0;
		for(int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) {
				t.dataGL[k++] = t.data[i][j];
			}

		return t.dataGL;//TODO warning C4172: returning address of local variable or temporary: res
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	
	MatrixStack::MatrixStack()
	{
		//TODO
	}
	void MatrixStack::loadMatrix(const mat4& m)
	{
		//TODO
	}
	void MatrixStack::multiplyMatrix(const mat4& m)
	{
		//TODO
	}
	void MatrixStack::pushMatrix()
	{
		if(current != NULL)
			stack.push(current);
		current = NULL;
	}
	void MatrixStack::popMatrix()
	{
		current = stack.top();
		stack.pop(); //TODO confirmar se esta bem
	}
}
