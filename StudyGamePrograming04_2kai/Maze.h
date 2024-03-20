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


	int GetMapWidth() { return mapWidth; }
	int GetMapHeight() { return mapHeight; }
	void SetWidth(int width) { mapWidth = width; }
	void SetHeigth(int height) { mapHeight = height; }
	std::vector<std::vector<int>> GetMapIndex() { return mapIndex; }
	void SetMapIndex(int i, int j, int data) { mapIndex[i][j] = data; }

	// 迷路の各アクターの位置をリセット
	void InitMaze(bool gamestart);
	// マップを（再）構成
	void GenerateMap();
	// ゲームクリア
	bool gameClear = false;
	// ゲームスタート
	bool gameStart = false;
	Vector2 GetTilePos(int index[2]);
	
	class Tile* GetStartTile();
	class Tile* GetEndTile();

	std::vector<std::vector<int>> mapIndex;
	int mapWidth;
	int mapHeight;
	
	//スタート位置インデックス
	int sindex[2];
	//ゴール位置インデックス
	int gindex[2];	
	// タイルサイズ
	float mTileSize;
	
	// 経路探索用
	void MakeGraphNodes();
	bool FindPath(class Tile* start, class Tile* goal);
	void UpdatePathTiles(class Tile* start);
	std::unordered_map <Tile*, Tile* > outMap;	// ノードから親へのマップ

};

