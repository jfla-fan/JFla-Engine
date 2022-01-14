#pragma once

#include "../Core.h"
#include "../Graphics/SDLWindow.h"
#include <string>


namespace J 
{
	using namespace Graphics;


	class Application
	{
	public:

		Application() = delete;
		Application(const std::string& InTitle, int32 InWidth, int32 InHeight);

		~Application();

		void Run();

		Ref<JSDLWindow> GetWindow() const { return window; }

		SWindowCreateOptions GetWindowOptions() const { return window->GetOptions(); }
		bool IsWindowOpened() const { return window->IsOpened(); }
	
	protected:

		virtual void OnCreate(const std::string& InTitle, int32 InWidth, int32 InHeight);

		virtual void OnUpdate();

		virtual void OnRender();

		virtual void OnDestroy();

	private:

		bool bIsRunning;

		Ref<JSDLWindow> window;


	};

}

