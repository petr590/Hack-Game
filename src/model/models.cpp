#include "models.h"
#include "frame_model.h"
#include "textured_model.h"
#include "composite_model.h"
#include "postprocessing_model.h"

namespace hack_game {
	namespace models {
		ColoredModel plane    (0xd00000, "plane.obj");
		ColoredModel platform (0xd0c8b5, "platform.obj");

		ColoredModel blackCube       (0x000000, "cube.obj");
		ColoredModel breakableCube   (0x41403B, blackCube);
		ColoredModel unbreakableCube (0xE0DAD1, blackCube);

		ColoredModel playerBase   (0xE0DAD1, "player/base.obj");
		ColoredModel playerCenter (0x41403B, "player/center.obj");
		ColoredModel playerLeft   (0xE0DAD1, "player/left.obj");
		ColoredModel playerRight  (0xE0DAD1, "player/right.obj");
		ColoredModel playerBullet (0xFFFFFF, "player-bullet.obj");

		ColoredModel sphere            (0x7A7876, "sphere.obj");
		ColoredModel breakableSphere   (0xF68118, sphere);
		ColoredModel unbreakableSphere (0x3E0065, sphere);

		ColoredModel minion (0x7d746e, "minion.obj");



		CompositeModel compositeModels[] = {
				CompositeModel { &playerBase, &playerCenter, &playerLeft, &playerRight },
				CompositeModel { &playerBase, &playerCenter, &playerLeft },
				CompositeModel { &playerBase, &playerCenter },
		};

		FrameModel frameModels[] = {
			FrameModel(0x000000, "cube-frame.obj"),
		};

		TexturedModel texturedModels[] = {
			TexturedModel("plane.obj", {"enemy-destroy-1.png", "enemy-destroy-2.png"}),
		};

		PostprocessingModel postprocessingModels[] = {
			PostprocessingModel(),
		};



		Model& player3hp = compositeModels[0];
		Model& player2hp = compositeModels[1];
		Model& player1hp = compositeModels[2];

		Model& cubeFrame = frameModels[0];
		Model& texturedPlane = texturedModels[0];
		Model& postprocessingModel = postprocessingModels[0];
	}
}