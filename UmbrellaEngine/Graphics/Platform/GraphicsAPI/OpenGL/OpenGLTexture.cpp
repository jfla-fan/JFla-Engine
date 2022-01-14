#include "OpenGLTexture.h"
#include "../../../../Image/ImageLoader.h"
#include "../GpuApi.h"



namespace J::Graphics
{
	using namespace Utils;

	using GpuApi::ETextureFormat;
	using GpuApi::STextureSizeInfo;


	static_assert((SIZE_T)ETextureFormat::AUTO - (SIZE_T)ETextureFormat::R >= 5, "Add texture formats.");


	static GLint WrapModeToGLEnumTable[] =
	{
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_BORDER,
		GL_MIRRORED_REPEAT,
		GL_REPEAT,
		GL_MIRROR_CLAMP_TO_EDGE
	};


	static GLenum Map(ETextureFormat format)
	{
#define CASE_LABEL(format_type)\
case ETextureFormat::format_type:\
		return GL_##format_type

#define CUSTOM_CASE_LABEL(format_type, gl_type)\
		case ETextureFormat::format_type:\
			return gl_type
		
		switch (format)
		{
			CUSTOM_CASE_LABEL(R, GL_RED);
			CASE_LABEL(R8);
			CASE_LABEL(R16);
			CASE_LABEL(R16F);
			CASE_LABEL(R32F);

			CASE_LABEL(RG);
			CASE_LABEL(RG8);
			CASE_LABEL(RG16);
			CASE_LABEL(RG16F);
			CASE_LABEL(RG32F);

			CASE_LABEL(RGB);
			CASE_LABEL(RGB8);
			CASE_LABEL(RGB16);
			CASE_LABEL(RGB16F);

			CASE_LABEL(RGBA);
			CASE_LABEL(RGBA8);
			CASE_LABEL(RGBA16F);

			CUSTOM_CASE_LABEL(AUTO, GL_RGB);

			CUSTOM_CASE_LABEL(DEPTH, GL_DEPTH_COMPONENT);

		default:
			{
				// #todo WARN!
				return GL_RGB;
			}
		}

#undef CASE_LABEL
#undef CUSTOM_CASE_LABEL

	}


	OpenGLTexture::OpenGLTexture()
		: Resource(OpenGLContext::GInvalidGLResource)
		, bLoaded(false)
		, bHasMipChain(false)
		, bInitialized(false)
		, Format()
		, ResourcePath("")
	{
		AllocateResource();
	}

	OpenGLTexture::OpenGLTexture(const system::FilePath& path, bool genMipChain)
	{
		AllocateResource();

		this->Load(path);

		if (genMipChain)
		{
			this->GenerateMipmapChain();
		}
	}

	OpenGLTexture::~OpenGLTexture()
	{
		this->Release();
	}

	bool OpenGLTexture::AllocateResource()
	{
		OpenGLContext::GenTextures(1, &Resource);

		return true;
	}

	void OpenGLTexture::Release() const
	{
		if (IsValid())
		{
			OpenGLContext::DeleteTextures(1, &Resource);
			bHasMipChain = false;
			bInitialized = false;
			bLoaded = false;
		}
	}

	void OpenGLTexture::Bind() const
	{
		OpenGLContext::ActiveTexture(GL_TEXTURE0 + Active);
		OpenGLContext::BindTexture(TextureType, Resource);
	}

	void OpenGLTexture::Bind(uint32 id) const
	{
		OpenGLContext::ActiveTexture(GL_TEXTURE0 + id);
		this->Active = id;
		OpenGLContext::BindTexture(TextureType, Resource);
	}

	void OpenGLTexture::Unbind() const
	{
		OpenGLContext::ActiveTexture(TextureType);
		OpenGLContext::BindTexture(TextureType, OpenGLContext::GInvalidGLResource);
	}

	void OpenGLTexture::GenerateMipmapChain() const
	{
		if (IsValid())
		{
			this->Bind(0);

			// #todo create methods for setting those?
			GLCALL(glTexParameteri(TextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
			GLCALL(glTexParameteri(TextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			
			OpenGLContext::GenerateMipmap(TextureType);
			bHasMipChain = true;
		}
	}

	bool OpenGLTexture::Load(const system::FilePath& path, ETextureFormat format /* = ETextureFormat::AUTO */)
	{
		auto img = ImageLoader::Load(path, true);
	
		if (!img || !img->IsInitialized())
		{
			// #todo log it
			return false;
		}

		GLenum pixelType = GL_UNSIGNED_BYTE;
		GLenum internalType = Map(format);
		GLenum dataFormat = GL_RGB;

		switch (img->GetChannelsCount())
		{
			case 1:
			{
				dataFormat = GL_RED;
				break;
			}

			case 2:
			{
				dataFormat = GL_RG;
				break;
			}
			
			case 3:
			{
				dataFormat = GL_RGB;
				break;
			}

			case 4:
			{
				dataFormat = GL_RGBA;
				break;
			}
			
			default:
			{
				// #todo LOG it
				return false;
			}

		}
		
		// opengl context :: ...
		OpenGLContext::BindTexture(TextureType, Resource);
		glTexImage2D(TextureType, 0, internalType, img->GetWidth(), img->GetHeight(), 0, dataFormat, pixelType, img->RawData());
		
		ResourcePath = path;

		SizeInfo.Width = img->GetWidth();
		SizeInfo.Height = img->GetHeight();
		SizeInfo.Depth = 0;
		SizeInfo.ChannelsCount = img->GetChannelsCount();
		
		bInitialized = true;
		bLoaded = true;

		this->GenerateMipmapChain();

		return true;
	}

	bool OpenGLTexture::Load(CMemPtr data, uint32 width, uint32 height, uint32 channels, ETextureFormat format /* = ETextureFormat::AUTO */)
	{
		GLenum pixelType = GL_UNSIGNED_BYTE;
		GLenum internalType = Map(format);
		GLenum dataFormat = GL_RGB;

		switch (channels)
		{
		case 1:
		{
			dataFormat = GL_RED;
			break;
		}

		case 2:
		{
			dataFormat = GL_RG;
			break;
		}

		case 3:
		{
			dataFormat = GL_RGB;
			break;
		}

		case 4:
		{
			dataFormat = GL_RGBA;
			break;
		}

		default:
		{
			// #todo LOG it
			return false;
		}

		}

		// opengl context :: ...
		OpenGLContext::BindTexture(TextureType, Resource);
		glTexImage2D(TextureType, 0, internalType, width, height, 0, dataFormat, pixelType, data);

		SizeInfo.Width = width;
		SizeInfo.Height = height;
		SizeInfo.Depth = 0;
		SizeInfo.ChannelsCount = channels;

		bInitialized = true;
		bLoaded = true;

		this->GenerateMipmapChain();

		return true;
	}

	bool OpenGLTexture::Load(const Image& image, ETextureFormat format /* = ETextureFormat::AUTO */)
	{
		if (!image.IsInitialized())
		{
			// #todo WARN
			return false;
		}

		return this->Load(image.RawData(), image.GetWidth(), image.GetHeight(), image.GetChannelsCount(), format);
	}

	void OpenGLTexture::SetMinLOD(float lod)
	{
		this->Bind(0);	// #TODO is this right? Make it out
		GLCALL(glTexParameterf(TextureType, GL_TEXTURE_MIN_LOD, lod));
	}

	void OpenGLTexture::SetMaxLOD(float lod)
	{
		this->Bind(0);
		GLCALL(glTexParameterf(TextureType, GL_TEXTURE_MAX_LOD, lod));
	}

	void OpenGLTexture::SetBorderColor(Vector4 color)
	{
		this->Bind(0);
		auto normalized = Clamp(color, MakeVector4(), MakeVector4(1.0f));
		GLCALL(glTexParameterfv(TextureType, GL_TEXTURE_BORDER_COLOR, &normalized[0]));
	}

	void OpenGLTexture::SetWrappingMode(ETextureWrappingMode mode, ETextureWrappingAxis axices)
	{
		OpenGLContext::BindTexture(TextureType, OpenGLContext::GInvalidGLResource);

		if (axices & ETextureWrappingAxis::S)
		{
			GLCALL(glTexParameteri(TextureType, GL_TEXTURE_WRAP_S, WrapModeToGLEnumTable[(uint32)mode]));
		}

		if (axices & ETextureWrappingAxis::T)
		{
			GLCALL(glTexParameteri(TextureType, GL_TEXTURE_WRAP_T, WrapModeToGLEnumTable[(uint32)mode]));
		}

		if (axices & ETextureWrappingAxis::R)
		{
			GLCALL(glTexParameteri(TextureType, GL_TEXTURE_WRAP_R, WrapModeToGLEnumTable[(uint32)mode]));
		}

		this->Unbind();
	}

	float OpenGLTexture::GetMaxLOD() const
	{
		return Math::Log2(Math::Max(SizeInfo.Width, SizeInfo.Height));
	}

	void OpenGLTexture::SetBiasLOD(float bias)
	{
		this->Bind(0);
		GLCALL(glTexParameterf(TextureType, GL_TEXTURE_LOD_BIAS, (float)bias));
	}

	ImagePtr OpenGLTexture::GetImage() const
	{
		if (!this->bInitialized || !this->bLoaded)
		{
			return nullptr;
		}

		ERawImageFormat format = ERawImageFormat::AUTO;

		switch (Format)
		{
			case ETextureFormat::R:
			case ETextureFormat::R8:
			{
				format = ERawImageFormat::R8;
				break;
			}

			case ETextureFormat::RG:
			case ETextureFormat::RG8:
			{
				format = ERawImageFormat::RG8;
				break;
			}

			case ETextureFormat::RGB:
			{
				format = ERawImageFormat::RGB8;
				break;
			}

			case ETextureFormat::RGBA:
			case ETextureFormat::RGBA8:
			{
				format = ERawImageFormat::RGBA8;
				break;
			}

			case ETextureFormat::DEPTH:
			{
				format = ERawImageFormat::L8;
				break;
			}

			default:
			{
				// #todo LOG
				format = ERawImageFormat::AUTO;
				break;
			}

		}

		auto img = Image( this->SizeInfo.Width, this->SizeInfo.Height, format );

		if ( !img.IsInitialized() )
		{
			return nullptr;
		}

		this->Bind();

		GLCALL( glGetTexImage( TextureType, 0, Map( Format ), GL_UNSIGNED_BYTE, img.RawData() ) );

		img.MarkInitialized();

		ImagePtr ptr = MakeScoped< Image >( std::move( img ) );

		return ptr;
	}



	bool OpenGLTexture::IsLoaded() const
	{
		return bLoaded;
	}

	bool OpenGLTexture::IsInitialized() const
	{
		return bInitialized;
	}

	bool OpenGLTexture::IsMultisampled() const
	{
		return TextureType == GL_TEXTURE_2D_MULTISAMPLE;
	}

	bool OpenGLTexture::IsFloatingPoint() const
	{
		switch (Format)
		{
			case ETextureFormat::R16F:
			case ETextureFormat::R32F:
			case ETextureFormat::RG16F:
			case ETextureFormat::RGB16F:
			case ETextureFormat::RGBA16F:
				return true;

			default:
				return false;
		}
	}

	bool OpenGLTexture::IsDepthOnly() const
	{
		return Format == ETextureFormat::DEPTH;
	}

	Vector4	OpenGLTexture::GetBorderColor() const
	{
		this->Bind(0);
		Vector4 color;

		GLCALL(glGetTexParameterfv(TextureType, GL_TEXTURE_BORDER_COLOR, &color[0]));

		return color;
	}

	uint32 OpenGLTexture::GetPixelSize() const
	{
		switch (Format)
		{
			case ETextureFormat::R:
			case ETextureFormat::R8:
			case ETextureFormat::DEPTH:
				return 1;

			case ETextureFormat::R16:
			case ETextureFormat::R16F:
			case ETextureFormat::RG:
			case ETextureFormat::RG8:
				return 2;
			
			case ETextureFormat::RGB:
			case ETextureFormat::RGB8:
				return 3;

			case ETextureFormat::RG16:
			case ETextureFormat::RG16F:
			case ETextureFormat::R32F:
			case ETextureFormat::RGBA:
			case ETextureFormat::RGBA8:
				return 4;

			case ETextureFormat::RGB16:
			case ETextureFormat::RGB16F:
				return 6;

			case ETextureFormat::RG32F:
			case ETextureFormat::RGBA16F:
				return 8;
			
			default:
				// #todo LOG THIS
				return 0;
		}
	}

	ETextureWrappingMode OpenGLTexture::GetWrapMode(ETextureWrappingAxis axis) const
	{
		GLint mode = GL_REPEAT;
		
		if (axis & ETextureWrappingAxis::S)
		{
			GLCALL(glGetTexParameteriv(TextureType, GL_TEXTURE_WRAP_S, &mode));
		}
		else if (axis & ETextureWrappingAxis::T)
		{
			GLCALL(glGetTexParameteriv(TextureType, GL_TEXTURE_WRAP_T, &mode));
		}
		else if (axis & ETextureWrappingAxis::R)
		{
			GLCALL(glGetTexParameteriv(TextureType, GL_TEXTURE_WRAP_R, &mode));
		}
		else
		{
			// #todo LOG ERROR
		}
		
#define CASE_LABEL(format_type)\
		case GL_##format_type:\
			return ETextureWrappingMode::format_type
		
		switch (mode)
		{
			CASE_LABEL(CLAMP_TO_EDGE);
			CASE_LABEL(CLAMP_TO_BORDER);
			CASE_LABEL(MIRRORED_REPEAT);
			CASE_LABEL(REPEAT);
			CASE_LABEL(MIRROR_CLAMP_TO_EDGE);
		
		default:
			// #todo LOG_ERROR
			break;
		}

		return ETextureWrappingMode::REPEAT;	// default open GL mode, should never reach this

#undef CASE_LABEL
	}

	STextureSizeInfo OpenGLTexture::GetSizeInfo() const
	{
		return SizeInfo;
	}

	system::FilePath OpenGLTexture::GetFilePath() const
	{
		return ResourcePath;
	}

	float OpenGLTexture::GetBiasLOD() const
	{
		this->Bind(0);
		float bias = 0.0f;

		GLCALL(glGetTexParameterfv(TextureType, GL_TEXTURE_LOD_BIAS, &bias));

		return bias;
	}
}