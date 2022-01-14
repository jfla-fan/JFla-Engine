#pragma once
#include "GpuTypes.h"
#include <array>
#include "../../../Math/Math.h"



namespace J::Graphics
{
	using namespace Math;

	struct SPipelineStageInfo;


	// Shader class

	class JShader
	{
	public:

		using IdType = uint32;
		using UniformIdType = int32;

	public:

		JShader() { }

		JShader( const JShader& ) = delete;
		JShader& operator = ( const JShader& ) = delete;

		virtual ~JShader() { };


		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual IdType GetHandle() const = 0;
		virtual void Release() = 0;


		virtual void LoadFromString( const std::string& vertex, const std::string& fragment ) = 0;
		virtual void LoadFromString( const std::string& vertex, const std::string& geometry, const std::string& fragment ) = 0;
		
		virtual void Load( const system::FilePath& vertexPath, const system::FilePath& fragmentPath ) = 0;
		virtual void Load( const system::FilePath& vertexPath, const system::FilePath& geometry, const system::FilePath& fragmentPath ) = 0;

		virtual void SetUniform( const std::string& name, float		 value ) const { };
		virtual void SetUniform( const std::string& name, int		 value ) const { };
		virtual void SetUniform( const std::string& name, double	 value ) const { };
		virtual void SetUniform( const std::string& name, Vector1	 value ) const { };
		virtual void SetUniform( const std::string& name, Vector2	 value ) const { };
		virtual void SetUniform( const std::string& name, Vector3	 value ) const { };
		virtual void SetUniform( const std::string& name, Vector4	 value ) const { };
		virtual void SetUniform( const std::string& name, VectorInt1 value ) const { };
		virtual void SetUniform( const std::string& name, VectorInt2 value ) const { };
		virtual void SetUniform( const std::string& name, VectorInt3 value ) const { };
		virtual void SetUniform( const std::string& name, VectorInt4 value ) const { };

	};




}

