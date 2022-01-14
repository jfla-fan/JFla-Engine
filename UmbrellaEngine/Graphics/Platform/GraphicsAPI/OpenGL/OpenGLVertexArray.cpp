#include "OpenGLVertexArray.h"



namespace J::Graphics
{
	using namespace GpuApi;

	// vertex attributes
	
	template<> SVertexAttribute SVertexAttribute::GetAttribute< float >( bool disable )
	{
		return { GL_FLOAT, false, disable, 1, 1, sizeof(float) };
	}

	template<> SVertexAttribute SVertexAttribute::GetAttribute< int >( bool disable )
	{
		return { GL_FLOAT, false, disable, 1, 1, sizeof(int) };
	}

	template<> SVertexAttribute SVertexAttribute::GetAttribute< double >( bool disable )
	{
		return { GL_FLOAT, false, disable, 1, 1, sizeof( double ) };
	}

	template<> SVertexAttribute SVertexAttribute::GetAttribute< Vector1 >( bool disable )
	{
		return { GL_FLOAT, false, disable, 1, 1, sizeof( Vector1 ) };
	}

	template<> SVertexAttribute SVertexAttribute::GetAttribute< Vector2 >( bool disable )
	{
		return { GL_FLOAT, false, disable, 2, 1, sizeof( Vector2 ) };
	}

	template<> SVertexAttribute SVertexAttribute::GetAttribute< Vector3 >( bool disable )
	{
		return { GL_FLOAT, false, disable, 3, 1, sizeof( Vector3 ) };
	}

	template<> SVertexAttribute SVertexAttribute::GetAttribute< Vector4 >( bool disable )
	{
		return { GL_FLOAT, false, disable, 4, 1, sizeof( Vector4 ) };
	}

	template<> SVertexAttribute SVertexAttribute::GetAttribute< VectorInt1 >( bool disable )
	{
		return { GL_FLOAT, false, disable, 1, 1, sizeof( VectorInt1 ) };
	}

	template<> SVertexAttribute SVertexAttribute::GetAttribute< VectorInt2 >( bool disable )
	{
		return { GL_FLOAT, false, disable, 2, 1, sizeof( VectorInt2 ) };
	}

	template<> SVertexAttribute SVertexAttribute::GetAttribute< VectorInt3 >( bool disable )
	{
		return { GL_FLOAT, false, disable, 3, 1, sizeof( VectorInt3 ) };
	}

	template<> SVertexAttribute SVertexAttribute::GetAttribute< VectorInt4 >( bool disable )
	{
		return { GL_FLOAT, false, disable, 4, 1, sizeof( VectorInt4 ) };
	}

	// todo: implement other attributes later


	// Vertex Array

	OpenGLVertexArray::IdType OpenGLVertexArray::CurrentlyBoundVertexArray = OpenGLVertexArray::InvalidVertexArrayId;


	void OpenGLVertexArray::Release()
	{
		if (this->Resource != OpenGLVertexArray::InvalidVertexArrayId)
		{
			if (this->Resource == OpenGLVertexArray::CurrentlyBoundVertexArray)
			{
				OpenGLVertexArray::CurrentlyBoundVertexArray = InvalidVertexArrayId;
			}

			OpenGLContext::DeleteVertexArrays(1, &this->Resource);
		}

		this->Resource = OpenGLVertexArray::InvalidVertexArrayId;
	}

	OpenGLVertexArray::OpenGLVertexArray()
		: IndexBuffer(nullptr), AttributeIndex(0)
	{
		OpenGLContext::GenVertexArrays(1, &Resource);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		Release();
	}


	OpenGLVertexArray::OpenGLVertexArray(OpenGLVertexArray&& another) NOEXCEPT
	{
		Release();

		this->Resource			= another.Resource;
		//this->VertexBuffers		= std::move(another.VertexBuffers);
		this->IndexBuffer		= std::move(another.IndexBuffer);
		this->AttributeIndex	= another.AttributeIndex;

		another.Resource		= OpenGLVertexArray::InvalidVertexArrayId;
		another.AttributeIndex	= 0;
	}

	OpenGLVertexArray& OpenGLVertexArray::operator = (OpenGLVertexArray&& another) NOEXCEPT
	{
		Release();

		this->Resource			= another.Resource;
		//this->VertexBuffers		= std::move(another.VertexBuffers);
		this->IndexBuffer		= std::move(another.IndexBuffer);
		this->AttributeIndex	= another.AttributeIndex;

		another.Resource		= OpenGLVertexArray::InvalidVertexArrayId;
		another.AttributeIndex	= 0;

		return *this;
	}

	void OpenGLVertexArray::Bind() const
	{
		OpenGLContext::BindVertexArray(this->Resource);
		OpenGLVertexArray::CurrentlyBoundVertexArray = this->Resource;
	}

	void OpenGLVertexArray::Unbind() const
	{
		OpenGLContext::BindVertexArray(OpenGLVertexArray::InvalidVertexArrayId);
		OpenGLVertexArray::CurrentlyBoundVertexArray = OpenGLVertexArray::InvalidVertexArrayId;
	}


	void OpenGLVertexArray::SetIndexBuffer(Ref<OpenGLIndexBuffer> indexBuffer)
	{
		this->Bind();
		indexBuffer->Bind();
		this->Unbind();

		this->IndexBuffer = indexBuffer;
	}
}
