#ifndef HACK_GAME__MODEL__VAO_MODEL_H
#define HACK_GAME__MODEL__VAO_MODEL_H

#include "model.h"

namespace hack_game {

	class VAOModel: public Model {
	protected:
		std::vector<GLuint> indices;
		GLuint vertexArray = 0;
	
	public:
		VAOModel() noexcept;
		VAOModel(const VAOModel&);

		void generateVertexArray() override;
		void draw(Shader&) const override;
	
	protected:
		virtual GLuint createVertexArray() = 0;
	};
}

#endif