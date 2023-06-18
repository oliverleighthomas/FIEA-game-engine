#include "pch.h"
//#include "GameState.h"
//
//namespace FieaGameEngine {
//	void GameState::Initialize(GameObject* newRoot) {
//		root = newRoot;
//	}
//
//	const std::string& GameState::GetName() {
//		return root->name;
//	}
//
//	void GameState::SetName(const std::string& newName) {
//		root->name = newName;
//	}
//
//	GameTime& GameState::GetGameTime() {
//		return time;
//	}
//
//	EventQueue* GameState::GetEventQueue() {
//		return &eventQueue;
//	}
//
//	void GameState::Update() {
//		assert(root);
//
//		clock.UpdateGameTime(time);
//		eventQueue.Update(time);
//		root->Update(time);
//	}
//
//	void GameState::CleanUp() {
//		root = nullptr;
//		clock = GameClock();
//		time = GameTime();
//		eventQueue.Clear();
//	}
//
//}