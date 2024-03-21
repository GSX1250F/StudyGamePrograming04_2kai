#include "Maze.h"
#include "MazeCreator.h"
#include "Game.h"
#include "Brave.h"
#include "MazeClr.h"
#include "Shadow.h"
#include "Treasure.h"
#include "Tile.h"
#include "CircleComponent.h"
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <vector>

Maze::Maze(Game* game) : Actor(game)
{
	mapWidth = 7;
	mapHeight = 7;
	//横幅、縦幅をともに7以上の奇数にする。
	while (mapWidth < 7 || mapWidth % 2 == 0) {	mapWidth++;}
	while (mapHeight < 7 || mapHeight % 2 == 0) { mapHeight++; }
}

void Maze::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_R]){
		// 迷路再構成
		ResetMaze();
	}
}

void Maze::UpdateActor(float deltaTime){
	if (gameClear == true) {
		GetGame()->mazeClr->SetPosition(Vector2(GetGame()->mWindowWidth / 2.0f, GetGame()->mWindowHeight / 2.0f));
	}
}

void Maze::ResetMaze()
{
	// 迷路作成
	InitMaze(false);
	GenerateMap();
	InitMaze(true);
	//隣接ノード作成
	MakeGraphNodes(GetGame()->tiles);

	// Find path (in reverse)
	FindPath(GetEndTile(), GetStartTile());
	UpdatePathTiles(GetStartTile());

	GetGame()->shadow->SetPath();
}

void Maze::InitMaze(bool gamestart)
{
	if (gamestart == false) {
		// 迷路作成中
		GetGame()->mazeClr->SetPosition(Vector2(GetGame()->mWindowWidth / 2.0f, 2 * GetGame()->mWindowHeight));	//初期位置は画面外
		gameClear = false;
		// 各アクターを迷路作成中は画面外におく。
		GetGame()->brave->SetPosition(Vector2(-100.0f, -100.0f));	//初期位置は画面外
		GetGame()->shadow->SetPosition(Vector2(-100.0f, -100.0f));	//初期位置は画面外
		GetGame()->treasure->SetPosition(Vector2(-100.0f, -100.0f));	//初期位置は画面外
		//GetGame()->shadow->SetDir(1);
		for (int i = 0; i < mapWidth; i++) {
			for (int j = 0; j < mapHeight; j++) {
				GetGame()->tiles[i][j]->SetTileState(Tile::EDefault);
				GetGame()->tiles[i][j]->SetPosition(Vector2(-100.0f, -100.0f));
				GetGame()->tiles[i][j]->mAdjacent.clear();
				GetGame()->tiles[i][j]->mParent = nullptr;
				mapIndex[i][j] = 0;
			}
		}
	}
	else {
		// 迷路作成完了
		// 各アクターの初期位置を設定
		GetGame()->brave->SetPosition(GetTilePos(sindex));
		GetGame()->shadow->SetPosition(GetTilePos(sindex));
		GetGame()->treasure->SetPosition(GetTilePos(gindex));
		for (int i = 0; i < mapWidth; i++) {
			for (int j = 0; j < mapHeight; j++) {
				int index[2] = {i , j};
				GetGame()->tiles[i][j]->SetPosition(GetTilePos(index));
				switch (mapIndex[i][j]) {
					case 1: //壁
						GetGame()->tiles[i][j]->SetTileState(Tile::EWall);
						break;
					default:	//壁以外
						GetGame()->tiles[i][j]->SetTileState(Tile::EDefault);
				}
			}
		}
	}
}

void Maze::GenerateMap()
{
	//迷路作成
	bool mazeNG = true;
	while (mazeNG)
	{
		mapIndex = MazeCreate(mapWidth, mapHeight);
		
		for (int i = 0; i < mapWidth; i++) {
			for (int j = 0; j < mapHeight; j++) {
				switch (mapIndex[i][j]) {
					case 2:		//スタート
						sindex[0] = i;
						sindex[1] = j;
						break;
					case 3:		//ゴール
						gindex[0] = i;
						gindex[1] = j;
						break;
				}
			}
		}
		//簡単すぎないかチェック
		if ((gindex[0] > static_cast<int>(mapWidth / 2)) && (gindex[1] > static_cast<int>(mapHeight / 2))) { mazeNG = false; }
	}
	gameStart = true;
}

Vector2 Maze::GetTilePos(int index[2])
{
	Vector2 pos;
	pos = mTileSize * Vector2((index[0] + 1) * 1.0f, (index[1] + 1) * 1.0f);
	return pos;
}

Tile* Maze::GetStartTile()
{
	Tile* tile = GetGame()->tiles[sindex[0]][sindex[1]];
	return tile;
}

Tile* Maze::GetEndTile()
{
	Tile* tile = GetGame()->tiles[gindex[0]][gindex[1]];
	return tile;
}

void Maze::MakeGraphNodes(std::vector<std::vector<Tile*>> &tiles)
{
	for (int i = 0; i < tiles.size(); i++) {
		for (int j = 0; j < tiles[i].size(); j++) {
			if (tiles[i][j]->mTileState != Tile::EWall) {
				if (tiles[i - 1][j]->mTileState != Tile::EWall) {
					tiles[i][j]->mAdjacent.push_back(tiles[i - 1][j]);
				}
				if (tiles[i + 1][j]->mTileState != Tile::EWall) {
					tiles[i][j]->mAdjacent.push_back(tiles[i + 1][j]);
				}
				if (tiles[i][j - 1]->mTileState != Tile::EWall) {
					tiles[i][j]->mAdjacent.push_back(tiles[i][j - 1]);
				}
				if (tiles[i][j + 1]->mTileState != Tile::EWall) {
					tiles[i][j]->mAdjacent.push_back(tiles[i][j + 1]);
				}

			}
		}
	}
}

bool Maze::FindPath(Tile* start, Tile* goal)
{
	// Implements BFS pathfinding

	// ノードから親へのマップ
	std::unordered_map <Tile*, Tile* > outMap;
	//経路を見つけたか？
	bool pathFound = false;
	// 検討するノード
	std::queue<Tile*> q;
	// 最初のノードをキューにいれる。
	q.emplace(start);

	while (!q.empty())
	{
		// ノードを１つキューから出す
		Tile* current = q.front();
		q.pop();
		if (current == goal)
		{
			pathFound = true;
			break;
		}

		// まだキューに入っていない隣接ノードをエンキューする。
		for (Tile* node : current->mAdjacent)
		{
			// 検討中のノードの隣接ノードのすべてについて親があるかを調べる。
			// outMapは、ノードと親の関係マップ
			Tile* parent = outMap[node];
			// もし親がnullptrなら、まだキューに追加されていない。
			// (ただし開始ノードは例外)
			if (parent == nullptr && node != start)
			{
				// このノードのエンキューと親の設定をする。
				outMap[node] = current;
				node->mParent = current;
				q.emplace(node);
			}
		}
		
	}

	return pathFound;

}

void Maze::UpdatePathTiles(class Tile* start)
{
	Tile* t = start->mParent;
	while (t != GetEndTile())
	{
		t->SetTileState(Tile::EPath);
		t = t->mParent;
	}
}