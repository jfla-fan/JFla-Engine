#include "Application.h"


namespace Engine
{
	Application::Application(const std::string& InTitle, int32 InWidth, int32 InHeight)
		: bIsRunning(false)
	{
		OnCreate(InTitle, InWidth, InHeight);
	}

	Application::~Application()
	{
		OnDestroy();
	}

	void Application::Run()
	{
		bIsRunning = true;

		while (bIsRunning)
		{
			OnUpdate();

			OnRender();
		}
	}

	void Application::OnCreate(const std::string& InTitle, int32 InWidth, int32 InHeight)
	{
		GraphicsContext::OnInit();
		window = MakeRef<SDLWindow>(WindowCreateOptions(InTitle, InWidth, InHeight));
	}

	void Application::OnUpdate()
	{
		// process all the events and update states.
		window->OnUpdate();
	}

	void Application::OnRender()
	{
		window->OnRender();
	}

	void Application::OnDestroy()
	{

	}


}