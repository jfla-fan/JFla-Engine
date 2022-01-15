#pragma once
#include "../Shader.h"
#include "OpenGLContext.h"


namespace J::Graphics
{
	bool LogGLShaderErrors(JShader::IdType resource, GLenum request);
	bool LogGLProgramErrors(JShader::IdType resource, GLenum request);

}
