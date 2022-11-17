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
		
		int GetInt(std::string name)
		{
			if (data.contains(name))	return data[name].get<int>();
			else return 0;
		}
		void SetInt(std::string name, int value) { data.emplace(name, value); }
		
		float GetFloat(std::string name)
		{
			if (data.contains(name))	return data[name].get<float>();
			else return 0.0f;
		}
		void SetFloat(std::string name, float value) { data.emplace(name, value); }

		std::string GetString(std::string name) 
		{ 
			if (data.contains(name))	return data[name].get<std::string>();
			else return "NULL";
		}
		void SetString(std::string name, std::string value) { data.emplace(name, value); }

		bool GetBool(std::string name)
		{
			if (data.contains(name))	return data[name].get<bool>();
			else return false;
		}
		void SetBool(std::string name, bool value) { data.emplace(name, value); }

		float2 GetFloat2(std::string name)
		{
			float2 toReturn(0, 0);
			if (data.contains(name))
			{
				std::vector<float> temp = data[name].get<std::vector<float>>();

				if (temp.size() >= 2)
				{
					toReturn[0] = temp[0];
					toReturn[1] = temp[1];
				}
			}
			
			return toReturn;
		}
		void SetFloat2(std::string name, float2 value)
		{
			nlohmann::json arr;
			arr.push_back(value.At(0));
			arr.push_back(value.At(1));
			data.emplace(name, arr);
		}

		float3 GetFloat3(std::string name)
		{
			float3 toReturn(0, 0, 0);
			if (data.contains(name))
			{
				std::vector<float> temp = data[name].get<std::vector<float>>();

				if (temp.size() >= 3)
				{
					toReturn[0] = temp[0];
					toReturn[1] = temp[1];
					toReturn[2] = temp[2];
				}
			}

			return toReturn;
		}
		void SetFloat3(std::string name, float3 value)
		{
			nlohmann::json arr;
			arr.push_back(value.At(0));
			arr.push_back(value.At(1));
			arr.push_back(value.At(2));
			data.emplace(name, arr);
		}

		std::vector<nlohmann::ordered_json> GetJsonVector(std::string name)
		{
			std::vector<nlohmann::ordered_json> toReturn;
			
			if (data.contains(name)) toReturn = data[name].get<nlohmann::ordered_json>();
			return toReturn;
		}
	};
}