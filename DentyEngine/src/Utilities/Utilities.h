#pragma once

#pragma warning(disable: 6387)

namespace DentyEngine
{
    //
    // Alias
    // 
    template <class T>
    using Ref = std::shared_ptr<T>;

    template <class T>
    using Scope = std::unique_ptr<T>;

    template <class T>
    using ComRef = Microsoft::WRL::ComPtr<T>;

    namespace Utils
    {
        template <class Type, class ...Args>
        Ref<Type> CreateRef(Args&&... args)
        {
            return std::make_shared<Type>(std::forward<Args>(args)...);
        }

        template <class Type, class ...Args>
        Scope<Type> CreateUnique(Args&&... args)
        {
            return std::make_unique<Type>(std::forward<Args>(args)...);
        }
        
        inline LPSTR HRTrace(HRESULT hr)
        {
            LPSTR msg = { };
            FormatMessageA(
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS |
                FORMAT_MESSAGE_ALLOCATE_BUFFER,
                nullptr, hr,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                msg, 0, nullptr
            );
            return msg;
        }
        extern void ShowErrorMessageBox();

        template <class T>
        static bool EraseFromVectorContainer(std::vector<T>& source, const T& target)
        {
            const auto result = std::ranges::find(source, target);

            if (result != source.end())
            {
                source.erase(result);

                return true;
            }

            return false;
        }

        static bool FileExists(const std::filesystem::path& path)
        {
            return exists(path);
        }

        static bool IsNeedSerialize(const std::filesystem::path& path, std::string_view extension)
        {
            std::filesystem::path extensionPath = path;
            extensionPath = extensionPath.replace_extension(extension);

            return (not FileExists(extensionPath));
        }
    }
}

//
// Class macros
//
#define DEFINE_SINGLETON_CLASS(Type) \
	~Type() = default; \
	Type(const Type&) = delete; \
	Type(const Type&&) noexcept = delete; \
	Type& operator=(const Type&) = delete; \
	Type& operator=(const Type&&) noexcept = delete; \
	static Type& GetInstance() \
	{\
		static Type instance; \
		return instance; \
	} \
	private: \
	Type() = default; \
	public:\

#define DELETE_COPY_MOVE_CONSTRUCTORS(Type) \
	Type(const Type&) = delete; \
	Type(const Type&&) noexcept = delete; \
	Type& operator=(const Type&) = delete; \
	Type& operator=(const Type&&) noexcept = delete;

#define DENTY_NOT_IMPLEMENTED_FUNCTION(functionName) DENTY_ASSERT(false, std::string(std::string(functionName) + " not implemented yet!").c_str());

#define DENTY_CHECK_IS_OUT_OF_RANGE_STL(index, objects, functionName) \
	do \
	{ \
		try \
		{ \
			std::ignore = (objects).at(index); \
		} \
		catch ([[maybe_unused]] const std::out_of_range& e) \
 		{ \
			DENTY_ASSERT(false, String(std::string(e.what()) + " (in " + std::string(functionName) + ")").CStr()); \
		} \
	} while (false); 

#define DENTY_RETURN_VALUE_IF_OPTIONAL_HAS(optionalValue, type) [&]() -> type \
    { \
        if ((optionalValue).has_value()) \
        { \
            return (optionalValue).value(); \
        } \
        DENTY_ERR_CONSOLE_LOG("Value not found"); \
        DENTY_SET_ERR_COLOR \
        DENTY_ERR_LOG << "Value not found"; \
        return type(); \
    }() 

#define DENTY_CHECK_STL_SIZE(stl, assumedSize) do { if ((stl).size() != (assumedSize)) { assert("Incorrect size!"); } } while (false); 
#define DENTY_CHECK_STL_MAP_VALUE_EXISTS(map, value) do { if ((map).find(value) == (map).end()) { assert("Value doesn't exists!"); }} while (false);
#define DENTY_CHECK_CONDITION(expression) do { if (not (expression)) { assert("The formula was not met!"); } } while (false);
#define DENTY_CHECK_NULL(object) do { if (not (object)) { assert("Object is null!"); } } while (false);

    //
    // Bit macros.
    //
#define BIT(x) (1 << (x))
#define REVERSE_ALL_BIT(x) (~(x))
#define BIT_OFF_AT(data, bit) (((data) & ~(bit)))

#define DEFINE_ENUM_CLASS_BIT_OPERATORS(T) \
	static constexpr T operator|(T lhs, T rhs) \
	{ \
		return static_cast<T>((static_cast<std::underlying_type<T>::type>(lhs) | static_cast<std::underlying_type<T>::type>(rhs))); \
	} \
	static constexpr T operator&(T lhs, T rhs) \
	{ \
		return static_cast<T>((static_cast<std::underlying_type<T>::type>(lhs) & static_cast<std::underlying_type<T>::type>(rhs))); \
	} \

    //
    // Utility macros.
    //
#define DENTY_FILE_PATH(filePath) FilePath(filePath).MakePreferred().GetAsString()
#define DENTY_FILE_PATH_REPLACE_EXTENSION(filePath, extension) FilePath(filePath).MakePreferred().ReplaceExtension(extension).GetAsString()
#define DENTY_REPLACE_FILENAME(filePath, newName) [&]() -> void { (filePath) = FilePath(filePath) \
		.ReplaceFilename(newName) \
		.ReplaceExtension(FilePath(filePath).Extension()).GetAsString(); }() \

#define DENTY_GET_FILENAME_FROM_FILE_PATH(filePath) FilePath(filePath).Filename().ReplaceExtension("").GetAsString()
#define DENTY_GET_FILE_PATH_WITHOUT_FILENAME(filePath) FilePath(filePath).ReplaceFilename("").ReplaceExtension("").GetAsString()

#define DENTY_SAFE_DELETE(x) if (x) { delete (x); } \
		(x) = nullptr

#define DENTY_SAFE_DELETE_ARRAY(x) if (x) { delete[] (x); } \
        (x) = nullptr

#ifdef _MSC_VER
  #define DENTY_NOVTABLE __declspec(novtable)
#else
  #define DENTY_NOVTABLE
#endif

#pragma warning(default: 6387)
