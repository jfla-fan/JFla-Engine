#pragma once

#include "../Core.h"
#include "../Graphics/SDLWindow.h"
#include <string>


namespace Engine 
{
	using namespace Graphics;


	class Application
	{
	public:

		Application() = delete;
		Application(const std::string& InTitle, int32 InWidth, int32 InHeight);

		~Application();

		void Run();

		Ref<SDLWindow> GetWindow() const { return window; }

		WindowCreateOptions GetWindowOptions() const { return window->GetOptions(); }
		bool IsWindowOpened() const { return window->IsOpened(); }
	
	protected:

		void OnCreate(const std::string& InTitle, int32 InWidth, int32 InHeight);

		void OnUpdate();

		void OnRender();

		void OnDestroy();

	private:

		bool bIsRunning;

		Ref<SDLWindow> window;


	};

}

