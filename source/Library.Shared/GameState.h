//#pragma once
//#include "Attributed.h"
//#include "GameClock.h"
//#include "GameTime.h"
//#include "GameObject.h"
//#include "EventQueue.h"
//
//namespace FieaGameEngine {
//	class GameState final
//	{
//	public:
//		/// <summary>
//		/// Initialize the game state object.
//		/// </summary>
//		/// <param name="root">The root game object containing the game state information.</param>
//		static void Initialize(GameObject* root);
//
//		/// <summary>
//		/// Get the name of the game state.
//		/// </summary>
//		/// <returns>The name of the game state.</returns>
//		static const std::string& GetName();
//
//		/// <summary>
//		/// Change the name of the game state.
//		/// </summary>
//		/// <param name="newName">The name to change the game state's name to.</param>
//		static void SetName(const std::string& newName);
//
//		/// <summary>
//		/// Get the GameTime of the game state.
//		/// </summary>
//		/// <returns>The GameTime associated with the game state.</returns>
//		static GameTime& GetGameTime();
//
//		/// <summary>
//		/// Get the EventQueue of the game state.
//		/// </summary>
//		/// <returns>The EventQueue associated with the game state.</returns>
//		static EventQueue* GetEventQueue();
//
//		/// <summary>
//		/// Update the game. First updates the root game object, then updates the associated event queue.
//		/// </summary>
//		static void Update();
//
//		/// <summary>
//		/// Clear the event queue and restore variables to defaults.
//		/// </summary>
//		static void CleanUp();
//
//	private:
//		inline static GameObject* root;
//		inline static GameClock clock;
//		inline static GameTime time;
//		inline static EventQueue eventQueue;
//	};
//}
//
//
