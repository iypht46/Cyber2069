//Editor
#include "EditorObject/EditorEntity.hpp"
#include "EditorObject/CoreComponentEC.hpp"
#include "EditorObject/ParticleComponentEC.hpp"
#include "EditorObject/EditorSceneObject.hpp"
#include "EditorWindow/PopupWindow.hpp"
//White Engine
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"
#include "Core/Logger.hpp"
#include "Utility/Macros.hpp"
//Third Party Library
#include <imgui.h>
//Standard Library
#include <algorithm>
#include <iostream>

#include "Serialization/Serialization.h"

namespace Tools
{
	
	//Use when creating entity in editor
	EditorEntity::EditorEntity() 
	{
		m_gameObject = std::make_shared<GameObject>();
		LoadGameObject();
	}

	EditorEntity::~EditorEntity()
	{
		for (auto comp : m_components)
		{
			delete comp;
		}
	}

	EditorEntity::EditorEntity(GameObject* obj)
	{
		m_gameObject.reset(obj);
		LoadGameObject();
	}

	//Use when loading from prefab or scene
	EditorEntity::EditorEntity(GameObjectHandle gameObject)
	{
		//Create gameobject from parameter
		m_gameObject = gameObject;
		gameObject->SetAllComponents();
		LoadGameObject();
	}

	void EditorEntity::AddObject(GameObjectHandle obj)
	{
		EditorEntity* ent = new EditorEntity(obj);
		AddObject(ent);
	}

	void EditorEntity::AddObject(EditorObject* obj)
	{
		auto ent = dynamic_cast<EditorEntity*>(obj);

		if (ent)
		{
			m_childEntities.push_back(ent);

			if (this->IsInScene() && !ent->IsInScene())
				m_scene->AddObjToScene(ent->GetGameObject());
		}
		else
			ENGINE_ERROR("ERROR: Object is not entity!");
			
	}

	void EditorEntity::AddObject()
	{
		EditorEntity* ent = new EditorEntity();
			
		ent->SetParent(this);
		AddObject(ent);
	}

	bool EditorEntity::Load(Container::wString path)
	{
		if (m_gameObject)
		{
			m_gameObject.reset();
			m_gameObject = std::make_unique<GameObject>();
		}

		m_transformEC.reset();
		m_scene = nullptr;
		m_componentsMap.clear();
		m_components.clear();

		Serialization::LoadObject(*m_gameObject.get(), path);

		if (m_gameObject.get())
		{
			m_gameObject->SetAllComponents();
			LoadGameObject();
			
		}

		return (m_gameObject.get());
	}

	bool EditorEntity::Save(Container::wString path)
	{
		Serialization::SaveObject(*m_gameObject, path);

		return Utility::fs::is_regular_file(Utility::fs::path(path));
	}

	void EditorEntity::RenderGameObject(glm::mat4 cam)
	{
		auto mesh = dynamic_cast<MeshRendererEC*>(this->GetComponent("MeshRendererEC"));

		if (mesh)
			mesh->RenderTexture(cam);

		if (m_childEntities.size() > 0)
		{
			for (auto it = m_childEntities.begin(); it != m_childEntities.end(); ++it)
			{
				auto childMesh = dynamic_cast<MeshRendererEC*>((*it)->GetComponent("MeshRendererEC"));

				if (childMesh)
					childMesh->RenderTexture(cam);
			}
		}
	}

	bool EditorEntity::IsInScene()
	{
		return (m_scene);
	}

	void EditorEntity::SetScene(EditorSceneObject* scene)
	{
		m_scene = scene;
	}

	void EditorEntity::PreRender()
	{
		if (m_childEntities.size() == 0)
			m_nodeFlag |= (ImGuiTreeNodeFlags_Leaf);// | ImGuiTreeNodeFlags_NoTreePushOnOpen);
		else
			m_nodeFlag &= ~(ImGuiTreeNodeFlags_Leaf);// | ImGuiTreeNodeFlags_NoTreePushOnOpen);

		m_open = ImGui::TreeNodeEx(m_objectName.c_str(), m_nodeFlag);
	}

	void EditorEntity::OnRender()
	{
		HierarchyPopup();
		
		if (ImGui::IsItemClicked())
		{
			m_selectedObject = this->m_objectID;
			m_selected = this;
			m_nodeFlag |= ImGuiTreeNodeFlags_Selected;

			//if (m_selectedObject == this->m_objectID)//((m_nodeFlag & ImGuiTreeNodeFlags_Selected) != 0)
			//{
			//	m_selectedObject = 0;
			//	m_selected = nullptr;
			//	m_nodeFlag &= ~ImGuiTreeNodeFlags_Selected;
			//}
			//else
			//{
			//	m_selectedObject = this->m_objectID;
			//	m_selected = this;
			//	m_nodeFlag |= ImGuiTreeNodeFlags_Selected;
			//}
		}
		else if (m_selectedObject != this->m_objectID)
		{
			m_selected = nullptr;
			m_nodeFlag &= ~ImGuiTreeNodeFlags_Selected;
		}

		if (m_open && m_childEntities.size() > 0)
		{
			for (auto it = m_childEntities.begin(); it != m_childEntities.end(); ++it)
			{
				EditorEntity* child = (*it);

				child->Render();
				
				if (child->GetSelected())
					m_selected = child->GetSelected();
			}
			//ImGui::TreePop();
		}
	}

	void EditorEntity::PostRender()
	{
		if (m_open)// && (m_childEntities.size() != 0))
			ImGui::TreePop();

	}

	void EditorEntity::HierarchyPopup()
	{
		if (ImGui::BeginPopupContextItem("##EntityAction"))
		{
			if (ImGui::Selectable("Add Child Object"))
			{
				this->AddObject();
			}

			//If this entity is in scene
			if (m_scene)
			{
				if (ImGui::Selectable("Remove"))
				{
					m_scene->RemoveEntity(this);
				}
			}
			
			ImGui::EndPopup();
		}
	}

	void EditorEntity::LoadChildObject()
	{
		int childCount = m_gameObject->m_transform->GetChildCount();

		if (childCount > 0)
		{
			//Loop through of gameobject
			for (int i = 0; i < childCount; i++)
			{
				//Create Wrap child with editor entity and add to vector?
				GameObject* child = m_gameObject->m_transform->GetChild(i)->GetGameObject(); //Get child by index
				if (child)
				{
					EditorEntity* childEntity = new EditorEntity(child); //Wrap with eitor entity class.
					AddObject(childEntity); //Add to container
				}
			}
		}
	}

	void EditorEntity::LoadComponents()
	{
		for (auto it = m_gameObject->m_components.begin(); it != m_gameObject->m_components.end(); ++it)
		{
			auto component = *it;
			this->AddComponent(component.get());
		}
	}

	void EditorEntity::LoadGameObject()
	{
		m_objectName = m_gameObject->GetName();
		m_isActive = m_gameObject->Active();
		//Create transform
		m_transformEC = std::make_unique<TransformEC>();
		m_transformEC->Init(m_gameObject->m_transform.get());
		//this->AddComponentToEntity(m_transformEC.get());
		//this->LoadChildObject();
		this->LoadComponents();
		//this->GetGameObject()->SetAllComponents();
	}

	void EditorEntity::SetParent(EditorEntity* ent)
	{
		m_parent = ent;
		TransformEC* transEC = ent->GetTransformEC();
		m_transformEC->SetParent(transEC);
		
	}

	EditorEntity* EditorEntity::GetParent()
	{
		return m_parent;
	}

	GameObject* EditorEntity::GetGameObject()
	{
		return m_gameObject.get();
	}

	GameObjectHandle EditorEntity::GetGameObjectHandle()
	{
		return m_gameObject;
	}

	TransformEC* EditorEntity::GetTransformEC()
	{
		return m_transformEC.get();
	}

	void EditorEntity::AddComponent(std::string type)
	{
		//Check if component have been added
		if (m_componentsMap.find(type) != m_componentsMap.end())
		{
			ENGINE_INFO("Component Already Exist!!");
			return;
		}

		Component* comp = nullptr;
		if (type == "MeshRendererEC")//(type.find("MeshRendererEC") != std::string::npos)
		{
			comp = m_gameObject->AddComponent<MeshRenderer>();
		}
		else if (type == "BoxColliderEC")
		{
			
			comp = m_gameObject->AddComponent<BoxCollider>();
		}
		else if (type == "RigidbodyEC")
		{
			if (!m_gameObject->GetComponent<Collider>())
			{
				PopupData data("Add Component:", "Please add collider before adding rigidbody");
				PopupWindow::GetGlobalPopup().Push(data);
			}
			else
			{
				comp = m_gameObject->AddComponent<Rigidbody>();
			}
		}
		else if (type == "AnimatorEC")
		{
			comp = m_gameObject->AddComponent<Animator>();
		}
		else if (type == "ParticleComponentEC")
		{
			comp = m_gameObject->AddComponent<ParticleSystem>();
		}

		if (comp)
		{
			EditorComponent* compEC = EditorComponent::makeComponent(type);
			compEC->Init(comp);
			this->AddComponentToEntity(compEC);
		}
		else
		{
			ENGINE_ERROR("ERROR: Failed adding component of type: " + type + ".");
		}
	}

	void EditorEntity::AddComponent(Component* comp)
	{
		Container::wString type = "none";

		if (CHECK_TYPE(MeshRenderer, comp))
		{
			type = "MeshRendererEC";
		}
		else if (CHECK_TYPE(BoxCollider, comp))
		{
			type = "BoxColliderEC";
		}
		else if (CHECK_TYPE(Rigidbody, comp))
		{
			type = "RigidbodyEC";
		}
		else if (CHECK_TYPE(Animator, comp))
		{
			type = "AnimatorEC";
		}
		else if (CHECK_TYPE(SoundPlayer, comp))
		{
			type = "SoundPlayerEC";
		}
		/*else if (CHECK_TYPE(BehaviourScript, comp))
		{

		}*/

		if (type != "none")
		{
			EditorComponent* compEC = EditorComponent::makeComponent(type);
			compEC->Init(comp);
			this->AddComponentToEntity(compEC);
		}
		else
		{
			ENGINE_ERROR("ERROR: Component is not register in editor component list.");
		}
	}

	void EditorEntity::AddComponentToEntity(EditorComponent* compEC)
	{
		m_componentsMap[compEC->GetName()] = compEC;
		m_components.push_back(compEC);
	}

	EditorComponent * EditorEntity::GetComponent(std::string name)
	{
		if (m_componentsMap.find(name) == m_componentsMap.end())
			return nullptr;

		return m_componentsMap[name];
	}

	bool EditorEntity::RemoveComponent(std::string type)
	{
		//If component doesn't exist in map then do nothing
		auto it = m_componentsMap.find(type);
		if (it == m_componentsMap.end())
			return false;

		Component* comp = nullptr;
		if (type == "MeshRendererEC")//(type.find("MeshRendererEC") != std::string::npos)
		{
			m_gameObject->RemoveComponent<MeshRenderer>();
		}
		else if (type == "BoxColliderEC")
		{

			m_gameObject->RemoveComponent<BoxCollider>();
		}
		else if (type == "RigidbodyEC")
		{
				m_gameObject->RemoveComponent<Rigidbody>();
		}
		else if (type == "AnimatorEC")
		{
			m_gameObject->RemoveComponent<Animator>();
		}
		else if (type == "ParticleComponentEC")
		{
			m_gameObject->RemoveComponent<ParticleSystem>();
		}
		else
		{
			return false;
		}

		auto component = (*it).second;
		m_componentsMap.erase(it);
		auto componentIt = std::find(m_components.begin(), m_components.end(), component);
		if (componentIt != m_components.end())
			m_components.erase(componentIt);
		delete component;
		return true;
	}

	std::vector<EditorComponent*>& EditorEntity::GetComponentList()
	{
		return m_components;
	}

	void EditorEntity::PrintInfo()
	{
		Transform* transform = m_gameObject->m_transform.get();
		std::cout << "GameObject Name: " << m_gameObject->GetName() << std::endl;
		std::cout << "# of components: " << m_components.size() << std::endl;
		std::cout << "GameObject Transform: \n";
		
		std::cout << "Position: " << glm::to_string(transform->GetPosition()) << std::endl;
		std::cout << "Scale: " << glm::to_string(transform->GetScale()) << std::endl;
		std::cout << "Model Matrix: \n" << glm::to_string(transform->GetModelMatrix()) << std::endl;
	}

	std::string EditorEntity::GetName()
	{
		return m_gameObject->GetName();
	}

	int EditorEntity::GetID()
	{
		return m_gameObject->GetID();
	}

}
