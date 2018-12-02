#ifndef __BACKGROUND_NODE_H__
#define __BACKGROUND_NODE_H__

#include "cocos2d.h"

namespace TakeTen {
	class BackgroundNode : public cocos2d::Node {
	public:
		CREATE_FUNC(BackgroundNode);
		//static BackgroundNode* create();
		~BackgroundNode();

		virtual bool init();

	private:

		cocos2d::DrawNode* _rectangle;
		float _height;
		void update(float dt) override;

		BackgroundNode();

		static float _yPos;
	};
}

#endif // !__BACKGROUND_NODE_H__
