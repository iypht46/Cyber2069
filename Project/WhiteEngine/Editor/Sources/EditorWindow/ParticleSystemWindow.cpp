#include "ParticleSystemWindow.hpp"
#include "Utility/Filesystem.hpp"
#include <misc/cpp/imgui_stdlib.h>


namespace Tools
{
	constexpr auto MAX_NUM = 9999;
	const std::string dirType[] = { "AWAY_FROM_CENTER", "TO_CENTER", "CUSTOM" };

	void ParticleSystemWindow::SetObjectToRender(ParticleSystem* particle)
	{
		m_particleToRender = particle;
	}

	PS_STATE ParticleSystemWindow::RunState()
	{
		return m_runState;
	}

	void ParticleSystemWindow::Init()
	{
		m_animationFileBrowser.SetTitle("Select Animation Controller");
		m_textureFileBrowser.SetTitle("Select Texture");
		m_animationFileBrowser.SetTypeFilters({ ".animcon" });
		m_textureFileBrowser.SetTypeFilters({ ".png", ".jpg" });
	}

	void ParticleSystemWindow::OnRender()
	{
		if (m_particleToRender)
			RenderObject(m_particleToRender);
	}

	void ParticleSystemWindow::Terminate()
	{

	}

	void ParticleSystemWindow::RenderObject(ParticleSystem* particle)
	{
		if (ImGui::Button("Play"))
		{
			if (m_runState == PS_STATE::PS_DEFAULT)
				particle->Init();

			m_runState = PS_STATE::PS_PLAY;

			if (!particle->emitter->constantParticle)
				particle->TriggerBurstEmission();
		}
			
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			m_runState = PS_STATE::PS_PAUSE;
		}
			
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			particle->Reset();
			m_runState = PS_STATE::PS_DEFAULT;
		}

		ImGui::AlignTextToFramePadding(); ImGui::Text("Particle Texture: "); ImGui::SameLine();
		auto path = Utility::fs::path(particle->texturePath);
		std::string filename;
		if (Utility::fs::exists(path) && Utility::fs::is_regular_file(path))
		{
			filename = path.filename().generic_string();
		}
		else
		{
			filename = "none";
		}
		ImGui::TextDisabled(filename.c_str()); ImGui::SameLine();

		if (ImGui::Button(".."))
		{
			m_textureFileBrowser.Open();

			if (m_runState != PS_STATE::PS_DEFAULT)
			{
				m_runState = PS_STATE::PS_DEFAULT;
				particle->Reset();
			}
		}
			

		m_textureFileBrowser.Display();

		if (m_textureFileBrowser.HasSelected())
		{
			auto path = Utility::File::GetRelativePath(m_textureFileBrowser.GetSelected(), Utility::File::s_projectPath);

			if (path.has_filename() && Utility::fs::is_regular_file(path))
			{
				particle->texturePath = path.generic_string();
			}
		}

		RenderObject(particle->emitter.get());
		RenderObject(particle->lifetime.get());
		RenderObject(particle->shape.get());
		RenderColorEditor(particle);
		RenderVelocityEditor(particle);
		RenderObject(particle->animation.get());
	}

	void ParticleSystemWindow::RenderObject(ParticleSystem::ParticleEmitter* emitter)
	{
		/*ImGui::AlignTextToFramePadding(); ImGui::Text("Enable"); ImGui::SameLine();
		ImGui::Checkbox("##EmitterEnable", &emitter->isEnabled);*/

		bool open = ImGui::CollapsingHeader("Particle Emitter");

		if (!open)
			return;

		ImGui::AlignTextToFramePadding(); ImGui::Text("Enable"); ImGui::SameLine();
		ImGui::Checkbox("##EmitterEnable", &emitter->isEnabled);

		if (!emitter->isEnabled)
			return;


		if (ImGui::TreeNode("Spawn Setting"))
		{
			ImGui::PushItemWidth(-1);
			ImGui::AlignTextToFramePadding(); ImGui::Text("Particle Samples"); ImGui::SameLine();
			ImGui::DragInt("##PSEmitterSamples", &emitter->particleSamples, 1, 0, MAX_NUM);

			/*ImGui::AlignTextToFramePadding(); ImGui::Text("Constant Particle"); ImGui::SameLine();
			ImGui::Checkbox("##EmitterConstant", &emitter->constantParticle);*/

			ImGui::AlignTextToFramePadding(); ImGui::Text("Spawn Type: "); ImGui::SameLine();
			if (ImGui::RadioButton("Constant", (emitter->constantParticle)))
				emitter->constantParticle = true;
			ImGui::SameLine();
			if (ImGui::RadioButton("Burst", (!emitter->constantParticle)))
				emitter->constantParticle = false;

			ImGui::AlignTextToFramePadding(); ImGui::Text("Constant Numbers"); ImGui::SameLine();
			ImGui::DragFloat("##EmitterConstSpawnRate", &emitter->particleRate, 0.1f, 0.0f, MAX_NUM);

			if (!emitter->constantParticle)
			{
				ImGui::AlignTextToFramePadding(); ImGui::Text("Burst Numbers"); ImGui::SameLine();
				ImGui::DragInt("##PSEmitterBurstNumber", &emitter->burstParticleNumber, 1, 0, MAX_NUM);
			}

			ImGui::PopItemWidth();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Spawn Shape Setting"))
		{
			ImGui::PushItemWidth(-1);
			ImGui::AlignTextToFramePadding(); ImGui::Text("Spawn On Edge"); ImGui::SameLine();
			ImGui::Checkbox("##EmitterSpawnOnEdge", &emitter->spawnOnEdge);

			ImGui::AlignTextToFramePadding(); ImGui::Text("Spawn Radius"); ImGui::SameLine();
			ImGui::DragFloat("##EmitterSpawnRadius", &emitter->spawnRadius, 0.1f, 0.0f, 360.0f);

			ImGui::AlignTextToFramePadding(); ImGui::Text("Min Emission Angle"); ImGui::SameLine();
			ImGui::DragFloat("##EmitterMinAngle", &emitter->minEmissionAngle, 0.1f, 0.0f, 360.0f);

			ImGui::AlignTextToFramePadding(); ImGui::Text("Max Emission Angle"); ImGui::SameLine();
			ImGui::DragFloat("##EmitterMaxAngle", &emitter->maxEmissionAngle, 0.1f, 0.0f, 360.0f);
			ImGui::TreePop();
			ImGui::PopItemWidth();
		}

	}

	void ParticleSystemWindow::RenderObject(ParticleSystem::ParticleLifeTime* lifetime)
	{
		bool open = ImGui::CollapsingHeader("Particle Lifetime");

		if (!open)
			return;

		ImGui::AlignTextToFramePadding(); ImGui::Text("Enable"); ImGui::SameLine();
		ImGui::Checkbox("##PSLifeTimeEnable", &lifetime->isEnabled);

		if (!lifetime->isEnabled)
			return;

		ImGui::PushItemWidth(-1);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Min Life Time"); ImGui::SameLine();
		ImGui::DragFloat("##PSLifeTimeMin", &lifetime->minLifeTime, 0.1f, 0.0f, MAX_NUM);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Max Life Time"); ImGui::SameLine();
		ImGui::DragFloat("##PSLifeTimeMax", &lifetime->maxLifeTime, 0.1f, 0.0f, MAX_NUM);
		ImGui::PopItemWidth();

	}

	void ParticleSystemWindow::RenderObject(ParticleSystem::ParticleShape* shape)
	{
		bool open = ImGui::CollapsingHeader("Particle Shape");

		if (!open)
			return;

		ImGui::AlignTextToFramePadding(); ImGui::Text("Enable"); ImGui::SameLine();
		ImGui::Checkbox("##PSShapeEnable", &shape->isEnabled);

		if (!shape->isEnabled)
			return;

		if (ImGui::TreeNode("Shape Setting"))
		{
			ImGui::PushItemWidth(-1);

			ImGui::AlignTextToFramePadding(); ImGui::Text("X Min Size"); ImGui::SameLine();
			ImGui::DragFloat("##PSminXSize", &shape->minXSize, 0.1f, 0.0f, MAX_NUM);
			ImGui::AlignTextToFramePadding(); ImGui::Text("X Max Size"); ImGui::SameLine();
			ImGui::DragFloat("##PSmaxXSize", &shape->maxXSize, 0.1f, 0.0f, MAX_NUM);
			ImGui::AlignTextToFramePadding(); ImGui::Text("Y Min Size"); ImGui::SameLine();
			ImGui::DragFloat("##PSminYSize", &shape->minYSize, 0.1f, 0.0f, MAX_NUM);
			ImGui::AlignTextToFramePadding(); ImGui::Text("Y Max Size"); ImGui::SameLine();
			ImGui::DragFloat("##PSmaxYSize", &shape->maxYSize, 0.1f, 0.0f, MAX_NUM);

			ImGui::AlignTextToFramePadding(); ImGui::Text("Custom Shape Lifetime"); ImGui::SameLine();
			ImGui::Checkbox("####PSEmitterCustom", &shape->usingLifetimeModifier);

			if (shape->usingLifetimeModifier)
			{
				ImGui::AlignTextToFramePadding(); ImGui::Text("Start Time"); ImGui::SameLine();
				ImGui::DragFloat("##PSStartTime", &shape->shape_ModStart, 0.1f, 0.0f, MAX_NUM);
				ImGui::AlignTextToFramePadding(); ImGui::Text("X Modifer"); ImGui::SameLine();
				ImGui::DragFloat("##PSXmodifier", &shape->sizeXModifierbyLifeTime, 0.01f, 0.0f, MAX_NUM);
				ImGui::AlignTextToFramePadding(); ImGui::Text("Y Modifer"); ImGui::SameLine();
				ImGui::DragFloat("##PSYmodifer", &shape->sizeYModifierbyLifeTime, 0.1f, 0.0f, MAX_NUM);
			}
			ImGui::PopItemWidth();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Rotation Setting"))
		{
			ImGui::PushItemWidth(-1);
			int currentDir = static_cast<int>(shape->rotationType);
			if (ImGui::BeginCombo("Rotation Type", dirType[currentDir].c_str()))
			{
				for (int i = 0; i < (sizeof(dirType) / sizeof(dirType[0])); i++)
				{
					bool is_selected = (static_cast<int>(shape->rotationType) == i);
					if (ImGui::Selectable(dirType[i].c_str(), &is_selected))
					{
						shape->rotationType = static_cast<ParticleSystem::DirectionType>(i);
						shape->sr_rotationTypeAsInt = i;
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::AlignTextToFramePadding(); ImGui::Text("Min Rotation"); ImGui::SameLine();
			ImGui::DragFloat("##PSminRot", &shape->minRotation, 0.1f, 0.0f, 360.f);
			ImGui::AlignTextToFramePadding(); ImGui::Text("Max Rotation"); ImGui::SameLine();
			ImGui::DragFloat("##PSmaxRot", &shape->maxRotaion, 0.1f, 0.0f, 360.f);

			ImGui::AlignTextToFramePadding(); ImGui::Text("Custom Rotation Lifetime"); ImGui::SameLine();
			ImGui::Checkbox("##PSEmitterCustomRot", &shape->usingLifetimeRotModifier);

			if (shape->usingLifetimeRotModifier)
			{
				ImGui::AlignTextToFramePadding(); ImGui::Text("Rotation Modifier"); ImGui::SameLine();
				ImGui::DragFloat("##PSRotmodifier", &shape->rotationModifier, 0.01f, 0.0f, MAX_NUM);
			}
			ImGui::PopItemWidth();
			ImGui::TreePop();
		}

	}

	void ParticleSystemWindow::RenderColorEditor(ParticleSystem* particle)
	{
		bool open = ImGui::CollapsingHeader("Particle Color");

		if (!open)
			return;

		ImGui::AlignTextToFramePadding(); ImGui::Text("Enable"); ImGui::SameLine();
		ImGui::Checkbox("##PSColorEnable", &particle->color->isEnabled);

		if (!particle->color->isEnabled)
			return;

		ImGui::PushItemWidth(-1);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Color Value"); ImGui::SameLine();
		if (ImGui::ColorEdit3("##PSColorValue", &particle->color->Color[0]) && m_runState != PS_STATE::PS_DEFAULT)
		{
			particle->UpdateMesh();
		}

		ImGui::AlignTextToFramePadding(); ImGui::Text("Use LifeTime Modifier"); ImGui::SameLine();
		ImGui::Checkbox("##PSColorLifeTimeMod", &particle->color->useLifeTimeMod);
		ImGui::PopItemWidth();
	}

	void ParticleSystemWindow::RenderVelocityEditor(ParticleSystem* particle)
	{
		bool open = ImGui::CollapsingHeader("Particle Velocity");

		if (!open)
			return;

		ImGui::AlignTextToFramePadding(); ImGui::Text("Enable"); ImGui::SameLine();
		ImGui::Checkbox("##PSVeloEnable", &particle->velocity->isEnabled);

		if (!particle->velocity->isEnabled)
		{
			return;
		}

		ImGui::PushItemWidth(-1);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Enable Lifetime Modifier"); ImGui::SameLine();
		ImGui::Checkbox("##PSVeloLifeTimeModEnable", &particle->velocity->usingLifetimeModifier);

		if (particle->velocity->usingLifetimeModifier)
		{
			ImGui::SameLine();
			ImGui::DragFloat("##PSVeloSpeedMod", &particle->velocity->lifetimeSpeedModifier, 0.1f, 0.0f, MAX_NUM);
		}
		ImGui::PopItemWidth();

		if (ImGui::TreeNode("Speed Setting"))
		{
			ImGui::PushItemWidth(-1);
			ImGui::AlignTextToFramePadding(); ImGui::Text("Min Speed"); ImGui::SameLine();
			ImGui::DragFloat("##PSVeloMinSpeed", &particle->velocity->minSpeed, 0.1f, 0.0f, MAX_NUM);

			ImGui::AlignTextToFramePadding(); ImGui::Text("Max Speed"); ImGui::SameLine();
			ImGui::DragFloat("##PSVeloMaxSpeed", &particle->velocity->maxSpeed, 0.1f, 0.0f, MAX_NUM);

			int currentDir = static_cast<int>(particle->velocity->GetDirectionType());
			if (ImGui::BeginCombo("Direction", dirType[particle->velocity->sr_directionTypeAsInt].c_str()))
			{
				for (int i = 0; i < (sizeof(dirType) / sizeof(dirType[0])); i++)
				{
					bool is_selected = (currentDir == i);
					if (ImGui::Selectable(dirType[i].c_str(), &is_selected))
					{
						particle->velocity->SetDirectiontype(static_cast<ParticleSystem::DirectionType>(i));
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (particle->velocity->GetDirectionType() == ParticleSystem::DirectionType::Custom)
			{
				ImGui::AlignTextToFramePadding(); ImGui::Text("Direction X"); ImGui::SameLine();
				ImGui::DragFloat("##PSVeloCustomDirX", &particle->velocity->customDirection.x, 0.1f, -1.0f, 1.0f);
				ImGui::AlignTextToFramePadding(); ImGui::Text("Direction Y"); ImGui::SameLine();
				ImGui::DragFloat("##PSVeloCustomDirY", &particle->velocity->customDirection.y, 0.1f, -1.0f, 1.0f);
			}
			ImGui::PopItemWidth();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Physics Setting"))
		{
			ImGui::PushItemWidth(-1);
			ImGui::AlignTextToFramePadding(); ImGui::Text("Gravity Scale"); ImGui::SameLine();
			if (ImGui::DragFloat("##PSVeloGravity", &particle->velocity->gravityScale, 0.1f, 0.0f, MAX_NUM))
				particle->UpdateRigidbody();

			ImGui::AlignTextToFramePadding(); ImGui::Text("Drag"); ImGui::SameLine();
			if (ImGui::DragFloat("##PSVeloDrag", &particle->velocity->drag, 0.1f, 0.0f, MAX_NUM))
				particle->UpdateRigidbody();

			ImGui::AlignTextToFramePadding(); ImGui::Text("Collide"); ImGui::SameLine();
			ImGui::Checkbox("##PSVeloColliding", &particle->velocity->colliding);

			ImGui::AlignTextToFramePadding(); ImGui::Text("Layer Name"); ImGui::SameLine();
			ImGui::InputText("##PSVeloPhysicsLayer", &particle->velocity->PhysicsLayer);
			ImGui::PopItemWidth();

			ImGui::TreePop();
		}

	}

	void ParticleSystemWindow::RenderObject(ParticleSystem::ParticleAnimation* animation)
	{
		bool open = ImGui::CollapsingHeader("Particle Animation");
		
		if (!open)
			return;
		
		ImGui::AlignTextToFramePadding(); ImGui::Text("Enable"); ImGui::SameLine();
		ImGui::SameLine(); ImGui::Checkbox("##PSAnimEnable", &animation->isEnabled);

		if (!animation->isEnabled)
			return;

		ImGui::PushItemWidth(-1);
		//Animation Loaded
		ImGui::AlignTextToFramePadding(); ImGui::Text("Animation Controller: "); ImGui::SameLine();
		ImGui::TextDisabled(animation->controllerPath.c_str()); ImGui::SameLine();

		if (ImGui::Button(".."))
			m_animationFileBrowser.Open();

		ImGui::PopItemWidth();

		m_animationFileBrowser.Display();

		if (m_animationFileBrowser.HasSelected())
		{
			auto path = Utility::File::GetRelativePath(m_animationFileBrowser.GetSelected(), Utility::File::s_projectPath);

			if (path.has_filename() && Utility::fs::is_regular_file(path))
			{
				animation->controllerPath = path.generic_string();
			}
		}
	}
}

