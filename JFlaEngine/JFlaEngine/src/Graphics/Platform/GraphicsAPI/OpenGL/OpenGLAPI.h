#pragma once

#include "../../../../Core.h"



namespace Engine::Graphics
{

	enum class EGraphicsAPI : uint8
	{
		NONE = 0x00,
		OpenGL,
		Vulkan,
		Metal,
	};


	class OpenGLAPI
	{
	public:

		EGraphicsAPI GetAPI() const { return EGraphicsAPI::OpenGL; }

	};


}
