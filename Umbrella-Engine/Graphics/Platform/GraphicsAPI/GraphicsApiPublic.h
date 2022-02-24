#pragma once

#include "../../../Core.h"

#include "OpenGL/GLRenderer.h"
#include "OpenGL/OpenGLContext.h"
#include "OpenGL/OpenGLShader.h"
#include "OpenGL/OpenGLTexture.h"
#include "OpenGL/OpenGLVertexBuffer.h"
#include "OpenGL/OpenGLIndexBuffer.h"
#include "InputLayout.h"
#include "RenderState.h"


// #TODO DO WE NEED THIS FILE?

namespace UE::Graphics
{
	enum class EGraphicsAPI : uint8
	{
		UNDEFINED = 0x00,
		OpenGL,
		Vulkan,
		Metal,
	};


}