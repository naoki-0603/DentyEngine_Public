#pragma once

#include <random>

namespace DentyEngine
{
	//
	// reference
	//

	// https://www.youtube.com/watch?v=GK0jHlv3e3w&t=922s
	// https://stackoverflow.com/questions/32575167/generate-random-numbers-with-uniform-distribution-getting-same-number-in-loop

	class Random final
	{
	public:
		Random() = default;
		~Random() = default;

		//
		// Integers
		//
		[[nodiscard]]
		static uint32_t GetRandomValuesUint32T()
		{
			static std::default_random_engine generator;
			std::uniform_int_distribution<uint32_t> distribution;

			return distribution(generator);
		}

		[[nodiscard]]
		static uint64_t GetRandomValuesUint64T()
		{
			static std::default_random_engine generator;
			std::uniform_int_distribution<uint64_t> distribution;

			return distribution(generator);
		}

		//
		// Float or double
		//

		[[nodiscard]]
		static float GetRandomValueAsFloat()
		{
			static std::default_random_engine generator;
			std::uniform_int_distribution<uint32_t> distribution;

			return static_cast<float>(distribution(generator)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
		}

		[[nodiscard]]
		static double GetRandomValueAsDouble()
		{
			static std::default_random_engine generator;
			std::uniform_int_distribution<uint32_t> distribution;

			return static_cast<double>(distribution(generator)) / static_cast<double>(std::numeric_limits<uint32_t>::max());
		}
	};
}
