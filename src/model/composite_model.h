#ifndef HACK_GAME__MODEL__COMPOSITE_MODEL_H
#define HACK_GAME__MODEL__COMPOSITE_MODEL_H

#include "model.h"

namespace hack_game {
	
	class CompositeModel: public Model {
		std::vector<Model*> models;

	public:
		explicit CompositeModel(std::initializer_list<Model*> models);

		void generateVertexArray() override;
		void draw(DrawContext&) const override;
		void draw(DrawContext&, const glm::vec3& color) const override;
	};
}

#endif
