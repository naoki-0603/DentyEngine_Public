#pragma once

#include "IDirectXBuffer.h"

namespace DentyEngine
{
	class DXIndexBuffer final : public IDirectXBuffer
	{
	public:
		struct IndexBufferInfo
		{
		public:
			IndexBufferInfo() : buffer(),  offset(0u), size(0u), count(0u), data(nullptr), accessFlag(), usage(), mapType() {}

			IndexBufferInfo& operator=(const IndexBufferInfo& source)
			{
				offset = source.offset;
				size = source.size;
				count = source.count;

				data = source.data;

				accessFlag = source.accessFlag;
				usage = source.usage;
				mapType = source.mapType;

				return *this;
			}

			void AssignData(const void* sourceData)
			{
				const auto& indices = std::static_pointer_cast<uint32_t[]>(data);
				const auto& sourceIndices = static_cast<const uint32_t*>(sourceData);

				for (uint32_t index = 0u; index < count; ++index)
				{
					indices[index] = sourceIndices[index];
				}
			}

			ComRef<ID3D11Buffer> buffer;

			uint32_t offset;
			uint32_t size;
			uint32_t count;

			// Data, should be vertices.
			Ref<void> data;

			CpuAccessFlag accessFlag;
			Usage usage;
			MapType mapType;
		};
	public:
		DXIndexBuffer();
		DXIndexBuffer(const DXIndexBuffer& source);
		~DXIndexBuffer() override = default;

		DXIndexBuffer& operator=(const DXIndexBuffer& source);

		// Create functions.
		void Create(
			const IndexBufferInfo& info
		);

		// Map/UnMap
		void* Map() override;
		void UnMap() const override;

		void Bind() const override;
		void Clone(const Ref<IDirectXBuffer>& buffer) const override;
	private:
		IndexBufferInfo _info;
	};
}
