#pragma once

#include "../Core.h"
#include "SDL.h"
#include "glad/glad.h"
#include "../Math/Color.h"


namespace Engine::Graphics
{

	class GraphicsContext
	{
	public:

		GraphicsContext(SDL_Window* InWindow);
		~GraphicsContext();

		static Scope<GraphicsContext> Create(SDL_Window* InWindow);

		static inline bool SetVSync(bool synchronized) { return SDL_GL_SetSwapInterval(int32(synchronized)); }

		static inline void SwapBuffers(SDL_Window* window) { SDL_GL_SwapWindow(window); }

		static inline void ClearColor(const Math::Color& color) 
		{ 
			Math::LinearColor linear = Math::Color::ToLinearColor(color); 
			glClearColor(linear.R(), linear.G(), linear.B(), linear.A()); 
		}
		
		static inline void ClearColor(const Math::LinearColor& linear) { glClearColor(linear.R(), linear.G(), linear.B(), linear.A()); }

		static inline void Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

		static inline bool UseDoublebuffering() { int32 result; SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &result); return result; }

		static inline void SetPointSize(float size) { glPointSize(size); }

		static inline void UseProgram(uint32 program) { glUseProgram(program); }

		static inline void Terminate() { SDL_Quit(); }

		/*
		* Call it only once at the moment of creating application.
		 */
		static void OnInit();

	private:

		using RendererDestroyCallback = void (*) (SDL_Renderer*);

		void OnCreate(SDL_Window* window);

		void OnDestroy();

		SDL_GLContext context;
		Ref<SDL_Renderer> renderer;
	};


}


