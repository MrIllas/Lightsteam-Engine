#include "SegmentScene.h"

#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModuleEditor.h"
#include "ModuleResources.h"
#include "FrameBuffer.h"

#include "Camera.h"
#include "GameObject.h"
#include "CompTransform.h"

#include "MathGeoLib/include/Math/float2.h"
#include "ImGui/imgui_internal.h"

#include "ImGuizmo/ImGuizmo.h"

#include "LibraryFolder.h"
#include "ResourceMesh.h"
#include "MeshImporter.h"

SegmentScene::SegmentScene(bool enabled) : Segment(enabled)
{
	name = "Scene";
	segmentSize = { 0, 0 };

	camInstance = CameraProperties::Instance();
	sceneInstance = SceneProperties::Instance();
	editorInstance = EditorProperties::Instance();
	resourceInstance = ResourceProperties::Instance();
}

SegmentScene::~SegmentScene()
{
	//renderer->CleanUp();
	//RELEASE(renderer);
}

void SegmentScene::Start()
{
	camInstance->editorCamera.SetRenderer({ segmentSize.x, segmentSize.y });
}

void SegmentScene::Update()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar;
	flags |= ImGuiWindowFlags_NoScrollWithMouse;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f,0.0f));
	if (ImGui::Begin(name.c_str(), 0, flags))
	{
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 winSize = ImGui::GetWindowSize();
		winSize.x += winPos.x;
		winSize.y += winPos.y;

		if (ImGui::IsMouseHoveringRect(winPos, winSize))
		{
			ImVec2 mouseGlobalPos = ImGui::GetMousePos();
			float2 normMousePos = float2(mouseGlobalPos.x - winPos.x, mouseGlobalPos.y - winPos.y - ImGui::GetFrameHeight());

			camInstance->isMouseOnScene = true;
			camInstance->mouseScreenPos.x = ((normMousePos.x / segmentSize.x) - 0.5) * 2.0f;
			camInstance->mouseScreenPos.y = ( (1.0f -(normMousePos.y / segmentSize.y)) - 0.5) * 2.0f;

		}
		else camInstance->isMouseOnScene = false;

		ImVec2 aux = ImGui::GetContentRegionAvail();

		if (aux.x != segmentSize.x || aux.y != segmentSize.y)
		{
			segmentSize.x = aux.x;
			segmentSize.y = aux.y;
			camInstance->editorCamera.SetAspectRatio(segmentSize.x / segmentSize.y);

			if (camInstance->editorCamera.renderer == nullptr)
			{
				camInstance->editorCamera.SetRenderer({ segmentSize.x, segmentSize.y });
			}
			else
			{
				camInstance->editorCamera.renderer->Resize({ segmentSize.x, segmentSize.y });
			}
		}
		RenderSpace();
		Guizmo(camInstance->editorCamera, sceneInstance->GetSelectedGO());


		
	}
	ImGui::End();
	ImGui::PopStyleVar();
}

void SegmentScene::RenderSpace()
{
	//float aux = (ImGui::GetWindowHeight()+20 - segmentSize.y) * 0.5f;

	//Render Framebuffer
	//ImGui::SetCursorPosY(aux);

	camInstance->editorCamera.renderer->GetFrameBufffer()->SetViewport();

	ImTextureID texID = (ImTextureID)camInstance->editorCamera.renderer->GetFrameBufffer()->GetTextureBuffer();
	ImGui::Image(texID, segmentSize, ImVec2(0, 1), ImVec2(1, 0));
	DropTarget();
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

	float4x4 aux = transform->GetLocalMatrix();

	ImGuizmo::SetRect(x, y, w, h);
	if (ImGuizmo::Manipulate(cam.GetViewMatrix(), cam.GetProjectionMatrix(), (ImGuizmo::OPERATION) sceneInstance->GetGuizmoOperation(), ImGuizmo::WORLD, &aux.v[0][0]))
	{
		aux.Transpose();
		transform->SetWorldMatrix(aux);
	}
		
}

void SegmentScene::DropTarget()
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserItem"))
		{
			IM_ASSERT(payload->DataSize == sizeof(LibraryItem));
			const LibraryItem item = *static_cast<const LibraryItem*>(payload->Data);

			if (item.extension.compare("fbx"))
			{
				ResourceMesh* res = (ResourceMesh*) resourceInstance->resources[item.resUuid];
			}

			
		}

		ImGui::EndDragDropTarget();
	}
}