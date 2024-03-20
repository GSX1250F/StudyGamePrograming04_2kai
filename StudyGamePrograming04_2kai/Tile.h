#pragma once
#include "Actor.h"
#include <vector>
class Tile : public Actor
{
public:
	//friend class Maze;	//friend class���ĂȂ񂶂�`
	enum TileState
	{
		EDefault,
		EPath,
		EWall,
		EStart,
		EGoal
	};

	Tile(class Game* game);
	void UpdateActor(float deltaTime) override;

	class AnimSpriteComponent* asc;
	class CircleComponent* cc;

	TileState mTileState;
	void SetTileState(TileState);
	float mTexSize;

	std::vector<Tile*> mAdjacent;	//�אڃm�[�h�z��
	Tile* mParent;	//�e�m�[�h
	void UpdateTexture();
	
	
};

