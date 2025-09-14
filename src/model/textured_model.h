#ifndef HACK_GAME__MODEL__TEXTURED_MODEL_H
#define HACK_GAME__MODEL__TEXTURED_MODEL_H

#include "vao_model.h"
#include <glm/vec2.hpp>

namespace hack_game {

	class TexturedModel: public VAOModel {
	public:
		struct Image;
		struct Vertex;

	private:
		std::vector<Image> images;
		std::vector<GLuint> textures;
		std::vector<Vertex> vertices;

	public:
		TexturedModel(const char* relativeModelPath, std::initializer_list<const char*> relativeTexturePaths);
		~TexturedModel() noexcept;

		void draw(Shader&) const override;

	protected:
		GLuint createVertexArray() override;
	};
}

#endif