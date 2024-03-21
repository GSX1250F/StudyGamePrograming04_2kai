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
	//�����A�c�����Ƃ���7�ȏ�̊�ɂ���B
	while (mapWidth < 7 || mapWidth % 2 == 0) {	mapWidth++;}
	while (mapHeight < 7 || mapHeight % 2 == 0) { mapHeight++; }
}

void Maze::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_R]){
		// ���H�č\��
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
	// ���H�쐬
	InitMaze(false);
	GenerateMap();
	InitMaze(true);
	//�אڃm�[�h�쐬
	MakeGraphNodes(GetGame()->tiles);

	// Find path (in reverse)
	FindPath(GetEndTile(), GetStartTile());
	UpdatePathTiles(GetStartTile());

	GetGame()->shadow->SetPath();
}

void Maze::InitMaze(bool gamestart)
{
	if (gamestart == false) {
		// ���H�쐬��
		GetGame()->mazeClr->SetPosition(Vector2(GetGame()->mWindowWidth / 2.0f, 2 * GetGame()->mWindowHeight));	//�����ʒu�͉�ʊO
		gameClear = false;
		// �e�A�N�^�[����H�쐬���͉�ʊO�ɂ����B
		GetGame()->brave->SetPosition(Vector2(-100.0f, -100.0f));	//�����ʒu�͉�ʊO
		GetGame()->shadow->SetPosition(Vector2(-100.0f, -100.0f));	//�����ʒu�͉�ʊO
		GetGame()->treasure->SetPosition(Vector2(-100.0f, -100.0f));	//�����ʒu�͉�ʊO
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
		// ���H�쐬����
		// �e�A�N�^�[�̏����ʒu��ݒ�
		GetGame()->brave->SetPosition(GetTilePos(sindex));
		GetGame()->shadow->SetPosition(GetTilePos(sindex));
		GetGame()->treasure->SetPosition(GetTilePos(gindex));
		for (int i = 0; i < mapWidth; i++) {
			for (int j = 0; j < mapHeight; j++) {
				int index[2] = {i , j};
				GetGame()->tiles[i][j]->SetPosition(GetTilePos(index));
				switch (mapIndex[i][j]) {
					case 1: //��
						GetGame()->tiles[i][j]->SetTileState(Tile::EWall);
						break;
					default:	//�ǈȊO
						GetGame()->tiles[i][j]->SetTileState(Tile::EDefault);
				}
			}
		}
	}
}

void Maze::GenerateMap()
{
	//���H�쐬
	bool mazeNG = true;
	while (mazeNG)
	{
		mapIndex = MazeCreate(mapWidth, mapHeight);
		
		for (int i = 0; i < mapWidth; i++) {
			for (int j = 0; j < mapHeight; j++) {
				switch (mapIndex[i][j]) {
					case 2:		//�X�^�[�g
						sindex[0] = i;
						sindex[1] = j;
						break;
					case 3:		//�S�[��
						gindex[0] = i;
						gindex[1] = j;
						break;
				}
			}
		}
		//�ȒP�����Ȃ����`�F�b�N
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

	// �m�[�h����e�ւ̃}�b�v
	std::unordered_map <Tile*, Tile* > outMap;
	//�o�H�����������H
	bool pathFound = false;
	// ��������m�[�h
	std::queue<Tile*> q;
	// �ŏ��̃m�[�h���L���[�ɂ����B
	q.emplace(start);

	while (!q.empty())
	{
		// �m�[�h���P�L���[����o��
		Tile* current = q.front();
		q.pop();
		if (current == goal)
		{
			pathFound = true;
			break;
		}

		// �܂��L���[�ɓ����Ă��Ȃ��אڃm�[�h���G���L���[����B
		for (Tile* node : current->mAdjacent)
		{
			// �������̃m�[�h�̗אڃm�[�h�̂��ׂĂɂ��Đe�����邩�𒲂ׂ�B
			// outMap�́A�m�[�h�Ɛe�̊֌W�}�b�v
			Tile* parent = outMap[node];
			// �����e��nullptr�Ȃ�A�܂��L���[�ɒǉ�����Ă��Ȃ��B
			// (�������J�n�m�[�h�͗�O)
			if (parent == nullptr && node != start)
			{
				// ���̃m�[�h�̃G���L���[�Ɛe�̐ݒ������B
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