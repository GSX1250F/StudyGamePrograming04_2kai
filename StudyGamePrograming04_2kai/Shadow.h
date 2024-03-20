#pragma once
#include "Actor.h"
class Shadow : public Actor
{
public:
	Shadow(Game* game);
	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;

	class CircleComponent* GetCircle() { return cc; }
	class NavComponent* GetNav() { return nc; }

	Vector2 GetIndex();
	Vector2 GetIndex2();
	void SetDir(int n) { ndir = n; }
	void SetPath();


private:
	class AnimSpriteComponent* asc;
	class CircleComponent* cc;
	class NavComponent* nc;
	int ndir;	//進行方向　ndir ==0:RIGHT,1:UP,2:LEFT,3:DOWN
	bool turnLeftOk;		//左手が空いたとき、次に左が壁になるまでfalse
	
};

