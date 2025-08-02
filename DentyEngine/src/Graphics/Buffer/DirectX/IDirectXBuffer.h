#pragma once

namespace DentyEngine
{
	class DENTY_NOVTABLE IDirectXBuffer
	{
	public:
		enum class CpuAccessFlag
		{
			NoAccessWriteAndRead = 0x00L,
			AccessWrite = 0x10000L,
			AccessRead = 0x20000L
		};

		enum class Usage
		{
			Default = 0,
			Immutable,
			Dynamic,
			Staging,

			Max
		};

		enum class MapType
		{
			None = 0,
			Read = 1,
			Write = 2,
			ReadWrite = 3,
			WriteDiscard = 4,
			WriteNoOverwrite = 5,

			Max
		};
	public:
		IDirectXBuffer() = default;
		virtual ~IDirectXBuffer() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(IDirectXBuffer)

		// Map/UnMap

		// Return memory data.
		virtual void* Map() = 0;
		virtual void UnMap() const = 0;

		virtual void Bind() const = 0;
		virtual void Clone(const Ref<IDirectXBuffer>& buffer) const = 0;
	};
}