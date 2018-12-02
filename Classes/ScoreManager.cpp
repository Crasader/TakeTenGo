#include "ScoreManager.h"
#include "Md5.h"

#include "AdmobHelper.h"

#include <vector>

namespace TakeTen {

	std::shared_ptr<ScoreManager> ScoreManager::_instance = nullptr;
	static std::string _addScoreUrl = "http://www.shadowbizgame.com/taketengo/";
	static std::string _secretKey = "ilovetaketen";

	std::shared_ptr<ScoreManager> ScoreManager::getInstance() {
		if (!_instance) {
			_instance = std::shared_ptr<ScoreManager>(new (std::nothrow) ScoreManager);
		}
		return _instance;
	}

	ScoreManager::ScoreManager() : _showAds(true), _showFullscreenAds(true) {
		for (auto i = 0; i != 6; ++i) {
			_scores.push_back(0);
		}
		getScore();
		getAds();
	}

	void ScoreManager::update() {
		getScore();
	}

	ScoreManager::~ScoreManager() {
		_scores.clear();
	}

	void ScoreManager::getAds() {
#if SLIDEME
		_showAds = true;
		_showFullscreenAds = true;
		AdmobHelper::showAd();
#else
		auto postData = "getads.php";
		auto url = _addScoreUrl + postData;
		auto request = new cocos2d::network::HttpRequest();
		request->setUrl(url.c_str());
		request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
		request->setResponseCallback(CC_CALLBACK_2(ScoreManager::responce, this));
		auto tag = "GETADS";
		request->setTag(tag);
		cocos2d::network::HttpClient::getInstance()->send(request);
		request->release();

		CCLOG("GET DATA %s", url.c_str());
		CCLOG("GET REQUEST TAG %s", tag);
#endif
	}

	void ScoreManager::postScore(int difficulty, int time) {

		if (time < 10) {
			return;
		}

		_scores[difficulty] = time;

		auto diffStr = std::to_string(difficulty);
		auto timeStr = std::to_string(time);
		auto hashStr = md5(diffStr + timeStr + _secretKey);
		auto postData = "addscorehard.php?difficulty=" + diffStr + "&time=" + timeStr +"&hash=" + hashStr;
		auto url = _addScoreUrl + postData;
		auto request = new cocos2d::network::HttpRequest();
		request->setTag("SET");
		request->setUrl(url.c_str());
		request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
		request->setResponseCallback(CC_CALLBACK_2(ScoreManager::responce, this));
		cocos2d::network::HttpClient::getInstance()->send(request);
		request->release();
	}

	void ScoreManager::getScore() {
		for (auto i = 0; i != 6; ++i) {
			auto diffStr = std::to_string(i);
			auto postData = "getscorehard.php?difficulty=" + diffStr;
			auto url = _addScoreUrl + postData;
			auto request = new cocos2d::network::HttpRequest();
			request->setUrl(url.c_str());
			request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
			request->setResponseCallback(CC_CALLBACK_2(ScoreManager::responce, this));
			auto tag = "GET" + diffStr;
			request->setTag(tag.c_str());
			cocos2d::network::HttpClient::getInstance()->send(request);
			request->release();
			CCLOG("GET DATA %s", url.c_str());
			CCLOG("GET REQUEST TAG %s %d", tag.c_str(), i);
		}
	}

	void ScoreManager::responce(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
		
		if (response->isSucceed()) {
			std::vector<char> *buffer = response->getResponseData();
			if (buffer) {
				auto request = response->getHttpRequest();
				std::string stringTag(request->getTag());
				if (stringTag == "GETADS") {
					if (buffer->size() == 2) {
						auto showAdsC = (*buffer)[0];
						auto showFullscreenAdsC = (*buffer)[1];
						_showAds = showAdsC == 't';
						_showFullscreenAds = showFullscreenAdsC == 't';
						if (_showAds) {
							CCLOG("SHOWING ADS");
							AdmobHelper::showAd();
						}
					}
					return;
				}
				std::string bufferString = "";
				for (auto c : *buffer) {
					bufferString += c;
				}
				auto bufferInt = atoi(bufferString.c_str());
				if (bufferInt < 1) {
					return;
				}
				if (stringTag == "GET0") {
					_scores[0] = bufferInt;
				}
				else if (stringTag == "GET1") {
					_scores[1] = bufferInt;
				}
				else if (stringTag == "GET2") {
					_scores[2] = bufferInt;
				}
				else if (stringTag == "GET3") {
					_scores[3] = bufferInt;
				}
				else if (stringTag == "GET4") {
					_scores[4] = bufferInt;
				}
				else if (stringTag == "GET5") {
					_scores[5] = bufferInt;
				}
			}
		}
	}
}
