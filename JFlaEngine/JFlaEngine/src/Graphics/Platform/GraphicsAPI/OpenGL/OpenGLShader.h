#pragma once
#include <string>
#include "Shader.h"



namespace Engine::Graphics
{
	enum class EShaderType
	{
		VERTEX_SHADER,
		GEOMETRY_SHADER,
		FRAGMENT_SHADER,
	};


	class OpenGLShader : public Shader
	{
	public:

		OpenGLShader() = delete;
		OpenGLShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
		OpenGLShader(const std::string& shaderSource, EShaderType type);

		OpenGLShader(const OpenGLShader& another) = delete;
		OpenGLShader& operator = (const OpenGLShader& another) = delete;
		OpenGLShader(OpenGLShader&& another) = delete;
		OpenGLShader& operator = (OpenGLShader&& another) = delete;

		void Bind() const override { }

		void Unbind() const override { }
	};


}

