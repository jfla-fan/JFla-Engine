#pragma once

#include "GpuTypes.h"



namespace UE::Graphics::GpuApi
{



	/************************************************************************/
	/*								SHADERS                                 */
	/************************************************************************/



	ShaderRef		CreateShader( EShaderType type, const String& debugName = "" );

	//< DOES NOT AUTOMATICALLY COMPILE SHADER!
	ShaderRef		CreateShader( EShaderType type, const SShaderInitData& initData, const String& debugName = "" );

	void			DestroyShader( ShaderRef ref );

	bool			IsResourceAllocated( ShaderRef ref );

	bool			CompileShader( ShaderRef ref );

	bool			RecompileShader( ShaderRef ref, ResourceView ShaderCode );

	void			UploadShaderSourceCode( ShaderRef ref, ResourceView ShaderCode );

	bool			IsShaderCompiled( ShaderRef ref );

	String			GetShaderCompilationFailureReason( ShaderRef ref );

	SShaderDesc		GetShaderDesc( ShaderRef ref );

	EShaderType		GetShaderType( ShaderRef ref );

	String			GetShaderDebugName( ShaderRef ref );

	void			SetShaderDebugName( ShaderRef ref, const String& debugName );

}
