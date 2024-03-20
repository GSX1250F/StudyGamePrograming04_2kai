#pragma once
#include "Actor.h"
class Treasure : public Actor
{
public:
	Treasure(class Game* game);
	class SpriteComponent* sc;
	class CircleComponent* cc;
};

