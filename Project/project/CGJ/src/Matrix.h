#pragma once
#include "Vector.h"
#include "Quaternions.h"
#include <stack>
#include <math.h>
#include "Constants.h"

namespace engine
{
	typedef float GLMatrix[16];
	struct mat3;
	struct mat4;
	struct MatrixStack;

	struct mat2
	{
		float data [2][2];
		mat2();
		mat2(const float k);
		mat2(const float m0, const float m1, const float m2, const float m3);
		mat2(const mat2& m);
		void clean();
		float determinant() const;
		mat2& operator = (const mat2& m);
		friend mat2 transpose(const mat2& m);
		friend mat2 inverse(const mat2& m);
		mat2& operator += (const mat2& v);
		mat2& operator -= (const mat2& v);
		mat2& operator *= (const float k);
		friend const mat2 operator + (const mat2& m0, const mat2& m1);
		friend const mat2 operator - (const mat2& m0, const mat2& m1);
		friend const mat2 operator * (const float k, const mat2& m);
		friend const mat2 operator * (const mat2& m, const float k);
		friend const vec2 operator * (const mat2& m, const vec2& v);
		friend const mat2 operator * (const mat2& m0, const mat2& m1);
		friend const bool operator == (const mat2& m0, const mat2& m1);
		friend const bool operator != (const mat2& m0, const mat2& m1);
		friend std::ostream& operator << (std::ostream& os, const  mat2& m);
	};

	struct mat3
	{
		float data[3][3];
		mat3();
		mat3(const float k);
		mat3(const float m0, const float m1, const float m2,
			const float m3, const float m4, const float m5,
			const float m6, const float m7, const float m8);
		mat3(const mat3& m);
		void clean();
		float determinant() const;
		mat3& operator = (const mat3& m);
		friend mat3 transpose(const mat3& m);
		friend mat3 inverse(const mat3& m);
		mat3& operator += (const mat3& v);
		mat3& operator -= (const mat3& v);
		mat3& operator *= (const float k);
		friend const mat3 operator + (const mat3& m0, const mat3& m1);
		friend const mat3 operator - (const mat3& m0, const mat3& m1);
		friend const mat3 operator * (const float k, const mat3& m);
		friend const mat3 operator * (const mat3& m, const float k);
		friend const vec3 operator * (const mat3& m, const vec3& v);
		friend const mat3 operator * (const mat3& m0, const mat3& m1);
		friend const bool operator == (const mat3& m0, const mat3& m1);
		friend const bool operator != (const mat3& m0, const mat3& m1);
		friend std::ostream& operator << (std::ostream& os, const  mat3& m);
	};

	struct mat4
	{
		float data[4][4];
		float dataGL[16];
		mat4();
		mat4(const float k);
		mat4(const float m0, const float m1, const float m2, const float m3,
			const float m4, const float m5, const float m6, const float m7,
			const float m8, const float m9, const float m10, const float m11,
			const float m12, const float m13, const float m14, const float m15);
		mat4(const mat4& m);
		float* getData();
		void clean();
		//float determinant() const;
		mat4& operator = (const mat4& m);
		friend mat4 transpose(const mat4& m);
		//friend mat4 inverse(const mat4& m);
		friend const mat4 operator + (const mat4& m0, const mat4& m1);
		friend const mat4 operator - (const mat4& m0, const mat4& m1);
		friend const mat4 operator * (const float k, const mat4& m);
		friend const mat4 operator * (const mat4& m, const float k);
		friend const vec4 operator * (const mat4& m, const vec4& v);
		friend const mat4 operator * (const mat4& m0, const mat4& m1);
		friend const bool operator == (const mat4& m0, const mat4& m1);
		friend const bool operator != (const mat4& m0, const mat4& m1);
		friend std::ostream& operator << (std::ostream& os, const  mat4& m);
	};

	struct MatrixStack {
		mat4 current;
		std::stack<mat4> stack;

		MatrixStack();
		void loadMatrix(const mat4& m);
		void multiplyMatrix(const mat4& m);
		void pushMatrix();
		void popMatrix();
	};
}