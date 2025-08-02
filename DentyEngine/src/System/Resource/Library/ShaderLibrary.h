#pragma once

#include "../ILibrary.h"

#include "System/Core/ID/ShaderID.h"

#include "System/Core/DString.h"

namespace DentyEngine
{
	class Event;

	class Shader;
	class ShaderLibrary final : public ILibrary
	{
	public:
		ShaderLibrary();
		~ShaderLibrary() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(ShaderLibrary)

		void Initialize() override;
		void Update() override;
		void OnGui() override;
		void OnEvent(Event* e) override;
		void Finalize() override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void Add(const Ref<Shader>& shader);

		void Remove(const FilePath& filePath);

		[[nodiscard]]
		std::optional<Ref<Shader>> Find(const FilePath& filePath) const;

		[[nodiscard]]
		bool Reload(const FilePath& path) override;
	public:
		//
		// Build in
		//
		static const std::string STATIC_MESH_SHADER_PATH;
		static const std::string SKINNED_MESH_SHADER_PATH;

		static const std::string STATIC_MESH_SHADOW_SHADER_PATH;
		static const std::string SKINNED_MESH_SHADOW_SHADER_PATH;

		static const std::string COLLISION_VISUALIZER_MESH_SHADER_PATH;

		static const std::string PHOTO_STATIC_MESH_SHADER_PATH;
		static const std::string PHOTO_SKINNED_MESH_SHADER_PATH;

		static const std::string MESH_TOON_SHADER_PATH;
		static const std::string PHOTO_MESH_SHADER_PATH;
		static const std::string POST_EFFECT_MESH_SHADER_PATH;

		static const std::string GRID_SHADER_PATH;
		static const std::string LINE_SHADER_PATH;
		static const std::string EXPANDED_IMAGE_SHADER_PATH;
		static const std::string DEBUG_DRAWER_PATH;
		static const std::string CPU_PARTICLE_SHADER_PATH;
		static const std::string SKYBOX_PATH;

		//
		// PostEffects
		//
		static const std::string FULL_SCREEN_QUAD_PATH;
		static const std::string GAUSSIAN_BLUR_HORIZONTAL_PATH;
		static const std::string GAUSSIAN_BLUR_VERTICAL_PATH;
		static const std::string BLOOM_PATH;

		//
		// Sprites
		//
		static const std::string SPRITE_SHADER_PATH;
		static const std::string SPRITE_3D_SHADER_PATH;

	private:
		[[nodiscard]]
		bool IsShaderFile(const String&) const;

	private:
		std::unordered_map<FilePath, Ref<Shader>, FilePath::Hash> _caches;
	};
};