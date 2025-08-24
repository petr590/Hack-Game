#include "model.h"

namespace hack_game {

	std::vector<Model*> Model::models;

	Model::Model() {
		models.push_back(this);
	}
}
