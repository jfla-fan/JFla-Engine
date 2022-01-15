#pragma once

#include "../../../Core.h"

#include "OpenGL/GLRenderer.h"
#include "OpenGL/OpenGLContext.h"
#include "OpenGL/OpenGLShader.h"
#include "OpenGL/OpenGLTexture.h"
#include "OpenGL/OpenGLVertexBuffer.h"
#include "OpenGL/OpenGLIndexBuffer.h"
#include "OpenGL/OpenGLVertexArray.h"


// #TODO DO WE NEED THIS FILE?

namespace J::Graphics
{
	enum class EGraphicsAPI : uint8
	{
		UNDEFINED = 0x00,
		OpenGL,
		Vulkan,
		Metal,
	};


}