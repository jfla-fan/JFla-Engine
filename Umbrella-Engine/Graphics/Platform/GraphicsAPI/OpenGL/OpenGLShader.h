#pragma once

#include "OpenGLContext.h"

#include <string>
#include <map>
#include <iterator>
#include <filesystem>
#include "../Shader.h"


namespace UE::Graphics
{
	using namespace UE::Math;

	USE_SHADER_TYPES()



	class OpenGLShader final : public CShader
	{
	public:

		OpenGLShader( EShaderType type, const String& debugName = "" );

		OpenGLShader( EShaderType type, ResourceView shaderCode, const String& debugName = "" );

		~OpenGLShader() override;


		OpenGLShader( OpenGLShader&& another ) NOEXCEPT;
		OpenGLShader& operator = ( OpenGLShader&& another ) NOEXCEPT;


		void Release() override;

		bool Load( ResourceView source ) override;

		bool Load( const system::FilePath& path ) override;

		bool Compile() override;

	private:

		String DebugName;

	};

}

