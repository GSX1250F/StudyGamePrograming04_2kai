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
	class SpriteComponent* sc;
	class CircleComponent* cc;

	TileState mTileState;
	void SetTileState(TileState);
	float mTexSize;

	std::vector<Tile*> mAdjacent;	//�אڃm�[�h�z��
	Tile* mParent;	//�e�m�[�h
	void UpdateTexture();
	
	
};

