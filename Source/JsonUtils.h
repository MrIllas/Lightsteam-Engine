#pragma once

#include <string>

#include "Nlohmann/json.hpp"

#include "MathGeoLib/include/Math/float2.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Math/float4x4.h"

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

		double GetDouble(std::string name)
		{
			if (data.contains(name)) return data[name].get<double>();
			else return 0.0;
		}
		void SetDouble(std::string name, double value) { data.emplace(name, value); }

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

		float4 GetFloat4(std::string name)
		{
			float4 toReturn(0, 0, 0, 0);
			if (data.contains(name))
			{
				std::vector<float> temp = data[name].get<std::vector<float>>();

				if (temp.size() >= 4)
				{
					toReturn[0] = temp[0];
					toReturn[1] = temp[1];
					toReturn[2] = temp[2];
					toReturn[3] = temp[3];
				}
			}

			return toReturn;
		}

		void SetFloat4(std::string name, float4 value)
		{
			nlohmann::json arr;
			arr.push_back(value.At(0));
			arr.push_back(value.At(1));
			arr.push_back(value.At(2));
			arr.push_back(value.At(3));
			data.emplace(name, arr);
		}
		
		float4x4 GetFloat4x4(std::string name)
		{
			float4x4 toReturn(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
			if (data.contains(name))
			{
				std::vector<float> temp = data[name].get<std::vector<float>>();

				if (temp.size() >= 16)
				{
					toReturn[0][0] = temp[0];
					toReturn[0][1] = temp[1];
					toReturn[0][2] = temp[2];
					toReturn[0][3] = temp[3];

					toReturn[1][0] = temp[4];
					toReturn[1][1] = temp[5];
					toReturn[1][2] = temp[6];
					toReturn[1][3] = temp[7];

					toReturn[2][0] = temp[8];
					toReturn[2][1] = temp[9];
					toReturn[2][2] = temp[10];
					toReturn[2][3] = temp[11];

					toReturn[3][0] = temp[12];
					toReturn[3][1] = temp[13];
					toReturn[3][2] = temp[14];
					toReturn[3][3] = temp[15];
				}
			}

			return toReturn;
		}

		void SetFloat4x4(std::string name, float4x4 value)
		{
			nlohmann::json arr;
			for (int i = 0; i < 4; ++i)
			{
				for (int k = 0; k < 4; ++k)
				{
					arr.push_back(value.At(i, k));
				}
			}

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