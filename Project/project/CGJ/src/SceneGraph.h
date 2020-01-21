#pragma once
#include <chrono>
#include <vector>
#include "SceneNode.h"
#include "Mesh.h"

namespace engine {
	class SceneGraph {
	protected:
		SceneNode* root;
		Camera* camera;
		std::chrono::system_clock::time_point start_time, current_time;
		float animationTime = 0.0f;
		enum AnimationState {START, END, STARTTOEND, ENDTOSTART};
		AnimationState state = START;
	public:
		void setRoot(SceneNode* node);
		SceneNode* getRoot();
		void setCamera(Camera* cam);
		void destroy();
		void draw();
		void animate();
	};
}