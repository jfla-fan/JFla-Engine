#include "FileSystem.h"



namespace UE::system
{
	void File::_open( const FilePath& path, EMode mode )
	{
		Stream.open( path.string(), static_cast< std::ios::openmode >( mode ) );
	}

	void File::_open( StringView path, EMode mode )
	{
		Stream.open( path, static_cast< std::ios::openmode >( mode ) );
	}

	void File::_open( const CHAR* path, EMode mode )
	{
		Stream.open( path, static_cast< std::ios::openmode >( mode ) );
	}

	File::File( const FilePath& path, EMode mode )
		: Path( path ), Mode( mode )
	{
		_open( path, mode );
	}

	File::File( StringView path, EMode mode )
		: Path( path ), Mode( mode )
	{
		_open( path, mode );
	}

	File::File( const CHAR* path, EMode mode )
		: Path( path ), Mode( mode )
	{
		_open( path, mode );
	}

	File::File( File&& another ) NOEXCEPT
	{
		this->Path		= std::move( another.Path );
		this->Mode		= another.Mode;
		this->Stream	= std::move( another.Stream );
	}

	File& File::operator = ( File&& another ) NOEXCEPT
	{
		this->Path		= std::move( another.Path );
		this->Mode		= another.Mode;
		this->Stream	= std::move( another.Stream );
	
		return *this;
	}

	File::~File()
	{
	}

	bool File::IsOpen() const { return Stream.is_open(); }

	bool File::IsGood() const { return Stream.good(); }

	void File::Open( const FilePath& path, EMode mode )
	{
		_open( path, mode );
		Path = path;
		Mode = mode;
	}

	void File::Open( StringView path, EMode mode )
	{
		_open( path, mode );
		Path = path;
		Mode = mode;
	}

	void File::Open( const CHAR* path, EMode mode )
	{
		_open( path, mode );
		Path = path;
		Mode = mode;
	}

	void File::Close()
	{
		Stream.close();
	}

	FileStream& File::GetStream()
	{
		return Stream;
	}

	FilePath File::GetPath() const
	{
		return Path;
	}

	String File::ReadAllText()
	{
		String content;

		if ( !IsOpen() || !( this->Mode & EMode::READ ) )
		{
			// todo: normal error log system
			throw "File should be opened in read mode!";
		}

		content.assign( std::istreambuf_iterator< CHAR >( this->Stream ), std::istreambuf_iterator< CHAR >() );
		return content;
	}

	void File::ReadBytes( MemPtr Data, SIZE_T Size )
	{
		Stream.read( ( CHAR* )Data, Size );
	}

	void File::WriteBytes( CMemPtr Data, SIZE_T Size )
	{
		Stream.write( ( const CHAR* )Data, Size );
	}

	// static functions

	bool File::Exists( const FilePath& path )
	{
		return std::filesystem::exists( path );
	}

	bool File::Exists( StringView path )
	{
		return std::filesystem::exists( path );
	}

	bool File::Exists( const CHAR* path )
	{
		return std::filesystem::exists( path );
	}

	bool File::IsFile( const FilePath& path )
	{
		return std::filesystem::is_regular_file( path );
	}

	bool File::IsFile( StringView path )
	{
		return std::filesystem::is_regular_file( path );
	}

	bool File::IsFile( const CHAR* path )
	{
		return std::filesystem::is_regular_file( path );
	}

	String File::ReadAllText( const FilePath& path )
	{
		return File( path ).ReadAllText();
	}

	String File::ReadAllText( StringView path )
	{
		return File( path ).ReadAllText();
	}

	String File::ReadAllText( const CHAR* path )
	{
		return File( path ).ReadAllText();
	}


	bool IsDirectory( const FilePath& path )
	{
		return std::filesystem::is_directory( path );
	}

	bool IsDirectory( StringView path )
	{
		return std::filesystem::is_directory( path );
	}

	bool IsDirectory( const CHAR* path )
	{
		return std::filesystem::is_directory( path );
	}

	bool CreateDirectory( const FilePath& path )
	{
		return std::filesystem::create_directory( path );
	}

	bool CreateDirectory( StringView path )
	{
		return std::filesystem::create_directory( path );
	}

	bool CreateDirectory( const CHAR* path )
	{
		return std::filesystem::create_directory( path );
	}

}
