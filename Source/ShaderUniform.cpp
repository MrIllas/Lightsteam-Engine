#include "ShaderUniform.h"

#include "ImGui/imgui.h"
#include "Glew/include/glew.h"
#include "MathGeoLib/include/Math/float2.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Math/float4x4.h"

#include "TextureImporter.h"
#include "ModuleResources.h"
#include "LibraryFolder.h"

ShaderUniform::ShaderUniform()
{

}

ShaderUniform::~ShaderUniform()
{
	VariableDeleting();
}

void ShaderUniform::VariableSetting()
{
	switch (type)
	{
		case GL_BOOL:
		{
			strType = "bool";
			bool* b = new bool(false);
			value = b;
		}
		break;
		case GL_INT:
		{
			strType = "int";
			int* i = new int(0);
			value = i;
		}
		break;
		case GL_UNSIGNED_INT:
		{
			strType = "uint";
			uint* ui = new uint(0);
			value = ui;
		}
		break;
		case GL_FLOAT:
		{
			strType = "float";
			float* f = new float(0.0f);
			value = f;
		}
		break;
		case GL_FLOAT_VEC2:
		{
			strType = "vec2";
			float2* f2 = new float2();
			value = f2;
		}
		break;
		case GL_FLOAT_VEC3:
		{
			strType = "vec3";
			float3* f3 = new float3();
			value = f3;
		}
		break;
		case GL_FLOAT_VEC4:
		{
			strType = "vec4";
			float4* f4 = new float4();
			value = f4;
		}
		break;
		case GL_DOUBLE:
		{
			strType = "double";
			double* d = new double(1.0);
			value = d;
		}
		break;
		case GL_SAMPLER_2D:
		{
			strType = "sampler2D";
			Texture* s2d = new Texture();
			value = s2d;
		}
		break;
		case GL_FLOAT_MAT4:
		{
			strType = "mat4";
			float4x4* f44 = new float4x4(float4x4::identity);
			value = f44;
		}
		break;
	}
}

void ShaderUniform::VariableDeleting()
{
	switch (type)
	{
		case GL_BOOL:
		{
			bool* b = static_cast<bool*>(value);
			RELEASE(b);
		}
		break;
		case GL_INT:
		{
			int* i = static_cast<int*>(value);
			RELEASE(i);
		}
		break;
		case GL_UNSIGNED_INT:
		{
			uint* ui = static_cast<uint*>(value);
			RELEASE(ui);
		}
		break;
		case GL_FLOAT:
		{
			float* f = static_cast<float*>(value);
			RELEASE(f);
		}
		break;
		case GL_FLOAT_VEC2:
		{
			float2* f2 = static_cast<float2*>(value);
			RELEASE(f2);
		}
		break;
		case GL_FLOAT_VEC3:
		{
			float3* f3 = static_cast<float3*>(value);
			RELEASE(f3);
		}
		break;
		case GL_FLOAT_VEC4:
		{
			float4* f4 = static_cast<float4*>(value);
			RELEASE(f4);
		}
		break;
		case GL_DOUBLE:
		{
			double* d = static_cast<double*>(value);
			RELEASE(d);
		}
		break;
		case GL_SAMPLER_2D:
		{
			Texture* s2d = static_cast<Texture*>(value);
			RELEASE(s2d);
		}
		break;
		case GL_FLOAT_MAT4:
		{
			float4x4* f44 = static_cast<float4x4*>(value);
			RELEASE(f44);
		}
		break;
	}
}

void ShaderUniform::HandleShaderGUI()
{
	if (name == "projection" || name == "view" || name == "model") return;

	ImGui::Text("Type: %s", strType.c_str());
	std::string name = this->name + "##" + std::to_string(index);
	std::string aux;
	switch (type)
	{
	case GL_BOOL:
	{
		ImGui::Checkbox(name.c_str(), static_cast<bool*>(value));
	}
	break;
	case GL_INT:
	{
		ImGui::InputInt(name.c_str(), static_cast<int*>(value));
	}
	break;
	case GL_UNSIGNED_INT:
	{
		ImGui::InputInt(name.c_str(), static_cast<int*>(value), 0, 4294967295);
	}
	break;
	case GL_FLOAT:
	{
		ImGui::InputFloat(name.c_str(), static_cast<float*>(value));
	}
	break;
	case GL_FLOAT_VEC2:
	{
		ImGui::InputFloat2(name.c_str(), &static_cast<float2*>(value)->At(0));
	}
	break;
	case GL_FLOAT_VEC3:
	{
		ImGui::InputFloat3(name.c_str(), &static_cast<float3*>(value)->At(0));
	}
	break;
	case GL_FLOAT_VEC4:
	{
		ImGui::InputFloat4(name.c_str(), &static_cast<float4*>(value)->At(0));
	}
	break;
	case GL_DOUBLE:
	{
		ImGui::InputDouble(name.c_str(), static_cast<double*>(value));
	}
	break;
	case GL_SAMPLER_2D:
	{
		Texture* tex = static_cast<Texture*>(value);

		ImGui::ImageButton((ImTextureID)tex->id, { 75, 75 });
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserItem"))
			{
				IM_ASSERT(payload->DataSize == sizeof(LibraryItem));
				const LibraryItem item = *static_cast<const LibraryItem*>(payload->Data);

				switch (str2int(item.extension.c_str()))
				{
				case str2int("png"):
				case str2int("PNG"):
				case str2int("DDS"):
				case str2int("dds"):
				case str2int("jpg"):
				case str2int("JPG"):

					ResourceTexture* res = (ResourceTexture*)ResourceProperties::Instance()->resources.at(item.resUuid);

					if (!tex->resUuid.empty()) //Decrease current RC
						ResourceProperties::Instance()->resources[tex->resUuid]->DecreaseRC();

					RELEASE(tex);
					value = new Texture(TextureImporter::ImportFromLibrary(res));
					tex = static_cast<Texture*>(value);
				}
			}
		}
		ImGui::SameLine();
		ImGui::Text("%s", name.c_str());
		std::string txt = "Size: " + std::to_string(tex->w) + " x " + std::to_string(tex->h);
		ImGui::Text(txt.c_str());
		txt = "Texture ID: " + std::to_string(tex->id);
		ImGui::Text(txt.c_str());
	}
	break;
	case GL_FLOAT_MAT4:
	{
		float4x4* f44 = static_cast<float4x4*>(value);
		//ImGui::Text(uni.name.c_str());
		aux = "1: " + name;
		ImGui::InputFloat4(aux.c_str(), &f44->v[0][0]);
		aux = "2: " + name;
		ImGui::InputFloat4(aux.c_str(), &f44->v[1][0]);
		aux = "3: " + name;
		ImGui::InputFloat4(aux.c_str(), &f44->v[2][0]);
		aux = "4: " + name;
		ImGui::InputFloat4(aux.c_str(), &f44->v[3][0]);
	}
	break;
	}
	ImGui::NewLine();
}

nlohmann::ordered_json ShaderUniform::GetJSON()
{
	nlohmann::JsonData toReturn;

	toReturn.SetString("Name", name);
	toReturn.SetInt("Type", type);
	toReturn.SetString("String Type", strType);
	toReturn.SetInt("Index", index);

	switch (type)
	{
		case GL_BOOL: toReturn.SetBool("Value", static_cast<bool*>(value)); break;
		case GL_INT:
		case GL_UNSIGNED_INT: toReturn.SetInt("Value", *static_cast<int*>(value)); break;
		case GL_FLOAT: toReturn.SetInt("Value", *static_cast<float*>(value)); break;
		case GL_FLOAT_VEC2: toReturn.SetFloat2("Value", *static_cast<float2*>(value)); break;
		case GL_FLOAT_VEC3: toReturn.SetFloat3("Value", *static_cast<float3*>(value)); break;
		//case GL_FLOAT_VEC4: toReturn.SetFloat4("Value", *static_cast<float4*>(value)); break;
		//case GL_DOUBLE: toReturn.SetDouble("Value", *static_cast<double*>(value)); break;
		case GL_SAMPLER_2D: toReturn.SetInt("Value", *static_cast<int*>(value)); break;
		//case GL_FLOAT_MAT4: toReturn.SetFloat4x4("Value", *static_cast<int*>(value)); break;
	}

	return toReturn.data;
}