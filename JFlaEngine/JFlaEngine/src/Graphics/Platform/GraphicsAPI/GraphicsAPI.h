#pragma once

#include "../../../Core.h"

#include "OpenGL/GLRenderer.h"
#include "OpenGL/OpenGLContext.h"
#include "OpenGL/OpenGLShader.h"
#include "OpenGL/OpenGLVertexBuffer.h"
#include "OpenGL/OpenGLIndexBuffer.h"


namespace Engine::Graphics
{
	enum class EGraphicsAPI : uint8
	{
		UNDEFINED = 0x00,
		OpenGL,
		Vulkan,
		Metal,
	};
}