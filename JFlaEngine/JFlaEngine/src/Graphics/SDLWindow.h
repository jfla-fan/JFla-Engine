#pragma once

#include "../Core.h"
#include <string>
#include <cmath>
#include "GraphicsContext.h"


namespace Engine
{
	class Application;
}

namespace Engine::Graphics
{

	struct WindowCreateOptions
	{
		std::string title;
		int32 width, height;

		WindowCreateOptions(const std::string InTitle, int32 InWidth, int32 InHeight)
			: title(InTitle), width(InWidth), height(InHeight)
		{
		}
	};

	class SDLWindow
	{
	public:

		using WindowHandle = SDL_Window;

	public:

		SDLWindow(const WindowCreateOptions& InOptions);
		~SDLWindow();

		std::string				GetTitle() const { return options.title; }
		int32					GetWidth() const { return options.width; }
		int32					GetHeight() const { return options.height; }
		bool					IsOpened() const { return bIsOpened; }
		WindowCreateOptions		GetOptions() const { return options; }
		WindowHandle*			GetNativeHandle() const { return window.get(); }

		void OnCreate(const WindowCreateOptions& InOptions);
		void OnUpdate();
		void OnRender();
		void OnDestroy();
	
		void Close();

	private:
		uint32 vao, rect_vao, rect_vbo, rect_ibo, vbo, color, vio, vShader, fShader;
		uint32 program;
		
		Scope<OpenGLVertexBuffer>	RECT_VBO;
		Scope<OpenGLVertexBuffer>	RECT_COLOR;
		Scope<OpenGLIndexBuffer>			RECT_IBO;

		using DestroyWindowCallback = void(*)(WindowHandle*);

		bool bIsOpened;

		WindowCreateOptions options;

		Scope<WindowHandle, DestroyWindowCallback> window;

		Scope<GraphicsContext> context;
	};

}
