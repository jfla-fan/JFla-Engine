#include "OpenGLShader.h"
#include "OpenGLContext.h"
#include "../GpuApi.h"
#include <vector>



namespace UE::Graphics
{

	OpenGLShader::OpenGLShader( EShaderType type, const String& debugName )
		: CShader( type )
		, DebugName( debugName )
	{
		Descriptor = GpuApi::CreateShader( type, debugName );
	
		JF_ASSERT( Descriptor, "Failed to create shader object." );
	}

	OpenGLShader::OpenGLShader( EShaderType type, ResourceView shaderCode, const String& debugName )
		: CShader( type )
		, DebugName( debugName )
	{
		GpuApi::SShaderInitData initData = { shaderCode };

		Descriptor = GpuApi::CreateShader( type, initData, debugName );
	
		JF_ASSERT( Descriptor, "Failed to create shader object." );
	
		if ( !shaderCode.Empty() ) bLoaded = true;
	}


	OpenGLShader::OpenGLShader( OpenGLShader&& another ) NOEXCEPT
	{
		this->Release();

		this->Descriptor = another.Descriptor;
		this->DebugName = std::move( another.DebugName );
		this->bLoaded = another.bLoaded;

		another.Descriptor = ShaderRef::Null();
		another.bLoaded = false;
	}

	OpenGLShader::~OpenGLShader()
	{
		this->Release();
	}


	OpenGLShader& OpenGLShader::operator = ( OpenGLShader&& another ) NOEXCEPT
	{
		this->Release();

		this->Descriptor = another.Descriptor;
		this->DebugName = std::move( another.DebugName );
		this->bLoaded = another.bLoaded;

		another.Descriptor = ShaderRef::Null();
		another.bLoaded = false;

		return *this;
	}
	
	void OpenGLShader::Release()
	{
		GpuApi::SafeRelease( this->Descriptor );
	}


	bool OpenGLShader::Load( ResourceView source )
	{
		GpuApi::UploadShaderSourceCode( this->Descriptor, source );
		bLoaded = true;

		return true;
	}

	bool OpenGLShader::Load( const system::FilePath& path )
	{
		auto file = system::File( path );

		if ( !file.IsGood() )
		{
			return false;
		}

		String code = file.ReadAllText();

		GpuApi::UploadShaderSourceCode( Descriptor, ResourceView{ code.data(), static_cast< uint32 >( code.length() ) } );
		bLoaded = true;

		file.Close();

		return true;
	}

	bool OpenGLShader::Compile()
	{
		bCompiled = GpuApi::CompileShader( Descriptor );
	
		return bCompiled;
	}
}
