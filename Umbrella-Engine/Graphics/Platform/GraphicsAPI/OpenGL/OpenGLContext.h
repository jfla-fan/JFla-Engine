#pragma once
#include "../../../../Core.h"
#include "glad/glad.h"

#define GL_ENUM( element ) ( GLenum )( element )

#ifdef JF_DEBUG

	#define GL_ERROR_CODE_STR( code ) OpenGLContext::GetStrErrorCode( code )

	#define GLCALL( call ) do													\
	{																			\
		call;																	\
		GLenum error = glGetError();											\
		bool has_error = error != GL_NO_ERROR;									\
		if ( has_error )														\
			std::cout << "OpenGL error in " << #call << " instruction.\n";		\
		while ( error != GL_NO_ERROR )											\
		{																		\
			std::cout << std::format( "Error code - {}\n.",						\
										GL_ERROR_CODE_STR( error ) );			\
			error = glGetError();												\
		}																		\
		if ( has_error ) JF_DEBUG_BREAK();										\
	} while ( 0 )

#else

	#define GLCALL( call ) do { call; } while( 0 )

#endif




namespace UE::Graphics
{

	// #TODO DO WE REALLY NEED THIS?
	struct OpenGLContext
	{
		constexpr static GLuint GInvalidGLResource = 0;

		/**
		 * POSSIBLY IMPLEMENT HERE OPENGL STATISTICS (INHERITED FROM GPU API STATISTICS ??? )
		 * 
		 * BindCalls
		 * GenCalls
		 * DeleteCalls
		 * Errors
		 * ShaderCalls
		 * 
		 * 
		 * etc...
		 */


		// constants

		static constexpr FORCEINLINE auto GetMaxVertexAttributesCount() { return GL_MAX_VERTEX_ATTRIBS; }

		static constexpr FORCEINLINE auto GetMaxCombinedTextureUnits() { int32 total; GLCALL(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &total)); return total; }

		// specific

		static constexpr const CHAR* GetStrErrorCode(GLenum error)
		{
#define CASE_LABEL(code)	\
		case code:			\
			return #code

			switch (error)
			{
				CASE_LABEL(GL_NO_ERROR);
				CASE_LABEL(GL_INVALID_ENUM);
				CASE_LABEL(GL_INVALID_VALUE);
				CASE_LABEL(GL_INVALID_OPERATION);
				CASE_LABEL(GL_INVALID_FRAMEBUFFER_OPERATION);
				CASE_LABEL(GL_OUT_OF_MEMORY);
				CASE_LABEL(GL_STACK_UNDERFLOW);
				CASE_LABEL(GL_STACK_OVERFLOW);

			default:
				return "[Unknown error]";
			}
#undef CASE_LABEL

		}
		

		/************************************************************************/
		/*							OPENGL API									*/
		/************************************************************************/

		static FORCEINLINE void ClearBufferfv( GLenum Buffer, GLint DrawBufferIndex, const GLfloat* Value )
		{
			GLCALL( glClearBufferfv( Buffer, DrawBufferIndex, Value ) );
		}

		static FORCEINLINE void ClearBufferfi( GLenum Buffer,
											   GLint DrawBufferIndex,
											   GLfloat Depth,
											   GLint Stencil )
		{
			GLCALL( glClearBufferfi( Buffer, DrawBufferIndex, Depth, Stencil ) );
		}

		static FORCEINLINE void ClearBufferiv( GLenum Buffer, GLint DrawBufferIndex, const GLint* Value )
		{
			GLCALL( glClearBufferiv( Buffer, DrawBufferIndex, Value ) );
		}

		static FORCEINLINE void ClearDepth( GLdouble Depth )
		{
			GLCALL( glClearDepth( Depth ) );
		}

		static FORCEINLINE void ClearColor( GLfloat Red, GLfloat Green, GLfloat Blue, GLfloat Alpha )
		{
			GLCALL( glClearColor( Red, Green, Blue, Alpha ) );
		}

		static FORCEINLINE void Clear( GLbitfield Mask )
		{
			GLCALL( glClear( Mask ) );
		}
		
		static FORCEINLINE void PointSize( GLfloat Size )
		{
			check( Size >= 1.f );
			GLCALL( glPointSize( Size ) );
		}

		// Buffers

		static FORCEINLINE void GenBuffers( GLsizei n, GLuint* buffers )
		{
			GLCALL( glGenBuffers( n, buffers ) );
		}

		static FORCEINLINE void CreateBuffers( GLsizei n, GLuint* buffers )
		{
			GLCALL( glCreateBuffers( n, buffers ) );
		}

		static FORCEINLINE void DeleteBuffers( GLsizei n, GLuint* buffers )
		{
			GLCALL( glDeleteBuffers( n, buffers ) );
		}

		static FORCEINLINE void BindBuffer( GLenum Target, GLuint Buffer )
		{
			GLCALL( glBindBuffer( Target, Buffer ) );
		}

		static FORCEINLINE void UnbindBuffer( GLenum Target )
		{
			GLCALL( glBindBuffer( Target, GInvalidGLResource ) );
		}

		static FORCEINLINE void BufferData( GLenum Target,
											GLsizeiptr Size,
											CMemPtr Data,
											GLenum Usage )
		{
			GLCALL( glBufferData( Target, Size, Data, Usage ) );
		}

		static FORCEINLINE void NamedBufferData( GLuint Buffer,
												 GLsizeiptr Size,
												 CMemPtr Data,
												 GLenum Usage )
		{
			GLCALL( glNamedBufferData( Buffer, Size, Data, Usage ) );
		}

		static FORCEINLINE void CopyBufferSubData( GLenum ReadTarget,
												   GLenum WriteTarget,
												   GLintptr ReadOffset,
												   GLintptr WriteOffset,
												   GLsizeiptr Size )
		{
			GLCALL( glCopyBufferSubData( ReadTarget, WriteTarget, ReadOffset, WriteOffset, Size ) );
		}

		static FORCEINLINE void CopyNamedBufferSubData( GLuint ReadBuffer,
														GLuint WriteBuffer,
														GLintptr ReadOffset,
														GLintptr WriteOffset,
														GLsizeiptr Size )
		{
			GLCALL( glCopyNamedBufferSubData( ReadBuffer, WriteBuffer, ReadOffset, WriteOffset, Size ) );
		}

		static FORCEINLINE void BufferSubData( GLenum Target,
											   GLintptr Offset,
											   GLsizeiptr Size,
											   CMemPtr Data )
		{
			GLCALL( glBufferSubData( Target, Offset, Size, Data ) );
		}

		static FORCEINLINE void NamedBufferSubData( GLuint Buffer,
													GLintptr Offset,
													GLsizeiptr Size,
													CMemPtr Data )
		{
			GLCALL( glNamedBufferSubData( Buffer, Offset, Size, Data ) );
		}

		static FORCEINLINE void GetBufferSubData( GLenum Target,
												  GLintptr Offset,
												  GLsizeiptr Size,
												  MemPtr Data )
		{
			GLCALL( glGetBufferSubData( Target, Offset, Size, Data ) );
		}

		static FORCEINLINE void GetNamedBufferSubData( GLuint Buffer,
													   GLintptr Offset,
													   GLsizeiptr Size,
													   MemPtr Data )
		{
			GLCALL( glGetNamedBufferSubData( Buffer, Offset, Size, Data ) );
		}

		static FORCEINLINE void BufferStorage( GLenum Target,
											   GLsizeiptr Size,
											   CMemPtr Data,
											   GLbitfield Flags )
		{
			GLCALL( glBufferStorage( Target, Size, Data, Flags ) );
		}

		static FORCEINLINE void NamedBufferStorage( GLuint Buffer,
													GLsizeiptr Size,
													CMemPtr Data,
													GLbitfield Flags )
		{
			GLCALL( glNamedBufferStorage( Buffer, Size, Data, Flags ) );
		}


		// Shaders

		static FORCEINLINE GLuint CreateShader( GLenum Type )
		{
			GLuint resource;
			GLCALL( resource = glCreateShader( Type ) );
		
			return resource;
		}

		static FORCEINLINE void ShaderSource( GLuint Shader,
											  GLsizei Count,
											  const GLchar* const *String,
											  const GLint* Length )
		{
			GLCALL( glShaderSource( Shader, Count, String, Length ) );
		}

		static FORCEINLINE void CompileShader( GLuint Shader )
		{
			GLCALL( glCompileShader( Shader ) );
		}

		static FORCEINLINE void GetShaderiv( GLuint Shader, GLenum ParameterName, GLint* Value )
		{
			GLCALL( glGetShaderiv( Shader, ParameterName, Value ) );
		}

		static FORCEINLINE void GetShaderInfoLog( GLuint Shader,
												  GLsizei BufferSize,
												  GLsizei* Length,
												  GLchar* InfoLogBuffer )
		{
			GLCALL( glGetShaderInfoLog( Shader, BufferSize, Length, InfoLogBuffer ) );
		}

		static FORCEINLINE void DeleteShader( GLuint Shader )
		{
			GLCALL( glDeleteShader( Shader ) );
		}

		static FORCEINLINE void DetachShader( GLuint Program, GLuint Shader )
		{
			GLCALL( glDetachShader( Program, Shader ) );
		}

		// uniforms

		static FORCEINLINE GLint GetUniformLocation( GLuint Program, const GLchar* Name )
		{
			GLint result;
			GLCALL( result = glGetUniformLocation( Program, Name ) );
			
			return result;
		}

		static FORCEINLINE void Uniform1f( GLint location, float v0 )
		{
			GLCALL( glUniform1f( location, v0 ) );
		}

		static FORCEINLINE void Uniform2f( GLint location, float v0, float v1 )
		{
			GLCALL( glUniform2f( location, v0, v1 ) );
		}

		static FORCEINLINE void Uniform3f( GLint location, float v0, float v1, float v2 )
		{
			GLCALL( glUniform3f( location, v0, v1, v2 ) );
		}

		static FORCEINLINE void Uniform4f( GLint location, float v0, float v1, float v2, float v3 )
		{
			GLCALL( glUniform4f( location, v0, v1, v2, v3 ) );
		}

		static FORCEINLINE void Uniform1i( GLint location, GLint v0 )
		{
			GLCALL( glUniform1i( location, v0 ) );
		}

		static FORCEINLINE void Uniform2i( GLint location, GLint v0, GLint v1 )
		{
			GLCALL( glUniform2i( location, v0, v1 ) );
		}

		static FORCEINLINE void Uniform3i( GLint location, GLint v0, GLint v1, GLint v2 )
		{
			GLCALL( glUniform3i( location, v0, v1, v2 ) );
		}

		static FORCEINLINE void Uniform4i( GLint location, GLint v0, GLint v1, GLint v2, GLint v3 )
		{
			GLCALL( glUniform4i( location, v0, v1, v2, v3 ) );
		}

		static FORCEINLINE void Uniform1d( GLint location, GLdouble v0 )
		{
			GLCALL( glUniform1d( location, v0 ) );
		}

		static FORCEINLINE void Uniform2d( GLint location, GLdouble v0, GLdouble v1 )
		{
			GLCALL( glUniform2d( location, v0, v1 ) );
		}

		static FORCEINLINE void Uniform3d( GLint location, GLdouble v0, GLdouble v1, GLdouble v2 )
		{
			GLCALL( glUniform3d( location, v0, v1, v2 ) );
		}

		static FORCEINLINE void Uniform4d( GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3 )
		{
			GLCALL( glUniform4d( location, v0, v1, v2, v3 ) );
		}

		static FORCEINLINE void UniformMatrix2fv( GLint location,
												  GLsizei count,
												  GLboolean transpose,
												  const GLfloat* value )
		{
			GLCALL( glUniformMatrix2fv( location, count, transpose, value ) );
		}

		static FORCEINLINE void UniformMatrix2x3fv( GLint location,
													GLsizei count,
													GLboolean transpose,
													const GLfloat* value )
		{
			GLCALL( glUniformMatrix2x3fv( location, count, transpose, value ) );
		}
		
		static FORCEINLINE void UniformMatrix3x2fv( GLint location,
													GLsizei count,
													GLboolean transpose,
													const GLfloat* value )
		{
			GLCALL( glUniformMatrix3x2fv( location, count, transpose, value ) );
		}
		
		static FORCEINLINE void UniformMatrix4fv( GLint location,
												  GLsizei count,
												  GLboolean transpose,
												  const GLfloat* value )
		{
			GLCALL( glUniformMatrix4fv( location, count, transpose, value ) );
		}

		static FORCEINLINE void UniformMatrix3x4fv( GLint location,
													GLsizei count,
													GLboolean transpose,
													const GLfloat* value )
		{
			GLCALL( glUniformMatrix3x4fv( location, count, transpose, value ) );
		}

		static FORCEINLINE void UniformMatrix4x3fv( GLint location,
													GLsizei count,
													GLboolean transpose,
													const GLfloat* value )
		{
			GLCALL( glUniformMatrix4x3fv( location, count, transpose, value ) );
		}

		static FORCEINLINE void UniformMatrix2x4fv( GLint location,
													GLsizei count,
													GLboolean transpose,
													const GLfloat* value )
		{
			GLCALL( glUniformMatrix2x4fv( location, count, transpose, value ) );
		}

		static FORCEINLINE void UniformMatrix4x2fv( GLint location,
													GLsizei count,
													GLboolean transpose,
													const GLfloat* value )
		{
			GLCALL( glUniformMatrix4x2fv( location, count, transpose, value ) );
		}

		// Program

		static FORCEINLINE GLuint CreateProgram()
		{
			GLuint resource;
			GLCALL( resource = glCreateProgram() );

			return resource;
		}

		static FORCEINLINE void UseProgram( GLuint Program )
		{
			GLCALL( glUseProgram( Program ) );
		}

		static FORCEINLINE void DeleteProgram( GLuint Program )
		{
			GLCALL( glDeleteProgram( Program ) );
		}

		static FORCEINLINE void ValidateProgram( GLuint Program )
		{
			GLCALL( glValidateProgram( Program ) );
		}

		static FORCEINLINE void AttachShader( GLuint Program, GLuint Shader )
		{
			GLCALL( glAttachShader( Program, Shader ) );
		}

		static FORCEINLINE void LinkProgram( GLuint Program )
		{
			GLCALL( glLinkProgram( Program ) );
		}

		static FORCEINLINE void GetProgramiv( GLuint Program, GLenum ParameterName, GLint* Value )
		{
			GLCALL( glGetProgramiv( Program, ParameterName, Value ) );
		}

		static FORCEINLINE void GetProgramInfoLog( GLuint Program,
												   GLsizei BufferSize,
												   GLsizei* Length,
												   GLchar* InfoLogBuffer )
		{
			GLCALL( glGetProgramInfoLog( Program, BufferSize, Length, InfoLogBuffer ) );
		}

		static FORCEINLINE GLuint CreateShaderProgram( GLenum Type, GLsizei Count, const GLchar* const *Sources )
		{
			GLuint program;
			GLCALL( program = glCreateShaderProgramv( Type, Count, Sources ) );
		
			return program;
		}

		// VAO

		static FORCEINLINE void GenVertexArrays( GLsizei Count, GLuint* Arrays )
		{
			GLCALL( glGenVertexArrays( Count, Arrays ) );
		}

		static FORCEINLINE void CreateVertexArrays( GLsizei Count, GLuint* Arrays )
		{
			GLCALL( glCreateVertexArrays( Count, Arrays ) );
		}

		static FORCEINLINE void DeleteVertexArrays( GLsizei Count, const GLuint* Arrays )
		{
			GLCALL( glDeleteVertexArrays( Count, Arrays ) );
		}

		static FORCEINLINE void BindVertexArray( GLuint Array )
		{
			GLCALL( glBindVertexArray( Array ) );
		}

		static FORCEINLINE void UnbindVertexArray()
		{
			GLCALL( glBindVertexArray( 0 ) );
		}

		static FORCEINLINE void EnableVertexAttribArray( GLuint Index )
		{
			GLCALL( glEnableVertexAttribArray( Index ) );
		}

		static FORCEINLINE void DisableVertexAttribArray( GLuint Index )
		{
			GLCALL( glDisableVertexAttribArray( Index ) );
		}

		static FORCEINLINE void EnableVertexArrayAttrib( GLuint VAO, GLuint Index )
		{
			GLCALL( glEnableVertexArrayAttrib( VAO, Index ) );
		}

		static FORCEINLINE void DisableVertexArrayAttrib( GLuint VAO, GLuint Index )
		{
			GLCALL( glDisableVertexArrayAttrib( VAO, Index ) );
		}

		static FORCEINLINE void VertexAttribPointer( GLuint Index,
													 GLint Size,
													 GLenum Type,
													 GLboolean Normalized,
													 GLsizei Stride,
													 CMemPtr Offset )
		{
			GLCALL( glVertexAttribPointer( Index, Size, Type, Normalized, Stride, Offset ) );
		}
		
		static FORCEINLINE void VertexAttribIPointer( GLuint Index,
													  GLint Size,
													  GLenum Type,
													  GLsizei Stride,
													  CMemPtr Offset)
		{
			GLCALL( glVertexAttribIPointer( Index, Size, Type, Stride, Offset ) );
		}

		static FORCEINLINE void VertexAttribLPointer( GLuint Index,
													  GLint Size,
													  GLenum Type,
													  GLsizei Stride,
													  CMemPtr Offset)
		{
			GLCALL( glVertexAttribLPointer( Index, Size, Type, Stride, Offset ) );
		}

		// #TODO not sure about this
		static FORCEINLINE void VertexAttribFormat( GLuint attribIndex,
													GLint size,
													GLenum type,
													GLboolean normalized,
													GLuint relativeOffset )
		{
			GLCALL( glVertexAttribFormat( attribIndex, size, type, normalized, relativeOffset ) );
		}


		static FORCEINLINE void VertexArrayAttribFormat( GLuint VAO,
														 GLuint AttributeIndex,
														 GLint Size,
														 GLenum Type,
														 GLboolean Normalized,
														 GLuint RelativeOffset )
		{
			GLCALL( glVertexArrayAttribFormat( VAO, AttributeIndex, Size, Type, Normalized, RelativeOffset ) );
		}

		static FORCEINLINE void VertexArrayAttribIFormat( GLuint VAO,
														  GLuint AttributeIndex,
														  GLint Size,
														  GLenum Type,
														  GLuint RelativeOffset )
		{
			GLCALL( glVertexArrayAttribIFormat( VAO, AttributeIndex, Size, Type, RelativeOffset ) );
		}

		static FORCEINLINE void VertexArrayAttribLFormat( GLuint VAO,
														  GLuint AttributeIndex,
														  GLint Size,
														  GLenum Type,
														  GLuint RelativeOffset )
		{
			GLCALL( glVertexArrayAttribLFormat( VAO, AttributeIndex, Size, Type, RelativeOffset ) );
		}

		static FORCEINLINE void VertexAttribBinding( GLuint AttributeIndex, GLuint BindingIndex )
		{
			GLCALL( glVertexAttribBinding( AttributeIndex, BindingIndex ) );
		}

		static FORCEINLINE void VertexArrayAttribBinding( GLuint VAO,
														  GLuint AttributeIndex,
														  GLuint BindingIndex )
		{
			GLCALL( glVertexArrayAttribBinding( VAO, AttributeIndex, BindingIndex ) );
		}


		static FORCEINLINE void BindVertexBuffer( GLuint Bindingindex,
												  GLuint Buffer,
												  GLintptr Offset,
												  GLsizei Stride )
		{
			GLCALL( glBindVertexBuffer( Bindingindex, Buffer, Offset, Stride ) );
		}

		static FORCEINLINE void VertexArrayVertexBuffer( GLuint VAO,
														 GLuint Bindingindex,
														 GLuint Buffer,
														 GLintptr Offset,
														 GLsizei Stride )
		{
			GLCALL( glVertexArrayVertexBuffer( VAO, Bindingindex, Buffer, Offset, Stride ) );
		}


		static FORCEINLINE void BindVertexBuffers( GLuint First,
												   GLsizei Count,
												   const GLuint* Buffers,
												   const GLintptr* Offsets,
												   const GLsizei* Strides )
		{
			GLCALL( glBindVertexBuffers( First, Count, Buffers, Offsets, Strides ) );
		}

		static FORCEINLINE void VertexArrayVertexBuffers( GLuint VAO,
														  GLuint First,
														  GLsizei Count,
														  const GLuint* Buffers,
														  const GLintptr* Offsets,
														  const GLsizei* Strides )
		{
			GLCALL( glVertexArrayVertexBuffers( VAO, First, Count, Buffers, Offsets, Strides ) );
		}


		// Textures

		static FORCEINLINE void GenTextures( GLsizei n, GLuint* textures )
		{
			GLCALL( glGenTextures( n, textures ) );
		}

		static FORCEINLINE void CreateTextures( GLenum Target, GLsizei Count, GLuint* Textures )
		{
			GLCALL( glCreateTextures( Target, Count, Textures ) );
		}

		static FORCEINLINE void BindTexture( GLenum Target, GLuint Texture )
		{
			GLCALL( glBindTexture( Target, Texture));
		}

		static FORCEINLINE void UnbindTexture( GLenum Target )
		{
			GLCALL( glBindTexture( Target, GInvalidGLResource ) );
		}

		static FORCEINLINE void ActiveTexture( GLenum Texture )
		{
			GLCALL( glActiveTexture( Texture ) );
		}

		static FORCEINLINE void GenerateMipmap( GLenum Target )
		{
			GLCALL( glGenerateMipmap( Target ) );
		}

		static FORCEINLINE void GenerateTextureMipmap( GLuint Texture )
		{
			GLCALL( glGenerateTextureMipmap( Texture ) );
		}

		static FORCEINLINE void DeleteTextures( GLsizei n, const GLuint* textures )
		{
			GLCALL( glDeleteTextures( n, textures ) );
		}

		static FORCEINLINE void TexParameterf( GLenum Target,
											   GLenum ParamName,
											   GLfloat Param )
		{
			GLCALL( glTexParameterf( Target, ParamName, Param ) );
		}

		static FORCEINLINE void TexParameteri( GLenum Target,
											   GLenum ParamName,
											   GLint Param )
		{
			GLCALL( glTexParameteri( Target, ParamName, Param ) );
		}

		static FORCEINLINE void TextureParameterf( GLuint Texture,
												   GLenum ParamName,
												   GLfloat Param )
		{
			GLCALL( glTextureParameterf( Texture, ParamName, Param ) );
		}

		static FORCEINLINE void TextureParameteri( GLuint Texture,
												   GLenum ParamName,
												   GLint Param )
		{
			GLCALL( glTextureParameteri( Texture, ParamName, Param ) );
		}

		static FORCEINLINE void TexParameterfv( GLenum Target,
												GLenum ParamName,
												const GLfloat* Params )
		{
			GLCALL( glTextureParameterfv( Target, ParamName, Params ) );
		}

		static FORCEINLINE void TexParameteriv( GLenum Target,
												GLenum ParamName,
												const GLint* Params )
		{
			GLCALL( glTextureParameteriv( Target, ParamName, Params ) );
		}

		static FORCEINLINE void glTexParameterIiv( GLenum Target,
												   GLenum ParamName,
												   const GLint* Params )
		{
			GLCALL( glTextureParameterIiv( Target, ParamName, Params ) );
		}

		static FORCEINLINE void TexParameterIuiv( GLenum Target,
												  GLenum ParamName,
												  const GLuint* Params )
		{
			GLCALL( glTextureParameterIuiv( Target, ParamName, Params ) );
		}

		static FORCEINLINE void TextureParameterfv( GLuint Texture,
													GLenum ParamName,
													const GLfloat* Params )
		{
			GLCALL( glTextureParameterfv( Texture, ParamName, Params ) );
		}

		static FORCEINLINE void TextureParameteriv( GLuint Texture,
													GLenum ParamName,
													const GLint* Params )
		{
			GLCALL( glTextureParameteriv( Texture, ParamName, Params ) );
		}

		static FORCEINLINE void TextureParameterIiv( GLuint Texture,
													 GLenum ParamName,
													 const GLint* Params )
		{
			GLCALL( glTextureParameterIiv( Texture, ParamName, Params ) );
		}

		static FORCEINLINE void TextureParameterIuiv( GLuint Texture,
													  GLenum ParamName,
													  const GLuint* Params )
		{
			GLCALL( glTextureParameterIuiv( Texture, ParamName, Params ) );
		}

		static FORCEINLINE void TexImage2D( GLenum Target,
											GLint Level,
											GLint InternalFormat,
											GLsizei Width,
											GLsizei Height,
											GLint Border,
											GLenum Format,
											GLenum Type,
											CMemPtr Data )
		{
			GLCALL( glTexImage2D( Target, Level, InternalFormat, Width, Height, Border, Format, Type, Data ) );
		}


		static FORCEINLINE void TexStorage2D( GLenum Target,
											  GLsizei Levels,
											  GLenum Internalformat,
											  GLsizei Width,
											  GLsizei Height )
		{
			GLCALL( glTexStorage2D( Target, Levels, Internalformat, Width, Height ) );
		}

		static FORCEINLINE void TextureStorage2D( GLuint Texture,
												  GLsizei Levels,
												  GLenum Internalformat,
												  GLsizei Width,
												  GLsizei Height )
		{
			GLCALL( glTextureStorage2D( Texture, Levels, Internalformat, Width, Height ) );
		}


		static FORCEINLINE void TexSubImage2D( GLenum Target,
											   GLint Level,
											   GLint xOffset,
											   GLint yOffset,
											   GLsizei Width,
											   GLsizei Height,
											   GLenum Format,
											   GLenum Type,
											   CMemPtr Pixels )
		{
			GLCALL( glTexSubImage2D( Target, Level, xOffset, yOffset, Width, Height, Format, Type, Pixels ) );
		}

		static FORCEINLINE void TextureSubImage2D( GLuint Texture,
												   GLint Level,
												   GLint xOffset,
												   GLint yOffset,
												   GLsizei Width,
												   GLsizei Height,
												   GLenum Format,
												   GLenum Type,
												   CMemPtr Pixels )
		{
			GLCALL( glTextureSubImage2D( Texture, Level, xOffset, yOffset, Width, Height, Format, Type, Pixels ) );
		}

		static FORCEINLINE void GetTexImage( GLenum Target,
											 GLint Level,
											 GLenum Format,
											 GLenum Type,
											 MemPtr Pixels )
		{
			GLCALL( glGetTexImage( Target, Level, Format, Type, Pixels ) );
		}

		static FORCEINLINE void GetnTexImage( GLenum Target,
											  GLint Level,
											  GLenum Format,
											  GLenum Type,
											  GLsizei BufSize,
											  MemPtr Pixels )
		{
			GLCALL( glGetnTexImage( Target, Level, Format, Type, BufSize, Pixels ) );
		}

		static FORCEINLINE void GetTextureImage( GLuint Texture,
												 GLint Level,
												 GLenum Format,
												 GLenum Type,
												 GLsizei BufSize,
												 MemPtr Pixels )
		{
			GLCALL( glGetTextureImage( Texture, Level, Format, Type, BufSize, Pixels ) );
		}

		static FORCEINLINE void GetTextureSubImage( GLuint Texture,
													GLint Level,
													GLint xOffset,
													GLint yOffset,
													GLint zOffset,
													GLsizei Width,
													GLsizei Height,
													GLsizei Depth,
													GLenum Format,
													GLenum Type,
													GLsizei BufSize,
													MemPtr Pixels )
		{
			GLCALL( glGetTextureSubImage( Texture, Level, xOffset, yOffset, zOffset, Width, Height, Depth, Format, Type, BufSize, Pixels ) );
		}

		// Frame Buffers

		static FORCEINLINE void GenFramebuffers( GLsizei n, GLuint* framebuffers )
		{
			GLCALL( glGenFramebuffers( n, framebuffers ) );
		}

		static FORCEINLINE void CreateFramebuffers( GLsizei n, GLuint* framebuffers )
		{
			GLCALL( glCreateFramebuffers( n, framebuffers ) );
		}

		static FORCEINLINE void BindFramebuffer( GLenum Target, GLuint Framebuffer )
		{
			GLCALL( glBindFramebuffer( Target, Framebuffer ) );
		}

		static FORCEINLINE void UnbindFramebuffer( GLenum Target )
		{
			GLCALL( glBindFramebuffer( Target, 0 ) );
		}

		static FORCEINLINE GLenum CheckFramebufferStatus( GLenum Target )
		{
			GLenum result;
			GLCALL( result = glCheckFramebufferStatus( Target ) );

			return result;
		}

		static FORCEINLINE GLenum CheckNamedFramebufferStatus( GLuint Framebuffer, GLenum Target )
		{
			GLenum result;
			GLCALL( result = glCheckNamedFramebufferStatus( Framebuffer, Target ) );

			return result;
		}

		static FORCEINLINE void DeleteFramebuffers( GLsizei n, const GLuint* framebuffers )
		{
			GLCALL( glDeleteFramebuffers( n, framebuffers ) );
		}


		static FORCEINLINE void FramebufferTexture( GLenum Target,
													GLenum Attachment,
													GLuint Texture,
													GLint  Level )
		{
			GLCALL( glFramebufferTexture( Target, Attachment, Texture, Level ) );
		}

		static FORCEINLINE void FramebufferTexture1D( GLenum Target,
													  GLenum Attachment,
													  GLenum TextureTarget,
													  GLuint Texture,
													  GLint  Level )
		{
			GLCALL( glFramebufferTexture1D( Target, Attachment, TextureTarget, Texture, Level ) );
		}

		static FORCEINLINE void FramebufferTexture2D( GLenum Target,
													  GLenum Attachment,
													  GLenum TextureTarget,
													  GLuint Texture,
													  GLint  Level )
		{
			GLCALL( glFramebufferTexture2D( Target, Attachment, TextureTarget, Texture, Level ) );
		}

		static FORCEINLINE void FramebufferTexture3D( GLenum Target,
													  GLenum Attachment,
													  GLenum TextureTarget,
													  GLuint Texture,
													  GLint  Level,
													  GLint  Layer )
		{
			GLCALL( glFramebufferTexture3D( Target, Attachment, TextureTarget, Texture, Level, Layer ) );
		}

		static FORCEINLINE void NamedFramebufferTexture( GLuint Framebuffer,
														 GLenum Attachment,
														 GLuint Texture,
														 GLint  Level )
		{
			GLCALL( glNamedFramebufferTexture( Framebuffer, Attachment, Texture, Level ) );
		}

		// Drawing

		static FORCEINLINE void DrawArrays( GLenum Mode, GLint First, GLsizei Count )
		{
			GLCALL( glDrawArrays( Mode, First, Count ) );
		}

		static FORCEINLINE void DrawElements( GLenum Mode, GLsizei Count, GLenum Type, CMemPtr Indices )
		{
			GLCALL( glDrawElements( Mode, Count, Type, Indices ) );
		}


		// Other

		static FORCEINLINE void ObjectLabel( GLenum Identifier, GLuint Name, GLsizei Length, const GLchar* Label )
		{
			glObjectLabel( Identifier, Name, Length, Label );
		}

		static FORCEINLINE void ObjectPtrLabel( MemPtr Ptr, GLsizei Length, const GLchar* Label )
		{
			glObjectPtrLabel( Ptr, Length, Label );
		}


	};

}
