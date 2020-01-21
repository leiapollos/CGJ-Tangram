#pragma once
#include "Matrix.h"
#include "Quaternions.h"
#include "Mesh.h"
#include "Camera.h"
#include <vector>
#include "Shader.h"

namespace engine {
	class SceneNode {
	protected:
		SceneNode* parent;
		Shader* shader;
		mat4 modelMatrix;
		vec3 initialTranslation;
		vec3 finalTranslation;
		mat4 scaleMatirx;
		qtrn initialQuaternion;
		qtrn finalQuaternion;
		mat4 totalMatrix;
		Mesh* mesh;
		vec4 color;
		float height;
		bool animatedTranslation = false, animatedQuaternion = false;
		std::vector<SceneNode*> nodes;
	public:
		vec3 localTranslationVec;
		qtrn localQuaternion;
	public:
		SceneNode(SceneNode* parent_, float h);
		void setModelMatrix(mat4 matrix);
		void setTranslation(vec3 v);
		void setAnimationTranslation(vec3 initial, vec3 final);
		void animateTranslation(float k);
		void setScaleMatirx(mat4 matrix);
		void setQuaternion(qtrn q);
		void setAnimationQuaternion(qtrn initial, qtrn final);
		void animateQuaternion(float k);
		void setColor(vec4 c);
		void setMesh(Mesh* m);
		Mesh* getMesh();
		void setShader(Shader* s);
		Shader* getShader();
		void DrawAnimated(Camera* cam, float k);
		void addChild(SceneNode* node);
		void destroy();
	};
}