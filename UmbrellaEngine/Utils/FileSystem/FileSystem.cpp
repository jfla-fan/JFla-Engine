#include "FileSystem.h"



namespace J::system
{
	void File::_open(const FilePath& path, EMode mode)
	{
		Stream.open(path.string(), static_cast<std::ios::openmode>(mode));
	}

	void File::_open(std::string_view path, EMode mode)
	{
		Stream.open(path, static_cast<std::ios::openmode>(mode));
	}

	void File::_open(const CHAR* path, EMode mode)
	{
		Stream.open(path, static_cast<std::ios::openmode>(mode));
	}

	File::File(const FilePath& path, EMode mode)
		: Path(path), Mode(mode)
	{
		_open(path, mode);
	}

	File::File(std::string_view path, EMode mode)
		: Path(path), Mode(mode)
	{
		_open(path, mode);
	}

	File::File(const CHAR* path, EMode mode)
		: Path(path), Mode(mode)
	{
		_open(path, mode);
	}

	File::File(File&& another)
	{
		this->Path		= std::move(another.Path);
		this->Mode		= another.Mode;
		this->Stream	= std::move(another.Stream);
	}

	File& File::operator = (File&& another)
	{
		this->Path		= std::move(another.Path);
		this->Mode		= another.Mode;
		this->Stream	= std::move(another.Stream);
	
		return *this;
	}

	File::~File()
	{
	}

	bool File::IsOpen() const { return Stream.is_open(); }

	bool File::IsGood() const { return Stream.good(); }

	void File::Open(const FilePath& path, EMode mode)
	{
		_open(path, mode);
		Path = path;
		Mode = mode;
	}

	void File::Open(std::string_view path, EMode mode)
	{
		_open(path, mode);
		Path = path;
		Mode = mode;
	}

	void File::Open(const CHAR* path, EMode mode)
	{
		_open(path, mode);
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

	std::string File::ReadAllText()
	{
		std::string content;

		if (!IsOpen() || (this->Mode & EMode::READ) == 0)
		{
			// todo: normal error log system
			throw "File should be opened in read mode!";
		}

		content.assign(std::istreambuf_iterator<CHAR>(this->Stream), std::istreambuf_iterator<CHAR>());
		return content;
	}

	void File::ReadBytes(MemPtr Data, SIZE_T Size)
	{
		Stream.read((CHAR*)Data, Size);
	}

	void File::WriteBytes(CMemPtr Data, SIZE_T Size)
	{
		Stream.write((const CHAR*)Data, Size);
	}

	// static functions

	bool File::Exists(const FilePath& path)
	{
		return std::filesystem::exists(path);
	}

	bool File::Exists(std::string_view path)
	{
		return std::filesystem::exists(path);
	}

	bool File::Exists(const CHAR* path)
	{
		return std::filesystem::exists(path);
	}

	bool File::IsFile(const FilePath& path)
	{
		return std::filesystem::is_regular_file(path);
	}

	bool File::IsFile(std::string_view path)
	{
		return std::filesystem::is_regular_file(path);
	}

	bool File::IsFile(const CHAR* path)
	{
		return std::filesystem::is_regular_file(path);
	}

	std::string File::ReadAllText(const FilePath& path)
	{
		return File(path).ReadAllText();
	}

	std::string File::ReadAllText(std::string_view path)
	{
		return File(path).ReadAllText();
	}

	std::string File::ReadAllText(const CHAR* path)
	{
		return File(path).ReadAllText();
	}


	bool IsDirectory(const FilePath& path)
	{
		return std::filesystem::is_directory(path);
	}

	bool IsDirectory(std::string_view path)
	{
		return std::filesystem::is_directory(path);
	}

	bool IsDirectory(const CHAR* path)
	{
		return std::filesystem::is_directory(path);
	}

	bool CreateDirectory(const FilePath& path)
	{
		return std::filesystem::create_directory(path);
	}

	bool CreateDirectory(std::string_view path)
	{
		return std::filesystem::create_directory(path);
	}

	bool CreateDirectory(const CHAR* path)
	{
		return std::filesystem::create_directory(path);
	}

}
