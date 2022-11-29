#include <string>
#include <random>
namespace LS_UUID
{
	inline std::string Generate()
	{
		std::string toReturn;
		static std::random_device device;
		static std::mt19937 random(device());

		std::uniform_int_distribution<int> dist(0, 15);

		const char* pool = "0123456789abcdef";
		const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

		for (int i = 0; i < 16; i++)
		{
			if (dash[i]) toReturn += "-";

			toReturn += pool[dist(random)];
			toReturn += pool[dist(random)];
		}

		return toReturn;
	}
}