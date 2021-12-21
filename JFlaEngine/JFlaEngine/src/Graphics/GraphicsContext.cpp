#include "GraphicsContext.h"



namespace Engine::Graphics
{

	GraphicsContext::GraphicsContext(SDL_Window* InWindow)
	{
		OnCreate(InWindow);
	}

	GraphicsContext::~GraphicsContext()
	{
		OnDestroy();
	}

	Scope<GraphicsContext> GraphicsContext::Create(SDL_Window* InWindow)
	{
		return MakeScoped<GraphicsContext>(InWindow);
	}

	void GraphicsContext::OnInit()
	{
		FATAL_ASSERT(SDL_Init(SDL_INIT_EVERYTHING) == 0, SDL_GetError());

		// opengl version
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	}

	void GraphicsContext::OnCreate(SDL_Window* window)
	{
		context = SDL_GL_CreateContext(window);
		renderer = Ref<SDL_Renderer>(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
			[](SDL_Renderer* renderer) { SDL_DestroyRenderer(renderer); });

		FATAL_ASSERT(gladLoadGLLoader(SDL_GL_GetProcAddress), "Could not initialize glad");	
	}

	void GraphicsContext::OnDestroy()
	{
		SDL_GL_DeleteContext(context);
		SDL_Quit();
	}
}