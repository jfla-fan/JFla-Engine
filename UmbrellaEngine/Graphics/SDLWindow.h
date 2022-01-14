#pragma once

#include "../Core.h"
#include <string>
#include <cmath>
#include "GraphicsContext.h"


namespace J
{
	class Application;
}

namespace J::Graphics
{

	struct SWindowCreateOptions
	{
		std::string title;
		int32 width, height;

		SWindowCreateOptions(const std::string InTitle, int32 InWidth, int32 InHeight)
			: title(InTitle), width(InWidth), height(InHeight)
		{
		}
	};

	class JSDLWindow
	{
	public:

		using WindowHandle = SDL_Window;

	public:

		JSDLWindow(const SWindowCreateOptions& InOptions);
		~JSDLWindow();

		std::string				GetTitle() const { return options.title; }
		int32					GetWidth() const { return options.width; }
		int32					GetHeight() const { return options.height; }
		bool					IsOpened() const { return bIsOpened; }
		SWindowCreateOptions		GetOptions() const { return options; }
		WindowHandle*			GetNativeHandle() const { return window.get(); }

		void OnCreate(const SWindowCreateOptions& InOptions);
		void OnUpdate();
		void OnRender();
		void OnDestroy();
	
		void Close();

	private:
		uint32 vao, rect_vao, rect_vbo, rect_ibo, vbo, color, vio, vShader, fShader;
		
		struct RectInput
		{
			Vector3 position;
			Vector3 color;
		};

		RectInput Vertices[4];

		Ref<JShader>				SHADER;
		
		Ref<OpenGLTexture>		RECT_TEXTURE1; // cat
		Ref<OpenGLTexture>		RECT_TEXTURE2; // dendy

		Ref<OpenGLVertexArray>	RECT_VAO;
		Ref<OpenGLVertexBuffer>	RECT_VBO;
		Ref<OpenGLVertexBuffer>	RECT_COLOR;
		Ref<OpenGLIndexBuffer>	RECT_IBO;
		
		uint32 TextureID;

		using DestroyWindowCallback = void(*)(WindowHandle*);

		bool bIsOpened;

		SWindowCreateOptions options;

		Scope<WindowHandle, DestroyWindowCallback> window;

		Scope<GraphicsContext> context;
	};

}
