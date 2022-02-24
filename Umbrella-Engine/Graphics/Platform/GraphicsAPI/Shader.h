#pragma once
#include "GpuTypes.h"
#include <array>
#include "../../../Math/Math.h"



namespace UE::Graphics
{
	using namespace Math;

	USE_SHADER_TYPES()


	// Shader class

	class CShader
	{
	protected:

		CShader() : Type(), Descriptor(), bLoaded( false ), bCompiled( false ) { }
		CShader( EShaderType type ) : Type( type ), Descriptor(), bLoaded( false ), bCompiled( false ) { }

	public:


		CShader( const CShader& ) = delete;
		CShader& operator = ( const CShader& ) = delete;


		virtual ~CShader() { };


		virtual ShaderRef	GetDescriptor() const { return Descriptor; }
		virtual EShaderType GetType() const { return Type; }
		virtual bool		IsValid() const { return !Descriptor.IsNull(); }

		virtual void Release() = 0;

		virtual bool Load( ResourceView source ) = 0;

		virtual bool Load( const system::FilePath& path ) = 0;

		virtual bool IsLoaded() const { return bLoaded; }

		virtual void MarkLoaded( bool loaded = true ) { bLoaded = loaded; }

		virtual void MarkCompiled( bool compiled = true ) { bCompiled = compiled; }

		virtual bool Compile() = 0;

		virtual bool IsCompiled() const { return bCompiled; }

		/*virtual void SetUniform( const std::string& name, float		 value ) const { };
		virtual void SetUniform( const std::string& name, int		 value ) const { };
		virtual void SetUniform( const std::string& name, double	 value ) const { };
		virtual void SetUniform( const std::string& name, Vector1	 value ) const { };
		virtual void SetUniform( const std::string& name, Vector2	 value ) const { };
		virtual void SetUniform( const std::string& name, Vector3	 value ) const { };
		virtual void SetUniform( const std::string& name, Vector4	 value ) const { };
		virtual void SetUniform( const std::string& name, VectorInt1 value ) const { };
		virtual void SetUniform( const std::string& name, VectorInt2 value ) const { };
		virtual void SetUniform( const std::string& name, VectorInt3 value ) const { };
		virtual void SetUniform( const std::string& name, VectorInt4 value ) const { };*/

	protected:

		ShaderRef	Descriptor;
		EShaderType Type;
		bool		bLoaded;
		bool		bCompiled;

	};




}

