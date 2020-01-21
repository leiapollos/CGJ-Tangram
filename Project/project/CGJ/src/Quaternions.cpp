#include "Quaternions.h"

namespace engine {
	qtrn::qtrn()
	{
		t = 1.0f;
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	qtrn::qtrn(const float t_, const float x_, const float y_, const float z_)
	{
		t = t_;
		x = x_;
		y = y_;
		z = z_;
	}
	const qtrn qtrn::qFromAngleAxis(const float theta, vec4 axis)
	{
		vec4 axisn = normalize(axis);

		qtrn q;
		float a = theta * (float)DEGREES_TO_RADIANS;
		q.t = cos(a / 2.0f);
		float s = sin(a / 2.0f);
		q.x = axisn.x * s;
		q.y = axisn.y * s;
		q.z = axisn.z * s;

		qClean(q);
		return qNormalize(q);
	}

	const void qtrn::qToAngleAxis(const qtrn& q, float& theta, vec4& axis)
	{
		qtrn qn = qNormalize(q);
		theta = 2.0f * acos(qn.t) * (float)RADIANS_TO_DEGREES;
		float s = sqrt(1.0f - qn.t * qn.t);
		if (s < THRESHOLD) {
			axis.x = 1.0f;
			axis.y = 0.0f;
			axis.z = 0.0f;
			axis.w = 1.0f;
		}
		else {
			float sinv = 1 / s;
			axis.x = qn.x * sinv;
			axis.y = qn.y * sinv;
			axis.z = qn.z * sinv;
			axis.w = 1.0f;
		}
	}

	qtrn& qtrn::operator=(const qtrn& q)
	{
		t = q.t;
		x = q.x;
		y = q.y;
		z = q.z;
		return *this;
	}

	const void qClean(qtrn& q)
	{
		if (fabs(q.t) < THRESHOLD) q.t = 0.0f;
		if (fabs(q.x) < THRESHOLD) q.x = 0.0f;
		if (fabs(q.y) < THRESHOLD) q.y = 0.0f;
		if (fabs(q.z) < THRESHOLD) q.z = 0.0f;
	}

	const float qQuadrance(const qtrn& q)
	{
		return q.t * q.t + q.x * q.x + q.y * q.y + q.z * q.z;
	}

	const float qNorm(const qtrn& q)
	{
		return sqrt(qQuadrance(q));
	}

	const qtrn qNormalize(const qtrn& q)
	{
		float s = 1 / qNorm(q);
		return q * s;
	}

	const qtrn qConjugate(const qtrn& q)
	{
		qtrn qconj = { q.t, -q.x, -q.y, -q.z };
		return qconj;
	}

	const qtrn qInverse(const qtrn& q)
	{
		return qConjugate(q) * (1.0f / qQuadrance(q));
	}

	const qtrn operator+(const qtrn& q0, const qtrn& q1)
	{
		qtrn q;
		q.t = q0.t + q1.t;
		q.x = q0.x + q1.x;
		q.y = q0.y + q1.y;
		q.z = q0.z + q1.z;
		return q;
	}

	const qtrn operator*(const qtrn& q, const float s)
	{
		qtrn sq;
		sq.t = s * q.t;
		sq.x = s * q.x;
		sq.y = s * q.y;
		sq.z = s * q.z;
		return sq;
	}

	const qtrn operator*(const qtrn& q0, const qtrn& q1)
	{
		qtrn q;
		q.t = q0.t * q1.t - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z;
		q.x = q0.t * q1.x + q0.x * q1.t + q0.y * q1.z - q0.z * q1.y;
		q.y = q0.t * q1.y + q0.y * q1.t + q0.z * q1.x - q0.x * q1.z;
		q.z = q0.t * q1.z + q0.z * q1.t + q0.x * q1.y - q0.y * q1.x;
		return q;
	}

	const qtrn qLerp(const qtrn& q0, const qtrn& q1, float k)
	{
		float cos_angle = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.t * q1.t;
		float k0 = 1.0f - k;
		float k1 = (cos_angle > 0) ? k : -k;
		qtrn qi = (q0 * k0) + (q1 * k1);
		return qNormalize(qi);
	}

	const qtrn qSlerp(const qtrn& q0, const qtrn& q1, float k)
	{
		float angle = acos(q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.t * q1.t);
		float k0 = sin((1 - k) * angle) / sin(angle);
		float k1 = sin(k * angle) / sin(angle);
		qtrn qi = (q0 * k0) + (q1 * k1);
		return qNormalize(qi);
	}

	const bool qEqual(const qtrn& q0, const qtrn& q1)
	{
		return (fabs(q0.t - q1.t) < THRESHOLD && fabs(q0.x - q1.x) < THRESHOLD &&
			fabs(q0.y - q1.y) < THRESHOLD && fabs(q0.z - q1.z) < THRESHOLD);
	}

	const void qPrint(const std::string& s, const qtrn& q)
	{
		std::cout << s << " = (" << q.t << ", " << q.x << ", " << q.y << ", " << q.z << ")" << std::endl;
	}

	const void qPrintAngleAxis(const std::string& s, const qtrn& q)
	{
		std::cout << s << " = [" << std::endl;

		float thetaf;
		vec4 axis_f;
		q.qToAngleAxis(q, thetaf, axis_f);
		std::cout << "  angle = " << thetaf << std::endl;
		std::cout << "  axis" << axis_f << std::endl;
		std::cout << "]" << std::endl;
	}
}
