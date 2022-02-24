#include "GpuDeviceData.h"
#include "../../GpuApiShader.h"


#define EMPTY_SHADER_CODE_ERR "Shader code is empty."


namespace UE::Graphics::GpuApi::Err
{
	static String ShaderErrorLog = "";
}

namespace UE::Graphics::GpuApi
{

	static GLenum Map( EShaderType type )
	{
		static HashMap< EShaderType, GLenum > ShaderTypeTable =
		{
			{ EShaderType::VERTEX_SHADER, GL_VERTEX_SHADER },
			{ EShaderType::PIXEL_SHADER, GL_FRAGMENT_SHADER },
			{ EShaderType::TESSELATION_SHADER, GL_TESS_CONTROL_SHADER },	// #todo DOESN'T MAKE SENSE FOR NOW
			{ EShaderType::COMPUTE_SHADER, GL_COMPUTE_SHADER },
			{ EShaderType::GEOMETRY_SHADER, GL_GEOMETRY_SHADER },
		};

		return ShaderTypeTable[type];
	}


	static bool LogGLShaderErrors( GLuint resource, GLenum request )
	{
		GLint result;
		OpenGLContext::GetShaderiv( resource, request, &result );

		if ( result == GL_FALSE )
		{
			GLint length;
			OpenGLContext::GetShaderiv( resource, GL_INFO_LOG_LENGTH, &length );

			String log;
			log.resize( length );

			OpenGLContext::GetShaderInfoLog( resource, length, &length, &log[0] );

			Err::ShaderErrorLog = std::move( log );

			return true;
		}

		return false;
	}

	static void UploadSourceCodeInternal( SShaderData& shaderData )
	{
		const auto codePtr = shaderData.Description.ByteCode.Pointer< GLchar >();
		const auto codeLength = static_cast< int32 >( shaderData.Description.ByteCode.Size() );

		OpenGLContext::ShaderSource( shaderData.Resource, 1, &codePtr, &codeLength );
	}


	ShaderRef AllocateShader( EShaderType type )
	{
		auto& deviceData = GetDeviceData();

		ShaderRef ref = deviceData.ShaderResources.Create();

		if ( !ref )
		{
			/* #TODO HALT HERE */
			return ref;
		}

		auto& shaderData = deviceData.ShaderResources.Data( ref );

		shaderData.Resource = OpenGLContext::CreateShader( Map( type ) );
	
		if ( shaderData.Resource == OpenGLContext::GInvalidGLResource )
		{
			// #todo HALT
			deviceData.ShaderResources.Destroy( ref );
			return ShaderRef::Null();
		}

		shaderData.Description.Type = type;

		return ref;
	}

	ShaderRef CreateShader( EShaderType type, const String& debugName )
	{
		auto& deviceData = GetDeviceData();

		ShaderRef ref = AllocateShader( type );

		if ( !ref )
		{
			return ref;
		}

		auto shaderData = deviceData.ShaderResources.Data( ref );

		if ( !debugName.empty() )
		{
			shaderData.Description.DebugName = debugName;
			OpenGLContext::ObjectLabel( GL_SHADER, shaderData.Resource, debugName.length(), debugName.c_str() );
		}


		return ref;
	}

	ShaderRef CreateShader( EShaderType type, const SShaderInitData& initData, const String& debugName )
	{
		auto& deviceData = GetDeviceData();

		ShaderRef ref = CreateShader( type, debugName );

		if ( !ref )
		{
			return ref;
		}

		auto& shaderData = deviceData.ShaderResources.Data( ref );

		shaderData.Description.ByteCode = initData.Code;

		UploadSourceCodeInternal( shaderData );

		return ref;
	}
	

	void DestroyShader( ShaderRef ref )
	{
		int32 newCount = SafeRelease( ref );
		JF_ASSERT( newCount == 0, "Cannot destroy shader. It used used somewhere else." );
	}

	bool IsResourceAllocated( ShaderRef ref )
	{
		return GetDeviceData().ShaderResources.IsInUse( ref );
	}

	bool RecompileShader( ShaderRef ref, ResourceView ShaderCode )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.ShaderResources.IsInUse( ref ), "shader does not exist" );

		auto& shaderData = deviceData.ShaderResources.Data( ref );

		shaderData.Description.ByteCode = ShaderCode;

		UploadSourceCodeInternal( shaderData );

		return CompileShader( ref );
	}

	bool CompileShader( ShaderRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.ShaderResources.IsInUse( ref ), "shader does not exist" );

		auto& shaderData = deviceData.ShaderResources.Data( ref );
	
		if ( !shaderData.Description.ByteCode )
		{
			// #todo LOG - EMPTY CODE
			shaderData.CompilationFailureMessage = EMPTY_SHADER_CODE_ERR;
			shaderData.Description.bCompiled = false;

			return false;
		}

		OpenGLContext::CompileShader( shaderData.Resource );

		if ( LogGLShaderErrors( shaderData.Resource, GL_COMPILE_STATUS ) )
		{
			shaderData.Description.bCompiled = false;
			shaderData.CompilationFailureMessage = std::move( Err::ShaderErrorLog );

			return false;
		}

		shaderData.Description.bCompiled = true;

		return true;
	}


	bool IsShaderCompiled( ShaderRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.ShaderResources.IsInUse( ref ), "shader does not exist" );

		auto& shaderData = deviceData.ShaderResources.Data( ref );

		return shaderData.Description.bCompiled;
	}

	void UploadShaderSourceCode( ShaderRef ref, ResourceView ShaderCode )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.ShaderResources.IsInUse( ref ), "shader does not exist" );

		auto& shaderData = deviceData.ShaderResources.Data( ref );

		shaderData.Description.ByteCode = ShaderCode;

		UploadSourceCodeInternal( shaderData );
	}

	String GetShaderCompilationFailureReason( ShaderRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.ShaderResources.IsInUse( ref ), "shader does not exist" );

		auto& shaderData = deviceData.ShaderResources.Data( ref );

		return shaderData.CompilationFailureMessage;
	}

	SShaderDesc GetShaderDesc( ShaderRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.ShaderResources.IsInUse( ref ), "shader does not exist" );

		auto& shaderData = deviceData.ShaderResources.Data( ref );

		return shaderData.Description;
	}

	EShaderType GetShaderType( ShaderRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.ShaderResources.IsInUse( ref ), "shader does not exist" );

		auto& shaderData = deviceData.ShaderResources.Data( ref );

		return shaderData.Description.Type;
	}

	String GetShaderDebugName( ShaderRef ref )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.ShaderResources.IsInUse( ref ), "shader does not exist" );

		auto& shaderData = deviceData.ShaderResources.Data( ref );

		return shaderData.Description.DebugName;
	}

	void SetShaderDebugName( ShaderRef ref, const String& debugName )
	{
		auto& deviceData = GetDeviceData();
		JF_ASSERT( deviceData.ShaderResources.IsInUse( ref ), "shader does not exist" );

		auto& shaderData = deviceData.ShaderResources.Data( ref );

		shaderData.Description.DebugName = debugName;

		OpenGLContext::ObjectLabel( GL_SHADER, shaderData.Resource, debugName.length(), debugName.c_str() );
	}

}
