#include "stdafx.h"

#include "DFilePath.h"

namespace DentyEngine
{
	FilePath::FilePath() noexcept :
		_path()
	{
	}

	FilePath::FilePath(const char* path) noexcept :
		_path(path)
	{
	}

	FilePath::FilePath(std::string_view path) noexcept :
		_path(path)
	{
	}

	FilePath::FilePath(const std::string& path) noexcept :
		_path(path)
	{
	}

	FilePath::FilePath(const std::filesystem::path& path) noexcept :
		_path(path)
	{
	}

	FilePath& FilePath::MakePreferred()
	{
		_path.make_preferred();

		return *this;
	}

	FilePath& FilePath::ReplaceFilename(const std::filesystem::path& fileName)
	{
		const std::filesystem::path extension = _path.extension();
		_path.replace_filename(fileName);
		_path.replace_extension(extension);

		return *this;
	}

	FilePath& FilePath::ReplaceExtension(const std::filesystem::path& extension)
	{
		_path.replace_extension(extension);

		return *this;
	}

	FilePath& FilePath::RemoveFileName()
	{
		_path.remove_filename();

		return *this;
	}

	FilePath& FilePath::RemoveExtension()
	{
		_path.replace_extension("");

		return *this;
	}

	FilePath FilePath::ParentPath() const
	{
		return FilePath(_path.parent_path());
	}

	FilePath FilePath::RelativePath() const
	{
		return FilePath(_path.relative_path());
	}

	FilePath FilePath::Extension() const
	{
		return FilePath(_path.extension());
	}

	FilePath FilePath::Filename() const
	{
		return FilePath(_path.filename());
	}

	FilePath FilePath::Stem() const
	{
		return FilePath(_path.stem());
	}

    FilePath FilePath::ObtainDirectory() const
    {
        auto path = FilePath(_path);
		path.RemoveExtension();
		path.RemoveFileName();

		return path;
    }

    void FilePath::RemoveFile() const
	{
		std::filesystem::remove(_path);
	}

	bool FilePath::IsValid() const
	{
		return exists(_path);
	}

	FilePath& FilePath::operator=(const std::filesystem::path& path) noexcept
	{
		_path = path;

		return *this;
	}

	FilePath& FilePath::operator+=(const std::string& other) noexcept
	{
		_path += other;

		return *this;
	}

	FilePath& FilePath::operator+=(const FilePath& other) noexcept
	{
		_path += other._path;

		return *this;
	}

	FilePath& FilePath::operator/=(const FilePath& other)
	{
		_path /= other._path;

		return *this;
	}

	bool FilePath::operator!=(const FilePath& other) const noexcept
	{
		return (this->Get() != other.Get());
	}

	FilePath FilePath::MakePath(const FilePath& directory, std::string_view name, std::string_view extension)
	{
		FilePath path = directory;
		return path.ReplaceFilename(name).ReplaceExtension(extension).MakePreferred();
	}
}
