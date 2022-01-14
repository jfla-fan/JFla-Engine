#pragma once

#include "../Core.h"
#include "SDL.h"
#include "Platform/GraphicsAPI/GraphicsAPI.h"
#include "../Math/Color.h"

//#include "Platform/GraphicsAPI/OpenGL/OpenGLContext.h"

namespace J::Graphics
{
	enum class EGLPrimitiveType
	{
		POINTS		= GL_POINTS,
		TRIANGLES	= GL_TRIANGLES,
		LINES		= GL_LINES,
	};

	enum class EGLDataType
	{
		BYTE		= GL_BYTE,
		UBYTE		= GL_UNSIGNED_BYTE,
		SHORT		= GL_SHORT,
		USHORT		= GL_UNSIGNED_SHORT,
		INT32		= GL_INT,
		UINT32		= GL_UNSIGNED_INT,
		FIXED		= GL_FIXED,
		HALF_FLOAT	= GL_HALF_FLOAT,
		FLOAT		= GL_FLOAT,
		DOUBLE		= GL_DOUBLE,
	};

	class GraphicsContext
	{
	public:

		GraphicsContext( SDL_Window* InWindow );
		~GraphicsContext();

		static Scope<GraphicsContext> Create( SDL_Window* InWindow );

		static inline bool SetVSync( bool synchronized ) { return SDL_GL_SetSwapInterval( int32( synchronized ) ); }

		static inline void SwapBuffers( SDL_Window* window ) { SDL_GL_SwapWindow( window ); }

		static inline void ClearColor( const Math::Color& color ) 
		{ 
			Math::LinearColor linear = Math::Color::ToLinearColor( color ); 
			OpenGLContext::ClearColor( linear.R(), linear.G(), linear.B(), linear.A() ); 
		}
		
		static inline void ClearColor( const Math::LinearColor& linear ) { OpenGLContext::ClearColor( linear.R(), linear.G(), linear.B(), linear.A() ); }

		static inline void Clear() { OpenGLContext::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); }

		static inline bool UseDoublebuffering() { return SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); }

		static inline bool DoublebufferingEnabled() { int32 result; SDL_GL_GetAttribute( SDL_GL_DOUBLEBUFFER, &result ); return result; }

		static inline void SetPointSize( float size ) { OpenGLContext::PointSize( size ); }

		static inline void BindShader( const JShader& shader ) { OpenGLContext::UseProgram( shader.GetHandle() ); }

		static inline void DrawElements( EGLPrimitiveType primitive, const OpenGLIndexBuffer& indicies )
		{
			indicies.Bind();
			OpenGLContext::DrawElements( GL_ENUM( primitive ), indicies.GetSize(), GL_ENUM( EGLDataType::UINT32 ), nullptr );
		}

		static inline void Terminate() { SDL_Quit(); }

		/*
		* Call it only once at the moment of creating application.
		 */
		static void OnInit();


	private:

		using RendererDestroyCallback = void ( * ) ( SDL_Renderer* );

		void OnCreate( SDL_Window* window );

		void OnDestroy();

		SDL_GLContext context;
		Ref<SDL_Renderer> renderer;
	};


}


