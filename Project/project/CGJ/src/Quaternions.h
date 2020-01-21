#pragma once
#include "Vector.h"
#include "Matrix.h"
#include "Constants.h"
#include <math.h>

namespace engine
{
	struct qtrn;

	struct qtrn{
		float t, x, y, z;
		qtrn();
		qtrn(const float t, const float x, const float y, const float z);
		static const qtrn qFromAngleAxis(const float theta, vec4 axis);
		static const void qToAngleAxis(const qtrn& q, float& theta, vec4& axis);
		friend const void qClean(qtrn& q);
		friend const float qQuadrance(const qtrn& q);
		friend const float qNorm(const qtrn& q);
		friend const qtrn qNormalize(const qtrn& q);
		friend const qtrn qConjugate(const qtrn& q);
		friend const qtrn qInverse(const qtrn& q);
		friend const qtrn operator + (const qtrn& q0, const qtrn& q1);
		friend const qtrn operator * (const qtrn& q, const float s);
		friend const qtrn operator * (const qtrn& q0, const qtrn& q1);
		qtrn& operator = (const qtrn& q);
		friend const qtrn qLerp(const qtrn& q0, const qtrn& q1, float k);
		friend const qtrn qSlerp(const qtrn& q0, const qtrn& q1, float k);
		friend const bool qEqual(const qtrn& q0, const qtrn& q1);
		friend const void qPrint(const std::string& s, const qtrn& q);
		friend const void qPrintAngleAxis(const std::string& s, const qtrn& q);
	};
}