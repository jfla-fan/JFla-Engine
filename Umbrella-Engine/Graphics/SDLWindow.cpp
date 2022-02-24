#include <OpenImageIO/imageio.h>
#include "../Image/ImageLoader.h"
#include "../Image/ImageUtils.h"
#include "SDLWindow.h"
#include "Platform/GraphicsAPI/GpuApi.h"



namespace UE::Graphics
{
	static bool LocalCheckFramebufferCompleteness( GLuint framebuffer, GLenum Target )
	{
		GLenum status = OpenGLContext::CheckNamedFramebufferStatus( framebuffer, Target );

#define CASE_LABEL( invalid_code )					\
		case JF_CONCATENATE2( GL_, invalid_code ):	\
			std::cout << #invalid_code << '\n';		\
			return false

#define CASE_LABEL_SUCCESS( code )					\
		case JF_CONCATENATE2( GL_, code ):			\
			return true

		switch( status )
		{
			CASE_LABEL( FRAMEBUFFER_UNDEFINED );
			CASE_LABEL( FRAMEBUFFER_INCOMPLETE_ATTACHMENT );
			CASE_LABEL( FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT );
			CASE_LABEL( FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER);
			CASE_LABEL( FRAMEBUFFER_INCOMPLETE_READ_BUFFER );
			CASE_LABEL( FRAMEBUFFER_UNSUPPORTED );
			CASE_LABEL( FRAMEBUFFER_INCOMPLETE_MULTISAMPLE );
			CASE_LABEL( FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS );

		default:
			return true;
		}

#undef CASE_LABEL
#undef CASE_LABEL_SUCCESS

	}


	JSDLWindow::JSDLWindow( const SWindowCreateOptions& InOptions )
		: options( InOptions ), bIsOpened( false ), window( nullptr, [](WindowHandle* window ) {} )
	{
		OnCreate( InOptions );
	}

	JSDLWindow::~JSDLWindow()
	{
		OnDestroy();
	}

	// examples of simple shaders

	const std::string& VertexShader = R"(
		#version 450 core

		in vec3 inPosition;
		in vec3 inColor;
		in vec2 inTexCoords;		
		
		out vec3 outColor;
		out vec2 outTexCoords;

		void main()
		{
			gl_Position = vec4(inPosition, 1.0);
			outColor = inColor;
			outTexCoords = inTexCoords;
		}
	)";

	const String VShader = R"(
		#version 450 core

		in vec3 inPosition;
		in vec3 inColor;

		out vec3 outColor;

		void main()
		{
			gl_Position = vec4( inPosition, 1.0f );
			outColor = inColor;
		}

	)";

	const String FShader = R"(
		#version 450 core

		in vec3 outColor;

		void main()
		{
			gl_FragColor = vec4( outColor, 1.0f );
		}

	)";

	const std::string& FragmentShader = R"(
		#version 450 core

		in vec3 outColor;
		in vec2 outTexCoords;		
	
		uniform sampler2D sampler1;		
		uniform sampler2D sampler2;

		void main()
		{
			gl_FragColor = max(texture(sampler1, outTexCoords), texture(sampler2, outTexCoords));
			//gl_FragColor = texture(sampler1, outTexCoords);
			//gl_FragColor = texture(sampler2, outTexCoords);
		}
	)";

	//static std::map<Utils::ERawImageFormat, EGLDataType> ImageFormatToGLDataTypeTable =
	//{
	//	{  }
	//}

	void JSDLWindow::OnCreate( const SWindowCreateOptions& InOptions )
	{
		window = Scope< WindowHandle, DestroyWindowCallback >
		(
			SDL_CreateWindow
			(
				options.title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				options.width, options.height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
			),

			[]( WindowHandle* handle ) { SDL_DestroyWindow( handle ); }
		);
		
		// #todo set time when the window was created.

		context = GraphicsContext::Create( window.get() );
		bIsOpened = true;

		glViewport( 0, 0, options.width, options.height );
		

		const Math::LinearColor& yellow = Math::LinearColor::Yellow;

		// rectangle
		
		// rectangle coordinates and color

						// position							// color
		Vertices[0] = { Vector3 { 0.5f, 0.5f, 0.f},		Vector3 { 1.0f, 0.0f, 0.0f } };
		Vertices[1] = { Vector3 { -0.5f, 0.5f, 0.f},	Vector3 { 0.0f, 1.0f, 0.0f } };
		Vertices[2] = { Vector3 { -0.5f, -0.5f, 0.f},	Vector3 { 0.0f, 0.0f, 1.0f } };
		Vertices[3] = { Vector3 { 0.5f, -0.5f, 0.f},	Vector3 { yellow.R(), yellow.G(), yellow.B() } };
		
		const String ImagePath = "C:/Users/ДНС/Documents/Projects/VisualStudioProjects/GameEngine/Resources/Images/sleepy-cat.jpg";
		const String DendyImagePath = "C:/Users/ДНС/Documents/Projects/VisualStudioProjects/GameEngine/Resources/Images/dendy.png";


		const String SaveImagePath = "C:/Users/ДНС/Documents/Projects/VisualStudioProjects/GameEngine/Resources/Images/screen.jpg";

		Utils::ImagePtr image = Utils::ImageLoader::Load( ImagePath, true );

		if ( !image )
		{
			std::cout << std::format( "Could not load {} image.\n", ImagePath );
			std::exit( EXIT_FAILURE );
		}

		image->PrintImageMetaData( std::cout );

		Utils::ImagePtr dendyImage = Utils::ImageLoader::Load( DendyImagePath, true );

		if ( !dendyImage )
		{
			std::cout << std::format( "Could not load {} image.\n", DendyImagePath );
			std::exit( EXIT_FAILURE );
		}

		dendyImage->PrintImageMetaData( std::cout );
		
		RECT_TEXTURE1 = MakeRef< OpenGLTexture >( ETextureType::TEXTURE_2D, ETextureFormat::AUTO, "First rectangle texture." );
		RECT_TEXTURE2 = MakeRef< OpenGLTexture >( ETextureType::TEXTURE_2D, ETextureFormat::AUTO, "Second rectangle texture.");


		if ( !RECT_TEXTURE1->Load( *image ) )
		{
			std::cout << "Failed to load texture\n";
			std::exit( 1 );
		}

		if ( !RECT_TEXTURE2->Load( *dendyImage ) )
		{
			std::cout << "Failed to load texture\n";
			std::exit( 1 );
		}

		RECT_TEXTURE1->GenerateMipmapChain();
		RECT_TEXTURE2->GenerateMipmapChain();

		RECT_TEXTURE1->SetFilter( ETextureFilter::MIN_MAG_LINEAR_MIP_POINT );
		RECT_TEXTURE2->SetFilter( ETextureFilter::MIN_MAG_LINEAR_MIP_POINT );

		image.reset();
		dendyImage.reset();




		TVBO = OpenGLVertexBuffer::Create(
			new float[]
			{
				// position			// color						
				0.5f, 0.5f, 0.f,	1.0f, 0.0f, 0.0f,	/* red   */	
				-0.5f, 0.5f, 0.f,	0.0f, 1.0f, 0.0f,	/* green */	
				-0.5f, -0.5f, 0.f,	0.0f, 0.0f, 1.0f,	/* blue  */	
				0.5f, -0.5f, 0.f,	yellow.R(), yellow.G(), yellow.B(),
			},
			6 * 4
		);

		RECT_VBO = OpenGLVertexBuffer::Create(
			new float[]
			{
				// position			// color								// uv - texture coordinates
				0.5f, 0.5f, 0.f,	1.0f, 0.0f, 0.0f,	/* red   */			1.0f, 1.0f,
				-0.5f, 0.5f, 0.f,	0.0f, 1.0f, 0.0f,	/* green */			0.0f, 1.0f,
				-0.5f, -0.5f, 0.f,	0.0f, 0.0f, 1.0f,	/* blue  */			0.0f, 0.0f,
				0.5f, -0.5f, 0.f,	yellow.R(), yellow.G(), yellow.B(),		1.0f, 0.0f,
			},
			8 * 4
			);
		// index buffer

		RECT_IBO = OpenGLIndexBuffer::Create(
			new uint32[]
			{
				0, 1, 2,
				3, 0, 2
			},
			6,
			GpuApi::EBufferAccessBits::ALL
		);


		//RECT_VAO->AddVertexLayout(RECT_VBO,
		//		std::array
		//		{
		//			SVertexAttribute::GetAttribute<Vector3>(),
		//			SVertexAttribute::GetAttribute<Vector3>(),
		//			SVertexAttribute::GetAttribute<Vector2>()
		//		}
		//	);

		// disable color attribute
		// this should result in a black square (square without color) [to enable add true to every attribute in constructor]
		//RECT_VAO->RemoveVertexLayout(
		//		std::array
		//		{
		//			SVertexAttribute::GetAttribute<Vector3>(),
		//			SVertexAttribute::GetAttribute<Vector3>(),
		//		}
		//);

		// Dealing with shaders

		//OpenGLContext::GenFramebuffers( 1, &frm );

		//OpenGLContext::BindFramebuffer( GL_FRAMEBUFFER, frm );
		//
		//OpenGLContext::GenTextures( 1, &text );
		//
		//OpenGLContext::BindTexture( GL_TEXTURE_2D, text );

		//OpenGLContext::TextureStorage2D( text, 1, GL_RGBA8, 512, 512 );

		//OpenGLContext::TextureParameteri( text, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		//OpenGLContext::TextureParameteri( text, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		////glNamedFramebufferTexture( frm, GL_COLOR_ATTACHMENT0, text, 0 );

		//OpenGLContext::FramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, text, 0 );

		//FATAL_ASSERT( LocalCheckFramebufferCompleteness( frm, GL_FRAMEBUFFER ), "Framebuffer is incomplete." );


		//// may be needed

		///*static const GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };

		//glDrawBuffers( 1, drawBuffers );*/


		//OpenGLContext::GenVertexArrays( 1, &TVAO );

		//OpenGLContext::BindVertexArray( TVAO );

		//OpenGLContext::BindBuffer( GL_ARRAY_BUFFER, GpuApi::GetBufferNativeResourceDEBUG( TVBO->GetNativeHandle() ) );

		//OpenGLContext::EnableVertexAttribArray( 0 );
		//OpenGLContext::VertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, reinterpret_cast< CMemPtr >( 0 ) );

		//OpenGLContext::EnableVertexAttribArray( 1 );
		//OpenGLContext::VertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, reinterpret_cast< CMemPtr >( sizeof( float ) * 3 ) );



		//SHADER = MakeRef< OpenGLShader >();
		//SHADER->LoadFromString( VertexShader, FragmentShader );

		//DefaultShader = MakeRef< OpenGLShader >();
		//DefaultShader->LoadFromString( VShader, FShader );

		//GraphicsContext::BindShader( *DefaultShader );

		////GraphicsContext::BindShader( *SHADER );
		//
		////RECT_TEXTURE1->Bind( 0 );
		////RECT_TEXTURE2->Bind( 1 );

		////SHADER->SetUniform( "sampler1", 0 );
		////SHADER->SetUniform( "sampler2", 1 );

		//OpenGLContext::ClearColor( 1.f, 1.f, 1.f, 1.f );
		//OpenGLContext::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		//glBindVertexArray( TVAO );

		//glBindFramebuffer( GL_FRAMEBUFFER, frm );

		//glViewport( 0, 0, 512, 512 );

		//GraphicsContext::DrawElements( EGLPrimitiveType::TRIANGLES, *RECT_IBO );	// instead of graphics context here will be used renderer

		////Utils::ImagePtr img = MakeRef< Utils::Image >( Utils::Image( { 512, 512 }, Utils::ERawImageFormat::RGBA8 ) );


		////OpenGLContext::GetTextureSubImage( text, 0, 0, 0, 0, 512, 512, 1, GL_RGBA, GL_UNSIGNED_BYTE, img->GetBytesSize(), img->RawData() );

		////Utils::ImageLoader::Save( "C:/Users/ДНС/Documents/Projects/VisualStudioProjects/GameEngine/Resources/Images/Picture.png", img );

		//glBindFramebuffer( GL_FRAMEBUFFER, 0 );	// binding default framebuffer

		//glViewport( 0, 0, 720, 720 );			// setting window size viewport
		//
		//glBindVertexArray( TVAO );

		//GraphicsContext::BindShader( *DefaultShader );



		////GraphicsContext::BindShader( *SHADER );
		
		////RECT_TEXTURE1->Bind( 0 );
		////RECT_TEXTURE2->Bind( 1 );

		////SHADER->SetUniform( "sampler1", 0 );
		////SHADER->SetUniform( "sampler2", 1 );

		/*dvShader = GpuApi::CreateShader( EShaderType::VERTEX_SHADER, { ResourceView( VShader.c_str(), VShader.size() ) }, "Default Vertex Shader" );
		dpShader = GpuApi::CreateShader( EShaderType::PIXEL_SHADER, { ResourceView( FShader.c_str(), FShader.size() ) }, "Default Pixel Shader" );*/

		dvShader = MakeRef< OpenGLShader >( EShaderType::VERTEX_SHADER, ResourceView( VertexShader.c_str(), VertexShader.size() ), "Default Vertex shader" );
		dpShader = MakeRef< OpenGLShader >( EShaderType::PIXEL_SHADER, ResourceView( FragmentShader.c_str(), FragmentShader.size()), "Default Vertex shader" );


		JF_ASSERT( dvShader->Compile(), "Failed to compile vertex shader." );
		JF_ASSERT( dpShader->Compile(), "Failed to compile pixel shader." );


		layout = MakeRef< CInputLayout >( 0u, "Input layout" );

		//GpuApi::STextureInitData textureInitData =
		//{
		//	,
		//	false,
		//	false,
		//	ETextureFormat::RGBA8,
		//	ETextureAccessBits( ETextureAccessBits::WRITE | ETextureAccessBits::READ ),
		//	ETextureType::TEXTURE_2D,
		//	NullPtr,
		//	0u,
		//	1080u * 1080u
		//};

		//renderTexture = MakeRef< OpenGLTexture> ( NullPtr, ETextureFormat::RGBA8, GpuApi::STextureSizeInfo{ 1080, 1080, 0, 4, 1 }, ETextureType::TEXTURE_2D );

		layout->Load( 0, Array< GpuApi::SInputLayoutDesc::LayoutUnit, 3 >
		{
			GpuApi::SInputLayoutDesc::LayoutUnit{ GpuApi::SInputElement( GpuApi::EInputElementFormat::R32G32B32_FLOAT, 12, 3, 1 ), 0 },
			GpuApi::SInputLayoutDesc::LayoutUnit{ GpuApi::SInputElement( GpuApi::EInputElementFormat::R32G32B32_FLOAT, 12, 3, 1 ), 0 },
			GpuApi::SInputLayoutDesc::LayoutUnit{ GpuApi::SInputElement( GpuApi::EInputElementFormat::R32G32_FLOAT, 8, 2, 1 ), 0 },
		} );


		state = MakeScoped< CRenderState >();

		state->SetInputLayout( layout->GetDescriptor() );

		state->SetShaders( Array< GpuApi::ShaderRef, 2 > { dvShader->GetDescriptor(), dpShader->GetDescriptor() } );

		state->SetVertexBuffers( Array< GpuApi::SRenderStateDesc::BufferUnit, 1 >
		 						 {
		 						 	 GpuApi::SRenderStateDesc::BufferUnit( RECT_VBO->GetDescriptor(), 0, 0 )
		 						 } );

		
		//GpuApi::SetRenderStateColorTargets(state, Array< GpuApi::SRenderStateDesc::RenderTargetUnit, 1 > { { renderTexture->GetDescriptor(), -1 } });

		state->UseDefaultRenderTargets();

		state->SetIndexBuffer( RECT_IBO->GetDescriptor() );

		JF_ASSERT( state->Cook(), "Failed to cook render state." );

		JF_ASSERT( state->Setup(), "Failed to setup render state." );
		
		//GLCALL( glViewport( 0, 0, 1080, 1080 ) );

		//OpenGLContext::ClearBufferfv( GL_COLOR, 0, Math::LinearColor::White.GetValue() );

		//GraphicsContext::DrawElements( EGLPrimitiveType::TRIANGLES, *RECT_IBO );	// instead of graphics context here will be used renderer
		//
		//auto img = MakeRef< Utils::Image >( Utils::Image( { 1080u, 1080u }, Utils::ERawImageFormat::RGBA8 ) );

		//GpuApi::GetTextureData( renderTexture->GetDescriptor(), GpuApi::STextureSubDataInfo(
		//						1080, 1080, 1, 0, 0, 0, img->GetBytesSize(), ETextureFormat::RGBA8 ), img->RawData() );
		//
		//JF_ASSERT( Utils::ImageLoader::Save( SaveImagePath, img ), "Failed to save image." );

		//GpuApi::UseRenderStateDefaultRenderTargets( state );

		GLCALL( glViewport( 0, 0, options.width, options.height ) );

		RECT_TEXTURE1->Bind( 0 );
		RECT_TEXTURE2->Bind( 1 );
	}

	void JSDLWindow::OnUpdate()
	{
		SDL_Event e;

		while ( SDL_PollEvent( &e ) )
		{
			// dispatch every event using event dispatcher.
			// all the callbacks should be invoked in application class
			// using event dispatcher

			if ( e.type == SDL_QUIT )
			{
				std::cout << "quit";
				this->Close();
				std::exit( 0 );
			}
		}
	}

	void JSDLWindow::OnRender()
	{
		/*GraphicsContext::ClearColor(Math::LinearColor::Blue);
		GraphicsContext::Clear();*/
		
		const float color[] = { 0.2f, 0.5f, 0.7f, 1.0f };
		
		OpenGLContext::ClearBufferfv( GL_COLOR, 0, color );

		// draw something with OpenGL here
		
		//RECT_VAO->Bind();

		//OpenGLContext::BindVertexArray( TVAO );

		//glDrawArrays(GL_POINTS, 0, 3);
		GraphicsContext::DrawElements( EGLPrimitiveType::TRIANGLES, *RECT_IBO );	// instead of graphics context here will be used renderer

		GraphicsContext::SwapBuffers( window.get() );
	}

	void JSDLWindow::OnDestroy()
	{
		this->Close();
	}

	void JSDLWindow::Close()
	{
		
		if ( bIsOpened )
		{	
			std::cout << "Releasing resource\n";
			window.reset();
			bIsOpened = false;
		}

		//glDeleteTextures( 1, &text );
		//glDeleteVertexArrays( 1, &vao );
		//glDeleteVertexArrays( 1, &TVAO );
		//glDeleteFramebuffers( 1, &frm );
	}
}
