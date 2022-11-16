#pragma once

#include <string>

#include "Nlohmann/json.hpp"

#include "MathGeoLib/include/Math/float2.h"
#include "MathGeoLib/include/Math/float3.h"

namespace nlohmann 
{
	struct JsonData
	{
		nlohmann::ordered_json data;
		
		void SetInt(std::string name, int value) { data.emplace(name, value); }
		void SetFloat(std::string name, float value) { data.emplace(name, value); }
		void SetString(std::string name, std::string value) { data.emplace(name, value); }
		void SetBool(std::string name, bool value) { data.emplace(name, value); }

		void SetFloat2(std::string name, float2 value)
		{
			nlohmann::json arr;
			arr.push_back(value.At(0));
			arr.push_back(value.At(1));
			data.emplace(name, arr);
		}

		void SetFloat3(std::string name, float3 value)
		{
			nlohmann::json arr;
			arr.push_back(value.At(0));
			arr.push_back(value.At(1));
			arr.push_back(value.At(2));
			data.emplace(name, arr);
		}
	};
}