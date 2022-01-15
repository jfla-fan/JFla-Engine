#include <OpenImageIO/imageio.h>
#include "../Image/ImageLoader.h"
#include "../Image/ImageUtils.h"
#include "SDLWindow.h"
#include <array>
#include <vector>


namespace J::Graphics
{
	JSDLWindow::JSDLWindow(const SWindowCreateOptions& InOptions)
		: options(InOptions), bIsOpened(false), window(nullptr, [](WindowHandle* window) {})
	{
		OnCreate(InOptions);
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

	void JSDLWindow::OnCreate(const SWindowCreateOptions& InOptions)
	{
		window = Scope<WindowHandle, DestroyWindowCallback>
		(
			SDL_CreateWindow
			(
				options.title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				options.width, options.height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
			),

			[](WindowHandle* handle) { SDL_DestroyWindow(handle); }
		);
		
		// #todo set time when the window was created.

		context = GraphicsContext::Create(window.get());
		bIsOpened = true;

		glViewport(0, 0, options.width, options.height);
		

		const Math::LinearColor& yellow = Math::LinearColor::Yellow;

		// rectangle

		RECT_VAO = MakeRef<OpenGLVertexArray>();
		
		// rectangle coordinates and color

						// position							// color
		Vertices[0] = { Vector3 {0.5f, 0.5f, 0.f},		Vector3 {1.0f, 0.0f, 0.0f} };
		Vertices[1] = { Vector3 {-0.5f, 0.5f, 0.f},		Vector3 {0.0f, 1.0f, 0.0f} };
		Vertices[2] = { Vector3 {-0.5f, -0.5f, 0.f},	Vector3 {0.0f, 0.0f, 1.0f} };
		Vertices[3] = { Vector3 {0.5f, -0.5f, 0.f},		Vector3 { yellow.R(), yellow.G(), yellow.B() } };
		
		const std::string ImagePath = "C:/Users/ДНС/Documents/Projects/VisualStudioProjects/GameEngine/Resources/Images/sleepy-cat.jpg";
		const std::string DendyImagePath = "C:/Users/ДНС/Documents/Projects/VisualStudioProjects/GameEngine/Resources/Images/dendy.png";


		const std::string SaveImagePath = "C:/Users/ДНС/Documents/Projects/VisualStudioProjects/GameEngine/Resources/Images/flipped-sleepy-cat.jpg";

		Utils::ImagePtr image = Utils::ImageLoader::Load(ImagePath, true);

		if (!image)
		{
			std::cout << std::format("Could not load {} image.\n", ImagePath);
			std::exit(EXIT_FAILURE);
		}

		image->PrintImageMetaData(std::cout);

		Utils::ImagePtr dendyImage = Utils::ImageLoader::Load(DendyImagePath, true);

		if (!dendyImage)
		{
			std::cout << std::format("Could not load {} image.\n", DendyImagePath);
			std::exit(EXIT_FAILURE);
		}

		dendyImage->PrintImageMetaData(std::cout);


		//glGenTextures(1, &TextureID);
		//glGenTextures(1, &texture2);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, TextureID);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dendyImage->GetWidth(), dendyImage->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, dendyImage->RawData());
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		////glTextureParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		//glGenerateMipmap(GL_TEXTURE_2D);

		////glBindTexture(GL_TEXTURE_2D, 0);

		//glActiveTexture(GL_TEXTURE0 + 1);
		//glBindTexture(GL_TEXTURE_2D, texture2);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image->GetWidth(), image->GetHeight(), 0, GL_RGB8, GL_UNSIGNED_BYTE, image->RawData());
		//
		//float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		////glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		//glGenerateMipmap(GL_TEXTURE_2D);
		
		RECT_TEXTURE1 = MakeRef<OpenGLTexture>();
		RECT_TEXTURE2 = MakeRef<OpenGLTexture>();


		if (!RECT_TEXTURE1->Load(*image))
		{
			std::cout << "Failed to load texture\n";
			std::exit(1);
		}

		if (!RECT_TEXTURE2->Load(*dendyImage))
		{
			std::cout << "Failed to load texture\n";
			std::exit(1);
		}


		image.reset();
		dendyImage.reset();


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


		RECT_VAO->AddVertexLayout(RECT_VBO,
				std::array
				{
					SVertexAttribute::GetAttribute<Vector3>(),
					SVertexAttribute::GetAttribute<Vector3>(),
					SVertexAttribute::GetAttribute<Vector2>()
				}
			);

		// disable color attribute
		// this should result in a black square (square without color) [to enable add true to every attribute in constructor]
		RECT_VAO->RemoveVertexLayout(
				std::array
				{
					SVertexAttribute::GetAttribute<Vector3>(),
					SVertexAttribute::GetAttribute<Vector3>(),
				}
		);

		// Dealing with shaders

		SHADER = MakeRef<OpenGLShader>();
		SHADER->LoadFromString(VertexShader, FragmentShader);
		
		/*glUniform1i(glGetUniformLocation(SHADER->GetHandle(), "samp"), 0);
		glUniform1i(glGetUniformLocation(SHADER->GetHandle(), "cat"), 1);*/


		GraphicsContext::BindShader(*SHADER);
		
		RECT_TEXTURE1->Bind(0);
		RECT_TEXTURE2->Bind(1);

		SHADER->SetUniform("sampler1", 0);
		SHADER->SetUniform("sampler2", 1);
		//SHADER->SetUniform("sampler2", 1);
	}

	void JSDLWindow::OnUpdate()
	{
		SDL_Event e;

		while (SDL_PollEvent(&e))
		{
			// dispatch every event using event dispatcher.
			// all the callbacks should be invoked in application class
			// using event dispatcher

			if (e.type == SDL_QUIT)
			{
				std::cout << "quit";
				this->Close();
				std::exit(0);
			}
		}
	}

	void JSDLWindow::OnRender()
	{
		/*GraphicsContext::ClearColor(Math::LinearColor::Blue);
		GraphicsContext::Clear();*/
		
		const float color[] = { 0.2f, 0.5f, 0.7f, 1.0f };
		
		glClearBufferfv(GL_COLOR, 0, color);

		// draw something with OpenGL here
		
		RECT_VAO->Bind();

		//glDrawArrays(GL_POINTS, 0, 3);
		GraphicsContext::DrawElements(EGLPrimitiveType::TRIANGLES, *RECT_IBO);	// instead of graphics context here will be used renderer

		GraphicsContext::SwapBuffers(window.get());
	}

	void JSDLWindow::OnDestroy()
	{
		this->Close();
	}

	void JSDLWindow::Close()
	{
		if (bIsOpened)
		{
			std::cout << "Releasing resource\n";
			window.reset();
			bIsOpened = false;
		}

	}
}
