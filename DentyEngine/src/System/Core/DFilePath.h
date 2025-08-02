#pragma once

namespace DentyEngine
{
	// Wrapper class for std::filesystem::path
	class FilePath final
	{
	public:
		struct Hash
		{
			std::size_t operator()(const FilePath& filePath) const
			{
				const std::size_t h1 = std::hash<std::filesystem::path>()(filePath._path);

				return h1;
			}
		};
	public:
		//
		// Constructor
		//
		FilePath() noexcept;
		FilePath(const char*) noexcept;
		FilePath(std::string_view path) noexcept;
		FilePath(const std::string&) noexcept;
		explicit FilePath(const std::filesystem::path&) noexcept;
	public:
		//
		// Member functions.
		//
		

		FilePath& MakePreferred();

		FilePath& ReplaceFilename(const std::filesystem::path&);
		FilePath& ReplaceExtension(const std::filesystem::path&);

		FilePath& RemoveFileName();
		FilePath& RemoveExtension();

		[[nodiscard]]
		FilePath ParentPath() const;

		[[nodiscard]]
		FilePath RelativePath() const;

		[[nodiscard]]
		FilePath Extension() const;

		[[nodiscard]]
		FilePath Filename() const;

		[[nodiscard]]
		FilePath Stem() const;

		// Example : Assets/Textures/hogehoge.Json -> Assets/Textures/
		[[nodiscard]]
		FilePath ObtainDirectory() const;

		[[nodiscard]]
		void* GetData() const { DENTY_NOT_IMPLEMENTED_FUNCTION(__func__);  return nullptr; }

		[[nodiscard]]
		std::string GetAsString() const { return _path.string(); }

		[[nodiscard]]
		const char* GetCStr() const noexcept { return _path.string().c_str(); }

		[[nodiscard]]
		const std::filesystem::path& Get() const { return _path; }

		// After call this function, IsValid should be return false if succeed.
		void RemoveFile() const;

		[[nodiscard]]
		bool HasFileName() const noexcept { return _path.has_filename(); }

		[[nodiscard]]
		bool HasStem() const noexcept { return _path.has_filename(); }

		[[nodiscard]]
		bool HasParentPath() const noexcept { return _path.has_parent_path(); }

		[[nodiscard]]
		bool HasRelativePath() const noexcept { return _path.has_relative_path(); }

		[[nodiscard]]
		bool IsAbsolute() const noexcept { return _path.is_absolute(); }

		[[nodiscard]]
		bool IsValid() const;

		[[nodiscard]]
		bool IsEmpty() const noexcept { return _path.string().empty(); }

		//
		// operators.
		//
		FilePath& operator=(const std::filesystem::path&) noexcept;

		FilePath& operator+=(const std::string& other) noexcept;
		FilePath& operator+=(const FilePath& other) noexcept;

		FilePath& operator/=(const FilePath& other);

		bool operator!=(const FilePath& other) const noexcept;

		friend bool operator==(const FilePath& lhs, const FilePath& rhs) noexcept;
		friend std::ostream& operator<<(std::ostream& os, const FilePath& r);
	public:
		[[nodiscard]]
		static FilePath MakePath(const FilePath& directory, std::string_view name, std::string_view extension);
	private:
		std::filesystem::path _path;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] uint32_t version) const
		{
			const std::string& path = _path.string();

			archive(
				CEREAL_NVP(path)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] uint32_t version)
		{
			std::string path = {};

			archive(
				CEREAL_NVP(path)
			);

			_path = path;
		}
	};

	inline bool operator==(const FilePath& lhs, const FilePath& rhs) noexcept
	{
		return (lhs._path == rhs._path);
	}

	inline std::ostream& operator<<(std::ostream& os, const FilePath& r)
	{
		os << r.Get();

		return os;
	}
}

CEREAL_CLASS_VERSION(DentyEngine::FilePath, 1u)