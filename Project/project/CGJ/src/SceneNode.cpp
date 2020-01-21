#include "SceneNode.h"

namespace engine {
	SceneNode::SceneNode(SceneNode* parent_, float h)
	{
		parent = parent_;
		if(parent!=nullptr)
			parent->addChild(this);
		setColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		setScaleMatirx(MatrixFactory::createScaleMat4(vec3(1.0f)));
		setTranslation(vec3(0.0f));
		setQuaternion(qtrn());
		height = h;
	}
	void SceneNode::setTranslation(vec3 v)
	{
		localTranslationVec = v;
	}
	void SceneNode::setAnimationTranslation(vec3 initial, vec3 final)
	{
		initialTranslation = initial;
		finalTranslation = final;
		animatedTranslation = true;
	}
	void SceneNode::animateTranslation(float k)
	{
		float h = height * abs(abs(k - 0.5f) - 0.5f) * 0.5f;
		localTranslationVec.x = initialTranslation.x + ((finalTranslation.x - initialTranslation.x) * k);
		localTranslationVec.y = initialTranslation.y + ((finalTranslation.y - initialTranslation.y) * k) + h;
		localTranslationVec.z = initialTranslation.z + ((finalTranslation.z - initialTranslation.z) * k);
	}
	void SceneNode::setScaleMatirx(mat4 matrix)
	{
		scaleMatirx = matrix;
	}
	void SceneNode::setModelMatrix(mat4 matrix)
	{
		modelMatrix = matrix;
	}
	void SceneNode::setQuaternion(qtrn q)
	{
		localQuaternion = q;
	}
	void SceneNode::setAnimationQuaternion(qtrn initial, qtrn final)
	{
		initialQuaternion = initial;
		finalQuaternion = final;
		animatedQuaternion = true;
	}
	void SceneNode::animateQuaternion(float k)
	{
		localQuaternion = qSlerp(initialQuaternion, finalQuaternion, k);
	}
	void SceneNode::setColor(vec4 c)
	{
		color = c;
	}
	void SceneNode::setMesh(Mesh* m)
	{
		mesh = m;
	}
	Mesh* SceneNode::getMesh()
	{
		return mesh;
	}
	void SceneNode::setShader(Shader* s)
	{
		shader = s;
	}
	Shader* SceneNode::getShader()
	{
		return shader;
	}

	void SceneNode::DrawAnimated(Camera* cam, float k)
	{
		if (animatedQuaternion) {
			animateQuaternion(k);
		}
		if (animatedTranslation) {
			animateTranslation(k);
		}
		
		float colorF[4];
		color.getData(colorF);

		if (parent != nullptr) {
			totalMatrix = parent->totalMatrix * MatrixFactory::createTranslationMat4(localTranslationVec) * MatrixFactory::createMat4FromQtrn(localQuaternion);
			modelMatrix = totalMatrix * scaleMatirx;
		} else {
			totalMatrix = MatrixFactory::createTranslationMat4(localTranslationVec) * MatrixFactory::createMat4FromQtrn(localQuaternion);
			modelMatrix = totalMatrix * scaleMatirx;
		}
		
		mat4 viewMatrix = cam->getViewMatrix();
		mat4 projectionMatrix = cam->getProjectionMatrix();

		glUseProgram(shader->ProgramId);

		glUniform4fv(shader->Color_UId, 1, colorF);
		glUniformMatrix4fv(shader->ModelMatrix_UId, 1, GL_FALSE, modelMatrix.getData());
		glUniformMatrix4fv(shader->ViewMatrix_UId, 1, GL_FALSE, viewMatrix.getData());
		glUniformMatrix4fv(shader->ProjectionMatrix_UId, 1, GL_FALSE, projectionMatrix.getData());

		mesh->draw();

		glUseProgram(0);
		glBindVertexArray(0);

		for (SceneNode* node : nodes)
		{
			node->DrawAnimated(cam, k);
		}
	}
	void SceneNode::addChild(SceneNode* node)
	{
		nodes.push_back(node);
	}
	void SceneNode::destroy()
	{
		mesh->destroyBufferObjects();
		shader->destroyShaderProgram();
		for (SceneNode* node : nodes)
		{
			node->destroy();
		}
	}
}