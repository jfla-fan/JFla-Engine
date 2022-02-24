#pragma once

#include "../Core.h"
#include <string>
#include <cmath>
#include "GraphicsContext.h"
#include "Platform/GraphicsAPI/GpuTypes.h"


/*amespace J::Graphics::GpuApi
{
	struct InputLayoutRef;
	struct TextureRef;
	struct ShaderRef;
	struct RenderStateRef;
}*/

namespace UE
{
	class Application;
}

namespace UE::Graphics
{

	struct SWindowCreateOptions
	{
		String title;
		int32 width, height;

		SWindowCreateOptions( const String& InTitle, int32 InWidth, int32 InHeight )
			: title( InTitle ), width( InWidth ), height( InHeight )
		{
		}
	};

	class JSDLWindow
	{
	public:

		using WindowHandle = SDL_Window;

	public:

		JSDLWindow( const SWindowCreateOptions& InOptions );
		~JSDLWindow();

		String					GetTitle() const { return options.title; }
		int32					GetWidth() const { return options.width; }
		int32					GetHeight() const { return options.height; }
		bool					IsOpened() const { return bIsOpened; }
		SWindowCreateOptions	GetOptions() const { return options; }
		WindowHandle*			GetNativeHandle() const { return window.get(); }

		void OnCreate( const SWindowCreateOptions& InOptions );
		void OnUpdate();
		void OnRender();
		void OnDestroy();
	
		void Close();

	private:
		uint32 vao, rect_vao, rect_vbo, rect_ibo, vbo, color, vio, vShader, fShader, text, TVAO;
		
		struct RectInput
		{
			Vector3 position;
			Vector3 color;
		};

		RectInput Vertices[4];

		Ref< CShader >				DefaultShader;

		Ref< CShader >				SHADER;
		
		Ref< OpenGLTexture >		RECT_TEXTURE1; // cat
		Ref< OpenGLTexture >		RECT_TEXTURE2; // dendy

		//Ref<OpenGLVertexArray>	RECT_VAO;
		Ref< OpenGLVertexBuffer >	RECT_VBO;
		Ref< OpenGLVertexBuffer >	RECT_COLOR;
		Ref< OpenGLIndexBuffer >	RECT_IBO;
		
		Ref< OpenGLVertexBuffer >	TVBO;

		uint32 frm;

		uint32 TextureID;

		/************************************************************************/
		/*							GPU API EXPERIMENTS                         */
		/************************************************************************/

		//GpuApi::ShaderRef dvShader, dpShader;	// default vertex shader and default pixel shader

		Ref< CShader > dvShader, dpShader;

		//GpuApi::TextureRef renderTexture;

		Ref< CTexture > renderTexture;

		//GpuApi::InputLayoutRef layout;			// InputLayout
		Ref< CInputLayout > layout;

		//GpuApi::RenderStateRef state;

		Ref< CRenderState > state;

		//////////////////////////////////////////////////////////////////////////
		
		using DestroyWindowCallback = void ( * ) ( WindowHandle* );

		bool bIsOpened;

		SWindowCreateOptions options;

		Scope< WindowHandle, DestroyWindowCallback > window;

		Scope< GraphicsContext > context;
	};

}
