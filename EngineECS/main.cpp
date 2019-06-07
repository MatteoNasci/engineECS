
#include "private.h"
#include "Entity.h"
#include "World.h"
#include "components.h"
#include "Engine.h"
#include <iostream>
#include "OpenGLWrapper.h"
#include "constants.h"
#include "FpsManager.h"
#include "Mesh.h"
#include "SkeletalMesh.h"
#include "Math.h"
#include "OpenGLShader.h"
#include "ShaderManager.h"
#include "MeshManager.h"
//TODO:: divide library in 2: one to open fbx files and save them in new format and the second to load new format files into engine (use only this last one in engine so i can remove refs to fbx lib)
#include "FastFbxHandler.h"

static int defaultSkeletalProgram;
static int defaultProgram;
static int runningMesh;
int main(int argc, char **argv)
{
	engineECS::OpenGLWrapper wrapper = {};
	engineECS::ShaderManager::initialize();
	engineECS::MeshManager::initialize();

	{
		engineECS::World* world = nullptr;
		//engine init
		{
			engineECS::WorldCreation creationResult;
			WorldIndex index = engineECS::Engine::getEngine().createWorld(creationResult);

			if (creationResult != engineECS::WorldCreation::WC_OK)
			{
				std::cerr << "An error occurred while creating a world instance!" << std::endl;
			}
			world = engineECS::Engine::getEngine().getWorld(index);

			engineECS::EntityCreation result;
			engineECS::Entity enemy001 = world->createEntity(result);
			if (result != engineECS::EntityCreation::EC_OK)
			{
				std::cerr << "Unable to create an entity instance!" << std::endl;
			}
			engineECS::Entity player = world->createEntity(result);
			if (result != engineECS::EntityCreation::EC_OK)
			{
				std::cerr << "Unable to create an entity instance!" << std::endl;
			}
			engineECS::Entity enemy002 = world->createEntity(result);
			if (result != engineECS::EntityCreation::EC_OK)
			{
				std::cerr << "Unable to create an entity instance!" << std::endl;
			}

			world->tryAddComponent<engineECS::RendererComponent>(player);
			world->tryAddComponent<engineECS::RendererComponent>(enemy001);
			world->tryAddComponent<engineECS::RendererComponent>(enemy002);

			world->tryAddComponent<engineECS::TransformComponent>(player);
			world->tryAddComponent<engineECS::TransformComponent>(enemy001);

			if (world->tryAddComponent<engineECS::TransformComponent>(enemy002))
			{
				world->getComponent<engineECS::TransformComponent>(enemy002).rotation = glm::vec3(0.2f, 10.0f, -55.3f);
			}

			if (world->tryAddComponent<engineECS::RotatorComponent>(enemy002))
			{
				world->getComponent<engineECS::RotatorComponent>(enemy002).rotationSpeed = glm::vec3(5, 1, 100);
			}
			do
			{
				engineECS::Entity entity = world->createEntity(result);
				if (result == engineECS::EntityCreation::EC_OK)
				{
					if (world->tryAddComponent<engineECS::TransformComponent>(entity))
					{
						engineECS::TransformComponent& transform = world->getComponent<engineECS::TransformComponent>(entity);
						transform.rotation = glm::vec3(0.2f, 10.0f, -55.3f);
						transform.position = glm::vec3(static_cast<float>(entity.id), 10.0f, 0.f);
					}
					if (world->tryAddComponent<engineECS::RotatorComponent>(entity))
					{
						world->getComponent<engineECS::RotatorComponent>(entity).rotationSpeed = glm::vec3(5, 1, 100);
					}
					if (world->tryAddComponent<engineECS::RendererComponent>(entity))
					{
						engineECS::RendererComponent& renderer = world->getComponent<engineECS::RendererComponent>(entity);
						renderer.isEnabled = false;
						renderer.isSkeletal = true;
						renderer.meshType = 0;
						renderer.programType = 0;
					}
				}
			} while (result == engineECS::EntityCreation::EC_OK);

			world->destroyEntity(player);
		}

		//systems
		if (world)
		{
			if (!world->tryAddSystem(engineECS::System::create<engineECS::TransformComponent>([](const engineECS::Entity& inEntity, const float deltaTime)
			{
				//std::cout << "Hello from " << inEntity.id << " entity with " << deltaTime << " delta time! (" << (deltaTime == 0.f ? -1 : 1.f / deltaTime) << ")" << std::endl;
			})))
			{
				std::cerr << "Failed to create empty system!" << std::endl;
			}

			if (!world->tryAddSystem(engineECS::System::create<engineECS::TransformComponent, engineECS::RotatorComponent>([](const engineECS::Entity& inEntity, const float deltaTime)
			{
				engineECS::TransformComponent& transform = inEntity.getComponent<engineECS::TransformComponent>();
				engineECS::RotatorComponent& rotator = inEntity.getComponent<engineECS::RotatorComponent>();
				transform.rotation += rotator.rotationSpeed * deltaTime;
			})))
			{
				std::cerr << "Failed to create rotating system!" << std::endl;
			}

			if (!world->tryAddSystem(engineECS::System::create<engineECS::TransformComponent, engineECS::RendererComponent>([](const engineECS::Entity& inEntity, const float deltaTime)
			{
				const engineECS::RendererComponent& renderer = inEntity.getComponent<engineECS::RendererComponent>();
				if (!renderer.isEnabled)
				{
					return;
				}

				const engineECS::TransformComponent& transform = inEntity.getComponent<engineECS::TransformComponent>();

				glm::mat4 translation = glm::translate(glm::mat4(1), transform.position);
				glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(transform.rotation)));
				glm::mat4 scale = glm::scale(transform.scale);

				glm::mat4 model = translation * rotation * scale;

				glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0), glm::vec3(0, 1, 0));

				glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.01f, 1000.0f);

				const engineECS::OpenGLShader& shader = engineECS::ShaderManager::getCompiledShader(renderer.programType);

				glUniformMatrix4fv(shader.getModelUniform(), 1, GL_FALSE, &model[0][0]);
				glUniformMatrix4fv(shader.getViewUniform(), 1, GL_FALSE, &view[0][0]);
				glUniformMatrix4fv(shader.getProjectionUniform(), 1, GL_FALSE, &projection[0][0]);

				if (renderer.isSkeletal)
				{
					//glUniformMatrix4fv(shader.getBonesMatrixUniform(), animationMatrix.size(), GL_FALSE, &animationMatrix[0][0][0]);
				}

				glUseProgram(shader.getProgram());
				//glBindVertexArray(mesh.vao);
				//glDrawArrays(GL_TRIANGLES, 0, mesh.numVertices);
			})))
			{
				std::cerr << "Failed to create rendering system!" << std::endl;
			}
		}

		//meshes
		{
			std::vector<ffh::Vector3> vertices;
			std::vector<ffh::Vector3> normals;
			std::vector<ffh::Matrix4> bindPoses;
			std::vector<std::array<int, FFH_INFLUENCES_AMOUNT>> influences;
			std::vector<std::array<float, FFH_WEIGHTS_AMOUNT>> weights;
			std::map<std::string, std::vector<std::vector<ffh::Matrix4>>> animations;
			ffh::LoadResult result = ffh::FFHandler::loadFbxFile("D:\\source\\repos\\CPP\\AIV\\Running.fbx", 0, "Running", vertices, normals, bindPoses, influences, weights, animations);

			if (result != ffh::LoadResult::LR_Ok)
			{
				std::cerr << "An error occurred while loading a file with error: " << result << std::endl;
			}

			if (!engineECS::MeshManager::tryCreateSkeletalMesh(vertices, normals, bindPoses, influences, weights, animations, runningMesh))
			{
				std::cerr << "An error occurred while creating a mesh" << std::endl;
			}
			
			//engineECS::SkeletalMesh mesh002(vertices, normals, bindPoses, influences, weights, animations);
			//mesh002.upload();
		}

		//shaders
		{
			engineECS::ShaderManager::tryCreateOpenglShader(R"(
#version 410 core

layout(location=0) in vec3 vertex;
layout(location=1) in vec3 normal;
layout(location=2) in ivec4 bones;
layout(location=3) in vec4 weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 bones_matrix[100];

out float lambert;
out vec3 bone_color;

vec3 bones_colors[6] = vec3[](
	vec3(0, 1, 0),
	vec3(0, 0, 1),
	vec3(1, 1, 0),
	vec3(1, 0, 1),
	vec3(0.2, 0.5, 1),
	vec3(0, 1, 1)
);

void main()
{
	bone_color = vec3(1, 1, 1);
	if (bones.x < 6)
	{
		bone_color = bones_colors[bones.x] * weights.x;
	}
	if (bones.y < 6)
	{
		bone_color += bones_colors[bones.y] * weights.y;
	}
	if (bones.z < 6)
	{
		bone_color += bones_colors[bones.z] * weights.z;
	}
	if (bones.w < 6)
	{
		bone_color += bones_colors[bones.w] * weights.w;
	}

	mat4 bone_matrix = bones_matrix[bones.x] * weights.x;
	bone_matrix += bones_matrix[bones.y] * weights.y;
	bone_matrix += bones_matrix[bones.z] * weights.z;
	bone_matrix += bones_matrix[bones.w] * weights.w;

	vec3 light_vector = vec3(0, 0, -1);
	vec4 world_vertex = model * bone_matrix * vec4(vertex, 1);
	vec4 world_normal = normalize(model * bone_matrix * vec4(normal, 0));
	lambert = clamp(dot(-light_vector, world_normal.xyz), 0, 1);
	
	gl_Position = projection * view * world_vertex;
}
)", R"(

#version 410 core

in float lambert;
out vec4 color;
in vec3 bone_color;

void main()
{
	color = vec4(bone_color, 1) * lambert;
}
)",
"",
defaultSkeletalProgram);

			engineECS::ShaderManager::tryCreateOpenglShader(R"(
#version 410 core

layout(location=0) in vec3 vertex;
layout(location=1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float lambert;
out vec3 in_color;

void main()
{
	in_color = vec3(1, 0, 0);

	vec3 light_vector = vec3(0, 0, -1);
	vec4 world_vertex = model * vec4(vertex, 1);
	vec4 world_normal = normalize(model * vec4(normal, 0));
	lambert = clamp(dot(-light_vector, world_normal.xyz), 0, 1);
	
	gl_Position = projection * view * world_vertex;
}
)", R"(

#version 410 core

in float lambert;
out vec4 color;
in vec3 in_color;

void main()
{
	color = vec4(in_color, 1) * lambert;
}
)",
"",
defaultProgram);
		}
	}

	std::cout << "Game loop starting..." << std::endl;

	engineECS::FpsManager fpsManager = { &engineECS::OpenGLWrapper::getTime, nullptr };
	fpsManager.setTargetFramePerSecond(-1.);
	fpsManager.setDebugOptions(true, static_cast<int>(engineECS::FpsManager::SavedDeltaTimesCount));

	while (!wrapper.shouldCloseWindow())
	{
		fpsManager.update();

		wrapper.prepareFrame();

		engineECS::Engine::getEngine().run(fpsManager.getDeltaTime());

		wrapper.finalizeFrame();
	}

	return 0;
}