#pragma once
#ifndef __SCORE_MANAGER_H__
#define __SCORE_MANAGER_H__

#include <vector>

#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "Definitions.h"

namespace TakeTen {
	class ScoreManager
	{
	public:

		~ScoreManager();

		static std::shared_ptr<ScoreManager> getInstance();

		void update();

		void postScore(int difficulty, int time);
		
		inline int getScore(int difficulty) const {
			return _scores[difficulty];
		}

		inline bool showAds() const {
			return _showAds;
		}

		inline bool showFullscreenAds() const {
			return _showFullscreenAds;
		}

	private:

		void getAds();
		void getScore();
		void responce(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);

		ScoreManager();
		
		bool _showAds;
		bool _showFullscreenAds;

		std::vector<int> _scores;
		static std::shared_ptr<ScoreManager> _instance;
	};
}

#endif
