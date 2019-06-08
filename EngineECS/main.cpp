
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
#include <PxPhysicsAPI.h>
#include "Allocator.h"
#include "SimulationEvent.h"
#include "ErrorCallback.h"
#include "Dispatcher.h"

static int defaultSkeletalProgram;
static int defaultProgram;
static int triangleProgram;
static int runningMesh;
static int randomMesh;
static int triangleMesh;
static int firstGeometry;
static int secondGeometry;
int main(int argc, char **argv)
{
	std::cout << "Start load process..." << std::endl;
	std::cout << sizeof(engineECS::Engine) << std::endl;

	engineECS::OpenGLWrapper wrapper = {};

	physx::PxRigidDynamic* body = nullptr;
	physx::PxRigidStatic*  body2 = nullptr;

	{
		engineECS::World* world = nullptr;
		engineECS::WorldCreation creationResult;
		WorldIndex index = GET_ENGINE.createWorld(creationResult);

		if (creationResult != engineECS::WorldCreation::WC_OK)
		{
			throw std::exception("An error occurred while creating a world instance!");
		}
		world = GET_ENGINE.getWorld(index);

		//physics
		{
			physx::PxMaterial* material = GET_PHYSICSENGINE.getPhysics()->createMaterial(1.f, 0.1f, 0.1f);
			if (!material)
			{
				throw std::exception("unable to create material");
			}

			physx::PxSphereGeometry sphere_geometry(10);

			physx::PxShape* shape = GET_PHYSICSENGINE.getPhysics()->createShape(sphere_geometry, *material, true);
			if (!shape)
			{
				throw std::exception("unable to create shape");
			}

			body = GET_PHYSICSENGINE.getPhysics()->createRigidDynamic(physx::PxTransform(0, 200, 0));
			if (!body)
			{
				throw std::exception("unable to create body");
			}
			body->setMass(1);
			body->attachShape(*shape);

			GET_PHYSICSENGINE.getScene()->addActor(*body);
			//if (!GET_MESHMANAGER.tryCreateMesh(shape, firstGeometry))
			//{
			//	throw std::exception("unable to create mesh from shape");
			//}

			physx::PxBoxGeometry box(20, 20, 20);
			physx::PxShape* shape2 = GET_PHYSICSENGINE.getPhysics()->createShape(box, *material, true);
			if (!shape2)
			{
				throw std::exception("unable to create shape");
			}

			body2 = GET_PHYSICSENGINE.getPhysics()->createRigidStatic(physx::PxTransform(0, -10, 0));
			if (!body2)
			{
				throw std::exception("unable to create body");
			}
			body2->attachShape(*shape2);
			GET_PHYSICSENGINE.getScene()->addActor(*body2);
			//if (!GET_MESHMANAGER.tryCreateMesh(shape2, secondGeometry))
			//{
			//	throw std::exception("unable to create mesh from shape");
			//}
		}

		//meshes
		{
			/*
			std::vector<ffh::Vector3> vertices;
			std::vector<ffh::Vector3> normals;
			std::vector<ffh::Vector2> uvs;
			std::vector<ffh::Matrix4> bindPoses;
			std::vector<std::array<int, FFH_INFLUENCES_AMOUNT>> influences;
			std::vector<std::array<float, FFH_WEIGHTS_AMOUNT>> weights;
			std::map<std::string, std::vector<std::vector<ffh::Matrix4>>> animations;
			ffh::LoadResult result = ffh::FFHandler::loadFbxFile("D:\\source\\repos\\CPP\\AIV\\Running.fbx", 0, "Running", vertices, normals, uvs, bindPoses, influences, weights, animations);

			if (result != ffh::LoadResult::LR_Ok)
			{
				std::cerr << "An error occurred while loading a file with error: " << result << std::endl;
			}

			if (!GET_MESHMANAGER.tryCreateSkeletalMesh(vertices, normals, uvs, bindPoses, influences, weights, animations, runningMesh))
			{
				std::cerr << "An error occurred while creating a skeletal mesh" << std::endl;
			}*/

			//std::vector<ffh::Vector3> vertices2;
			//std::vector<ffh::Vector3> normals2;
			//std::vector<ffh::Vector2> uvs2;

			//if (!GET_MESHMANAGER.tryCreateMesh(vertices2, normals2, uvs2, randomMesh))
			//{
			//	std::cerr << "An error occurred while creating a mesh" << std::endl;
			//}

			std::vector<ffh::Vector3> vertices3;
			std::vector<ffh::Vector3> normals3;
			std::vector<ffh::Vector2> uvs3;

			vertices3.push_back({ -1, -1, 0 });
			vertices3.push_back({ 0, 1, 0 });
			vertices3.push_back({ 1, -1, 0 });

			normals3.push_back({ 0, 0, 1 });
			normals3.push_back({ 0, 0, 1 });
			normals3.push_back({ 0, 0, 1 });

			uvs3.push_back({ 1,1 });
			uvs3.push_back({ -1,1 });
			uvs3.push_back({ 1,-1 });

			if (!GET_MESHMANAGER.tryCreateMesh(vertices3, normals3, uvs3, triangleMesh))
			{
				std::cerr << "An error occurred while creating a skeletal mesh" << std::endl;
			}
		}

		//shaders
		{
			/*
			GET_SHADERMANAGER.tryCreateOpenglShader(R"(
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

			GET_SHADERMANAGER.tryCreateOpenglShader(R"(
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
defaultProgram);*/
			GET_SHADERMANAGER.tryCreateOpenglShader(R"(
#version 410 core

layout(location=0) in vec3 vertex;

out vec3 out_color;

void main()
{
	out_color = vec3(1, 0, 0);
	
	gl_Position = vec4(vertex, 1);
}
)", R"(

#version 410 core

in vec3 out_color;
out vec4 final_color;

void main()
{
	final_color = vec4(out_color, 1);
}
)",
"",
triangleProgram);
		}

		//engine init
		{
			engineECS::EntityCreation result;
			do
			{
				engineECS::Entity entity = world->createEntity(result);
				if (result == engineECS::EntityCreation::EC_OK)
				{
					if (world->tryAddComponent<engineECS::TransformComponent>(entity))
					{
						engineECS::TransformComponent& transform = world->getComponent<engineECS::TransformComponent>(entity);
						transform.rotation = glm::vec3(0.2f, 10.0f, -55.3f);
						transform.position = glm::vec3(static_cast<float>(entity.id), 0.f, 0.f);
						transform.scale = glm::vec3(0.1f);
					}
					if (world->tryAddComponent<engineECS::RotatorComponent>(entity))
					{
						world->getComponent<engineECS::RotatorComponent>(entity).rotationSpeed = glm::vec3(5, 1, 100);
					}
					if (world->tryAddComponent<engineECS::RendererComponent>(entity))
					{
						engineECS::RendererComponent& renderer = world->getComponent<engineECS::RendererComponent>(entity);
						renderer.isEnabled = true;
						renderer.isSkeletal = false;
						renderer.meshType = triangleMesh;
						renderer.programType = triangleProgram;
					}
				}
			} while (result == engineECS::EntityCreation::EC_OK);
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

				//TODO: mettere dentro transform
				glm::mat4 translation = glm::translate(glm::mat4(1), transform.position);
				glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(transform.rotation)));
				glm::mat4 scale = glm::scale(transform.scale);

				glm::mat4 model = translation * rotation * scale;

				const engineECS::OpenGLShader& shader = GET_SHADERMANAGER.getCompiledShader(renderer.programType);
				glUseProgram(shader.getProgram());

				glUniformMatrix4fv(shader.getModelUniform(), 1, GL_FALSE, &model[0][0]);
				glUniformMatrix4fv(shader.getViewUniform(), 1, GL_FALSE, &GET_MAIN_CAMERA.getView()[0][0]);
				glUniformMatrix4fv(shader.getProjectionUniform(), 1, GL_FALSE, &GET_MAIN_CAMERA.getProjection()[0][0]);

				if (renderer.isSkeletal)
				{
					engineECS::SkeletalMesh& mesh = GET_MESHMANAGER.getSkeletalMesh(renderer.meshType);
					//glUniformMatrix4fv(shader.getBonesMatrixUniform(), animationMatrix.size(), GL_FALSE, &animationMatrix[0][0][0]);

					glBindVertexArray(mesh.vao);
					glDrawArrays(GL_TRIANGLES, 0, mesh.numVertices);
					glBindVertexArray(0);
					return;
				}

				engineECS::Mesh& mesh = GET_MESHMANAGER.getMesh(renderer.meshType);
				glBindVertexArray(mesh.vao);
				glDrawArrays(GL_TRIANGLES, 0, mesh.numVertices);
				glBindVertexArray(0);
			})))
			{
				std::cerr << "Failed to create rendering system!" << std::endl;
			}
		}
	}

	std::cout << "Game loop starting..." << std::endl;

	engineECS::FpsManager fpsManager = { &engineECS::OpenGLWrapper::getTime, nullptr };
	fpsManager.setTargetFramePerSecond(-1.);
	fpsManager.setDebugOptions(false, static_cast<int>(engineECS::FpsManager::SavedDeltaTimesCount));

	while (!wrapper.shouldCloseWindow())
	{
		fpsManager.update();

		wrapper.prepareFrame();

		GET_ENGINE.run(fpsManager.getDeltaTime());

		std::cout << body->getGlobalPose().p.y << std::endl;

		/*glUseProgram(GET_SHADERMANAGER.getCompiledShader(triangleProgram).getProgram());
		glBindVertexArray(GET_MESHMANAGER.getMesh(triangleMesh).vao);
		glDrawArrays(GL_TRIANGLES, 0, GET_MESHMANAGER.getMesh(triangleMesh).numVertices);*/

		wrapper.finalizeFrame();
	}

	return 0;
}