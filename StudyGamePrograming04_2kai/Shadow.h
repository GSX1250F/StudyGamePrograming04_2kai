#pragma once
#include "Actor.h"
class Shadow : public Actor
{
public:
	Shadow(Game* game);
	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;

	class AnimSpriteComponent* asc;
	class CircleComponent* cc;
	class NavComponent* nc;

	class Tile* tile;
	void SetPath();

	float speed = 75.0f;

};

