#include "composite_model.h"

namespace hack_game {
	using glm::vec3;


	CompositeModel::CompositeModel(std::initializer_list<Model*> models):
			models(models) {}
	
	void CompositeModel::generateVertexArray() {}

	void CompositeModel::draw(DrawContext& context) const {
		for (const Model* model : models) {
			model->draw(context);
		}
	}

	void CompositeModel::draw(DrawContext& context, const vec3& color) const {
		for (const Model* model : models) {
			model->draw(context, color);
		}
	}
}
