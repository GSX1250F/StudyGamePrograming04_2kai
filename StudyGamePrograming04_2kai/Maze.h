#pragma once
#include <vector>
#include "Actor.h"
#include <unordered_map>

class Maze : public Actor
{
public:
	Maze(class Game* game);
	void ActorInput(const uint8_t* keyState) override;
	void UpdateActor(float deltaTime) override;


	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; }
	void SetWidth(int width) { mWidth = width; }
	void SetHeigth(int height) { mHeight = height; }
	std::vector<std::vector<int>> GetMap() { return map; }
	std::vector<std::vector<class Tile*>> GetTiles() { return mTiles; }
	void GenerateMap();
	void SetGoal() { goal = true; };
	bool startOk = false;

	class Tile* GetStartTile();
	class Tile* GetEndTile();
	
	// Use A* to find a path
	bool FindPath(class Tile* start, class Tile* goal);

private:
	std::vector<std::vector<int>> map;
	std::vector<std::vector<class Tile*>> mTiles;
	class Brave* brave;
	//class MazeGenerator* mz;
	class MazeClr* mazeclr;
	class Shadow* shadow;
	int mWidth;
	int mHeight;
	float TileSize;
	float mBraveSize;
	//スタート位置インデックス
	int sxindex;
	int syindex;
	//ゴール位置インデックス
	int gxindex;	
	int gyindex;
	bool goal = false;
	void MakeGraphNodes();


	// Update textures for tiles on path
	void UpdatePathTiles(class Tile* start);

	// ノードから親へのマップ
	std::unordered_map <Tile*, Tile* > outMap;

};

