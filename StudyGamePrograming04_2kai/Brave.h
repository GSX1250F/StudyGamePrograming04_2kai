#pragma once
#include "Actor.h"
class Brave : public Actor
{
public:
	Brave(Game* game);
	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;
	
	class CircleComponent* GetCircle() { return cc; }
	
private:
	class AnimSpriteComponent* asc;
	class CircleComponent* cc;
	
	
};

