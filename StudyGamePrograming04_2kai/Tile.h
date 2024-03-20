#pragma once
#include "Actor.h"
#include <vector>
class Tile : public Actor
{
public:
	friend class Maze;
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
	float GetTexSize() { return mTexSize; }

	class CircleComponent* GetCircle() { return mCircle; }

	const Tile* GetParent() const { return mParent; }

private:
	void UpdateTexture();
	class SpriteComponent* mSprite;
	TileState mTileState;
	float mTexSize;
	class CircleComponent* mCircle;
	std::vector<Tile*> mAdjacent;	//ó◊ê⁄ÉmÅ[ÉhîzóÒ
	Tile* mParent;
	float f;
	float g;
	float h;
	bool mInOpenSet;
	bool mInClosedSet;
	bool mBlocked;
};

