#include "SceneGraph.h"

namespace engine {
	void SceneGraph::setRoot(SceneNode* node)
	{
		root = node;
	}
	SceneNode* SceneGraph::getRoot()
	{
		return root;
	}
	void SceneGraph::setCamera(Camera* cam)
	{
		camera = cam;
	}
	void SceneGraph::destroy()
	{
		root->destroy();
	}
	void SceneGraph::animate() //called when key is pressed
	{
		switch (state) {
		case START:
			state = STARTTOEND;
			break;
		case END:
			state = ENDTOSTART;
			break;
		case ENDTOSTART:
			state = STARTTOEND;
			break;
		case STARTTOEND:
			state = ENDTOSTART;
			break;
		}
		start_time = std::chrono::system_clock::now();
	}
	void SceneGraph::draw()
	{
		if (state == STARTTOEND || state == ENDTOSTART) {

			current_time = std::chrono::system_clock::now();
			std::chrono::duration<float> passed = current_time - start_time;

			if (state == ENDTOSTART)
				animationTime -= (float)(passed.count());
			if (state == STARTTOEND)
				animationTime += (float)(passed.count());

			if (animationTime > ANIMATIONTIME || animationTime < 0.0f) {
				if (state == STARTTOEND)
					state = END;
				else if (state == ENDTOSTART)
					state = START;
			}

			animationTime = animationTime > ANIMATIONTIME ? ANIMATIONTIME : animationTime;
			animationTime = animationTime < 0.0f ? 0.0f : animationTime;
			start_time = current_time;
		}
		root->DrawAnimated(camera, animationTime/ANIMATIONTIME);
	}
}