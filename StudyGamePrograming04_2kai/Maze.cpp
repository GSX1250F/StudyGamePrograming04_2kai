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

Maze::Maze(Game* game) : Actor(game),mWidth(7),mHeight(7),gxindex(0),gyindex(0)
{
	//�����A�c�����Ƃ���7�ȏ�̊�ɂ���B
	while (mWidth < 7 || mWidth % 2 == 0) {	mWidth++;}
	while (mHeight < 7 || mHeight % 2 == 0) { mHeight++; }

	//�A�N�^�[���C���X�^���X
	brave = new Brave(GetGame());
	mazeclr = new MazeClr(GetGame());
	
	//AI����̃L�������C���X�^���X
	shadow = new Shadow(GetGame());
	
	//Tile���C���X�^���X
	mTiles.resize(mWidth);
	for (int i = 0; i < mWidth; i++)
	{
		mTiles[i].resize(mHeight);

		for (int j = 0; j < mHeight; j++)
		{
			mTiles[i][j] = new Tile(GetGame());
		}
	}
	TileSize = mTiles[0][0]->GetTexSize();
	
	
	//�}�b�v�����쐬
	GenerateMap();


	

}

void Maze::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_R]){	GenerateMap();	}
}

void Maze::UpdateActor(float deltaTime)
{
	if (goal)
	{
		mazeclr->SetState(EActive);
	}
}

void Maze::GenerateMap()
{
	//������
	startOk = false;
	goal = false;
	brave->SetState(EPaused);
	brave->SetPosition(Vector2(0.0f, 1000.0f));
	shadow->SetState(EPaused);
	shadow->SetPosition(Vector2(0.0f, 1000.0f));
	shadow->SetDir(1);
	mazeclr->SetState(EPaused);
	mazeclr->SetPosition(Vector2(1024.0f / 2.0f, 768.0f / 2.0f));
	for (int i = 0; i < mWidth; i++) {
		for (int j = 0; j < mHeight; j++) {
			mTiles[i][j]->SetState(EPaused);
			mTiles[i][j]->SetPosition(Vector2(0.0f, 1000.0f));
			mTiles[i][j]->GetCircle()->SetRadius(mTiles[i][j]->GetTexSize() / 2.0f);
		}
	}

	//���H�쐬
	while (true)
	{
		map = MazeCreate(mWidth, mHeight);
		//map = mz->GetMaze();
		for (int i = 0; i < mWidth; i++) {
			for (int j = 0; j < mHeight; j++) {
				if (map[i][j] == 2) {
					//�X�^�[�g�ʒu�C���f�b�N�X
					sxindex = i;
					syindex = j;
				}
				if (map[i][j] == 3) {
					//�S�[���ʒu�C���f�b�N�X
					gxindex = i;
					gyindex = j;
				}
			}
		}
		//�ȒP�����Ȃ����`�F�b�N
		if ((gxindex > static_cast<int>(mWidth / 2)) && (gyindex > static_cast<int>(mHeight / 2))) { break; }
	}

	//Tile�Ǝ�l����index��position���Z�b�g.Active��
	for (int i = 0; i < mWidth; i++) {
		for (int j = 0; j < mHeight; j++) {
			switch (map[i][j]) {
				case 0:
					//�ʘH
					mTiles[i][j]->SetTileState(Tile::EDefault);
					break;
				case 1:
					//��
					mTiles[i][j]->SetTileState(Tile::EWall);
					break;
				case 2:
					//�X�^�[�g
					mTiles[i][j]->SetTileState(Tile::EStart);
					brave->SetState(EActive);
					brave->SetPosition(mTiles[i][j]->GetTexSize() * Vector2((i + 1) * 1.0f, (j + 1) * 1.0f));
					shadow->SetState(EActive);
					shadow->SetPosition(mTiles[i][j]->GetTexSize() * Vector2((i + 1) * 1.0f, (j + 1) * 1.0f));
					break;
				case 3:
					//�S�[��
					mTiles[i][j]->SetTileState(Tile::EGoal);
					mTiles[i][j]->GetCircle()->SetRadius(2.0f);
					break;
			}
			mTiles[i][j]->SetState(EActive);
			mTiles[i][j]->SetPosition(mTiles[i][j]->GetTexSize() * Vector2((i + 1), (j + 1)));
		}

		

	}

	// Set start/end tiles
	GetStartTile()->SetTileState(Tile::EStart);
	GetEndTile()->SetTileState(Tile::EGoal);
	

	startOk = true;

	//�אڃm�[�h�쐬
	MakeGraphNodes();

	// Find path (in reverse)
	FindPath(GetEndTile(), GetStartTile());
	UpdatePathTiles(GetStartTile());

	auto st = GetStartTile();
	shadow->SetPath();
}

Tile* Maze::GetStartTile()
{
	return mTiles[sxindex][syindex];
}

Tile* Maze::GetEndTile()
{
	return mTiles[gxindex][gyindex];
}

void Maze::MakeGraphNodes()
{
	for (int i = 0; i < mTiles.size(); i++) {
		for (int j = 0; j < mTiles[i].size(); j++) {
			if (mTiles[i][j]->GetTileState() != Tile::EWall) {
				if (mTiles[i - 1][j]->GetTileState() != Tile::EWall) {
					mTiles[i][j]->mAdjacent.push_back(mTiles[i - 1][j]);
				}
				if (mTiles[i + 1][j]->GetTileState() != Tile::EWall) {
					mTiles[i][j]->mAdjacent.push_back(mTiles[i + 1][j]);
				}
				if (mTiles[i][j - 1]->GetTileState() != Tile::EWall){
					mTiles[i][j]->mAdjacent.push_back(mTiles[i][j - 1]);
				}
				if (mTiles[i][j + 1]->GetTileState() != Tile::EWall) {
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

	// �m�[�h����e�ւ̃}�b�v
	//std::unordered_map <const Tile*, const Tile* > outMap;
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