#include "SDLWindow.h"




namespace Engine::Graphics
{
	SDLWindow::SDLWindow(const WindowCreateOptions& InOptions)
		: options(InOptions), bIsOpened(false), window(nullptr, [](WindowHandle* window) {})
	{
		OnCreate(InOptions);
	}

	SDLWindow::~SDLWindow()
	{
		OnDestroy();
	}

	// examples of simple shaders
	const CHAR* v_shader = R"(
		#version 450 core

		in vec3 inPosition;
		in vec3 inColor;

		out vec3 outColor;

		void main()
		{
			gl_Position = vec4(inPosition, 1.0);
			outColor = inColor;
		}
	)";
	
	const CHAR* f_shader = R"(
		#version 450 core

		in vec3 outColor;
		
		void main()
		{
			gl_FragColor = vec4(outColor, 1.0);
		}
	)";


	void SDLWindow::OnCreate(const WindowCreateOptions& InOptions)
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
		
		// set time when the window was created.

		context = GraphicsContext::Create(window.get());
		bIsOpened = true;

		glViewport(0, 0, options.width, options.height);
		
		// saving vertex data into buffers

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		
		// triangle coordinates

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 3, 
			new float[] 
			{ 
				0.0f, 0.5f, 0.0f, 
				-0.5f, 0.0f, 0.0f,
				0.5f, 0.0f, 0.0f
			}, GL_STATIC_DRAW);
		
		// color for each vertex

		//const Math::LinearColor orange = Math::Color::ToLinearColor(Math::Color::Orange);
		const Math::LinearColor& yellow = Math::LinearColor::Yellow;

		RECT_COLOR = MakeScoped<OpenGLVertexBuffer>(new float[]
			{
				1.0f, 0.0f, 0.0f,	// red
					0.0f, 1.0f, 0.0f,	// green
					0.0f, 0.0f, 1.0f,	// blue
					yellow.R(), yellow.G(), yellow.B()
			}, sizeof(float) * 3 * 4);

		//glGenBuffers(1, &color);
		//glBindBuffer(GL_ARRAY_BUFFER, color);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, new float[]
		//	{
		//		1.0f, 0.0f, 0.0f,	// red
		//		0.0f, 1.0f, 0.0f,	// green
		//		0.0f, 0.0f, 1.0f,	// blue
		//		yellow.R(), yellow.G(), yellow.B()
		//	},
		//	GL_STATIC_DRAW);
		
		// vertex position attribute
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		// color attribute
		
		RECT_COLOR->Bind();
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindVertexArray(0);

		// rectangle

		glGenVertexArrays(1, &rect_vao);
		
		// rectangle coordinates

		RECT_VBO = OpenGLVertexBuffer::Create(
			new float[]
			{
				0.5f, 0.5f, 0.f,
				-0.5f, 0.5f, 0.f,
				-0.5f, -0.5f, 0.f,
				0.5f, -0.5f, 0.f,
			},
			sizeof(float) * 3 * 4
			);

		/*glGenBuffers(1, &rect_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, rect_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4,
			new float[]
			{
				0.5f, 0.5f, 0.f,
				-0.5f, 0.5f, 0.f,
				-0.5f, -0.5f, 0.f,
				0.5f, -0.5f, 0.f,
			}, GL_STATIC_DRAW);*/

		// index buffer

		RECT_IBO = OpenGLIndexBuffer::Create(
			new uint32[]
			{
				0, 1, 2,
				3, 0, 2
			},
			sizeof(uint32) * 6
		);

		/*glGenBuffers(1, &rect_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rect_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * 6, new uint32[]
			{
				0, 1, 2, 
				3, 0, 2
			}, GL_STATIC_DRAW);*/

		glBindVertexArray(rect_vao);
		
		RECT_VBO->Bind();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		RECT_COLOR->Bind();
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		// unbind everything
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Dealing with shaders
		
		vShader = glCreateShader(GL_VERTEX_SHADER);
		fShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vShader, 1, &v_shader, nullptr);
		glShaderSource(fShader, 1, &f_shader, nullptr);

		glCompileShader(vShader);
		
		CHAR log[512];
		int32 success;
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vShader, 512, nullptr, log);
			std::cout << "Vertex shader error: " << log << '\n';
			std::exit(1);
		}

		glCompileShader(fShader);

		glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fShader, 512, nullptr, log);
			std::cout << "Fragment shader error: " << log << '\n';
			std::exit(1);
		}

		program = glCreateProgram();

		glAttachShader(program, vShader);
		glAttachShader(program, fShader);

		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &success);
		
		if (!success)
		{
			glGetProgramInfoLog(program, 512, nullptr, log);
			std::cout << log << '\n';
			std::exit(1);
		}

		glDeleteShader(vShader);
		glDeleteShader(fShader);

		//GraphicsContext::SetPointSize(44.f);
		glUseProgram(program);
	}

	void SDLWindow::OnUpdate()
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

	void SDLWindow::OnRender()
	{
		/*GraphicsContext::ClearColor(Math::LinearColor::Blue);
		GraphicsContext::Clear();*/
		
		const float color[] = { 0.2f, 0.5f, 0.7f, 1.0f };
		
		glClearBufferfv(GL_COLOR, 0, color);

		// draw something with opengl here
		
		glBindVertexArray(rect_vao);

		//glDrawArrays(GL_POINTS, 0, 3);
		GraphicsContext::DrawElements(EGLPrimitiveType::TRIANGLES, *RECT_IBO);

		GraphicsContext::SwapBuffers(window.get());
	}

	void SDLWindow::OnDestroy()
	{
		this->Close();
	}

	void SDLWindow::Close()
	{
		if (bIsOpened)
		{
			std::cout << "Releasing resource\n";
			window.reset();
			bIsOpened = false;
		}

		glDeleteProgram(program);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(2, new uint32[]{ vbo, color });
	}
}
