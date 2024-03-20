#include "Maze.h"
#include "MazeCreator.h"
#include "Game.h"
#include "Brave.h"
#include "MazeClr.h"
#include "Shadow.h"
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
		InitMaze(false);
		GenerateMap();	
		InitMaze(true);
	}
}

void Maze::UpdateActor(float deltaTime){ }

void Maze::InitMaze(bool gamestart)
{
	GetGame()->mazeClr->SetPosition(Vector2(GetGame()->mWindowWidth / 2.0f, 2 * GetGame()->mWindowHeight));	//初期位置は画面外
	gameClear = false;
	if (gamestart = false) {
		// 迷路作成中
		// 各アクターを迷路作成中は画面外におく。
		GetGame()->brave->SetPosition(Vector2(-100.0f, -100.0f));	//初期位置は画面外
		GetGame()->shadow->SetPosition(Vector2(-100.0f, -100.0f));	//初期位置は画面外
		GetGame()->shadow->SetDir(1);
		for (int i = 0; i < mapWidth; i++) {
			for (int j = 0; j < mapHeight; j++) {
				GetGame()->tiles[i][j]->SetTileState(Tile::EDefault);
				GetGame()->tiles[i][j]->SetPosition(Vector2(-100.0f, -100.0f));
				mapIndex[i][j] = 0;
			}
		}
	}
	else {
		// 迷路作成完了
		// 各アクターの初期位置を設定
		GetGame()->brave->SetPosition(GetTilePos(sindex));
		GetGame()->shadow->SetPosition(GetTilePos(sindex));
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
				if (mapIndex[i][j] == 2) {
					//スタート位置インデックス
					sindex[0] = i;
					sindex[1] = j;
				}
				if (mapIndex[i][j] == 3) {
					//ゴール位置インデックス
					gindex[0] = i;
					gindex[1] = j;
				}
			}
		}
		//簡単すぎないかチェック
		if ((gindex[0] > static_cast<int>(mapWidth / 2)) && (gindex[1] > static_cast<int>(mapHeight / 2))) { mazeNG = false; }
	}
	gameStart = true;

	//隣接ノード作成
	MakeGraphNodes();

	// Find path (in reverse)
	FindPath(GetEndTile(), GetStartTile());
	UpdatePathTiles(GetStartTile());

	GetGame()->shadow->SetPath();
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

void Maze::MakeGraphNodes()
{
	auto mTiles = GetGame()->tiles;
	for (int i = 0; i < mTiles.size(); i++) {
		for (int j = 0; j < mTiles[i].size(); j++) {
			if (mTiles[i][j]->mTileState != Tile::EWall) {
				if (mTiles[i - 1][j]->mTileState != Tile::EWall) {
					mTiles[i][j]->mAdjacent.push_back(mTiles[i - 1][j]);
				}
				if (mTiles[i + 1][j]->mTileState != Tile::EWall) {
					mTiles[i][j]->mAdjacent.push_back(mTiles[i + 1][j]);
				}
				if (mTiles[i][j - 1]->mTileState != Tile::EWall){
					mTiles[i][j]->mAdjacent.push_back(mTiles[i][j - 1]);
				}
				if (mTiles[i][j + 1]->mTileState != Tile::EWall) {
					mTiles[i][j]->mAdjacent.push_back(mTiles[i][j + 1]);
				}

			}
		}
	}
}


bool Maze::FindPath(Tile* start, Tile* goal)
{
	/*
	// Implements A* pathfinding
	for (int i = 0; i < mWidth; i++)
	{
		for (int j = 0; j < mHeight; j++)
		{
			mTiles[i][j]->g = 0.0f;
			mTiles[i][j]->mInOpenSet = false;
			mTiles[i][j]->mInClosedSet = false;
		}
	}

	std::vector<Tile*> openSet;

	// Set current node to start, and add to closed set
	Tile* current = start;
	current->mInClosedSet = true;

	do
	{
		// Add adjacent nodes to open set
		for (Tile* neighbor : current->mAdjacent)
		{
			if (neighbor->mBlocked)
			{
				continue;
			}

			// Only check nodes that aren't in the closed set
			if (!neighbor->mInClosedSet)
			{
				if (!neighbor->mInOpenSet)
				{
					// Not in the open set, so set parent
					neighbor->mParent = current;
					neighbor->h = (neighbor->GetPosition() - goal->GetPosition()).Length();
					// g(x) is the parent's g plus cost of traversing edge
					neighbor->g = current->g + TileSize;
					neighbor->f = neighbor->g + neighbor->h;
					openSet.emplace_back(neighbor);
					neighbor->mInOpenSet = true;
				}
				else
				{
					// Compute g(x) cost if current becomes the parent
					float newG = current->g + TileSize;
					if (newG < neighbor->g)
					{
						// Adopt this node
						neighbor->mParent = current;
						neighbor->g = newG;
						// f(x) changes because g(x) changes
						neighbor->f = neighbor->g + neighbor->h;
					}
				}
			}
		}

		// If open set is empty, all possible paths are exhausted
		if (openSet.empty())
		{
			break;
		}

		// Find lowest cost node in open set
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[](Tile* a, Tile* b) {
				return a->f < b->f;
			});
		// Set to current and move from open to closed
		current = *iter;
		openSet.erase(iter);
		current->mInOpenSet = false;
		current->mInClosedSet = true;
	} while (current != goal);

	// Did we find a path?
	return (current == goal) ? true : false;
	*/


	// Implements BFS pathfinding

	// ノードから親へのマップ
	//std::unordered_map <const Tile*, const Tile* > outMap;
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