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

	Vector2 GetIndex();
	Vector2 GetIndex2();
	//void SetDir(int n) { ndir = n; }
	class Tile* tile;
	void SetPath();

	// for NavComponent
	bool turnLeftOk;		//左手法用。左手が空いたとき、次に左が壁になるまでfalse
	int nindex[2];			//次に向かうタイルのインデックス
};

