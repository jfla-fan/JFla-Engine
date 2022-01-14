#include "OpenGLUtils.h"



namespace J::Graphics
{
	bool LogGLShaderErrors(JShader::IdType resource, GLenum request)
	{
		GLint result;
		OpenGLContext::GetShaderiv(resource, request, &result);

		if (result == GL_FALSE)
		{
			GLint length;
			OpenGLContext::GetShaderiv(resource, GL_INFO_LOG_LENGTH, &length);
			
			std::string log;
			log.resize(length);

			OpenGLContext::GetShaderInfoLog(resource, length, &length, &log[0]);

			if (!log.empty())
			{
				// #todo LOG THIS OUT WITH A MACRO
				std::cout << log << '\n';
			}

			return true;
		}

		return false;
	}


	bool LogGLProgramErrors(JShader::IdType resource, GLenum request)
	{
		GLint result;
		OpenGLContext::GetProgramiv(resource, request, &result);

		if (result == GL_FALSE)
		{
			GLint length;
			OpenGLContext::GetProgramiv(resource, GL_INFO_LOG_LENGTH, &length);

			std::string log;
			log.resize(length);

			OpenGLContext::GetProgramInfoLog(resource, length, &length, &log[0]);

			if (!log.empty())
			{
				// #todo LOG THIS OUT WITH A MACRO
				std::cout << log << '\n';
			}

			return true;
		}

		return false;
	}

}
