#pragma once
#include "Actor.h"
#include <vector>
class Tile : public Actor
{
public:
	//friend class Maze;	//friend classってなんじゃ～
	enum TileState
	{
		EDefault,
		EPath,
		EWall,
		EStart,
		EGoal
	};

	Tile(class Game* game);
	void SetTileState(TileState state);
	TileState GetTileState() const { return mTileState; }
	
	class CircleComponent* GetCircle() { return cc; }

	const Tile* GetParent() const { return mParent; }
	

private:
	void UpdateTexture();
	class SpriteComponent* sc;
	TileState mTileState;
	class CircleComponent* cc;
	std::vector<Tile*> mAdjacent;	//隣接ノード配列
	Tile* mParent;	//親ノード
	
};

