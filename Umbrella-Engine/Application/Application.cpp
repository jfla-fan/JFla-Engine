#include "Application.h"


namespace UE
{
	Application::Application( const String& InTitle, int32 InWidth, int32 InHeight )
		: bIsRunning( false )
	{
		OnCreate( InTitle, InWidth, InHeight );
	}

	Application::~Application()
	{
		OnDestroy();
	}

	void Application::Run()
	{
		bIsRunning = true;

		while ( bIsRunning )
		{
			OnUpdate();

			OnRender();
		}
	}

	void Application::OnCreate( const String& InTitle, int32 InWidth, int32 InHeight )
	{
		GraphicsContext::OnInit();
		window = MakeRef< JSDLWindow >( SWindowCreateOptions( InTitle, InWidth, InHeight ) );
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