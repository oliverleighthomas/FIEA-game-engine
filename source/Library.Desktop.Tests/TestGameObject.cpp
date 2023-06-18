#include "pch.h"
//#include "TestGameObject.h"
//
//namespace UnitTests {
//	RTTI_DEFINITIONS(TestGameObject);
//
//	void TestGameObject::Update(const FieaGameEngine::GameTime& time) {
//		++timesUpdated;
//
//		FieaGameEngine::Datum* children = Find("Children");
//		for (unsigned int i = 0; i < children->Size(); ++i) {
//			auto child = children->GetScope(i);
//			auto gameObjectChild = child->As<GameObject>();
//
//			assert(gameObjectChild != nullptr);
//
//			gameObjectChild->Update(time);
//		}
//	}
//}