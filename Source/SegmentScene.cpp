#include "SegmentScene.h"

#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "FrameBuffer.h"

#include "Camera.h"
#include "GameObject.h"
#include "CompTransform.h"

#include "MathGeoLib/include/Math/float2.h"
#include "ImGui/imgui_internal.h"

#include "ImGuizmo/ImGuizmo.h"

SegmentScene::SegmentScene(bool enabled) : Segment(enabled)
{
	name = "Scene";
	segmentSize = { 0, 0 };

	camInstance = CameraProperties::Instance();
	sceneInstance = SceneProperties::Instance();
}

SegmentScene::~SegmentScene()
{
	//renderer->CleanUp();
	//RELEASE(renderer);
}

void SegmentScene::Start()
{
	//renInstance->render = new Renderer({ segmentSize.x, segmentSize.y });
	//camInstance->editorCamera.renderer = new Renderer({ segmentSize.x, segmentSize.y });
	camInstance->editorCamera.SetRenderer({ segmentSize.x, segmentSize.y });
}

void SegmentScene::Update()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar;
	flags |= ImGuiWindowFlags_NoScrollWithMouse;
	if (ImGui::Begin(name.c_str(), 0, flags))
	{
		ImVec2 pos = ImGui::GetWindowPos();
		ImVec2 size = ImGui::GetWindowSize();
		size.x += pos.x;
		size.y += pos.y;

		if (ImGui::IsMouseHoveringRect(pos, size)) camInstance->isMouseOnScene = true;
		else camInstance->isMouseOnScene = false;

		ImVec2 aux = ImGui::GetContentRegionAvail();
		aux.y = (aux.x / 16) * 9;


		if (aux.x != segmentSize.x || aux.y != segmentSize.y)
		{
			segmentSize.x = aux.x;
			segmentSize.y = aux.y;

			if (camInstance->editorCamera.renderer == nullptr)
			{
				camInstance->editorCamera.SetRenderer({ segmentSize.x, segmentSize.y });
				//camInstance->editorCamera.renderer = new Renderer({ segmentSize.x, segmentSize.y });
			}
			else camInstance->editorCamera.renderer->Resize({ segmentSize.x, segmentSize.y });

		}

		RenderSpace();
		Guizmo(camInstance->editorCamera, sceneInstance->GetSelectedGO());
	}
	ImGui::End();
}

void SegmentScene::RenderSpace()
{
	float aux = (ImGui::GetWindowHeight()+20 - segmentSize.y) * 0.5f;

	//Render Framebuffer
	ImGui::SetCursorPosY(aux);
	ImTextureID texID = (ImTextureID)camInstance->editorCamera.renderer->GetFrameBufffer()->GetTextureBuffer();
	ImGui::Image(texID, segmentSize, ImVec2(0, 1), ImVec2(1, 0));
}

void SegmentScene::Guizmo(Camera& cam, GameObject* go)
{
	if (go == nullptr) return;
	CompTransform* transform = go->GetComponent<CompTransform>(TRANSFORM);
	if (transform == nullptr) return;

	//ImGuizmo::Enable(true);


	ImGuizmo::SetDrawlist();

	float x = ImGui::GetWindowPos().x;
	float y = ImGui::GetWindowPos().y;
	float w = segmentSize.x;
	float h = segmentSize.y;
	//Guizmo

	float4x4 aux = transform->GetWorldMatrix();


	ImGuizmo::SetRect(x, y, w, h);
	if (ImGuizmo::Manipulate(cam.GetViewMatrix(), cam.GetProjectionMatrix(), (ImGuizmo::OPERATION) sceneInstance->GetGuizmoOperation(), ImGuizmo::WORLD, &aux.v[0][0]))
	{
		aux.Transpose();
		transform->SetWorldMatrix(aux);
	}
	

	if (ImGuizmo::IsOver())
	{
		
	}
	
}