#include "Shadow.h"
#include "Brave.h"
#include "Game.h"
#include "Maze.h"
#include "Tile.h"
#include "Treasure.h"
#include "AnimSpriteComponent.h"
#include "CircleComponent.h"
#include "MoveComponent.h"
#include "Random.h"
#include "NavComponent.h"
#include <algorithm>
#include "Math.h"


Shadow::Shadow(Game* game) : Actor(game)
{
	// アニメーションのスプライトコンポーネントを作成
	asc = new AnimSpriteComponent(this, 40);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Shadow01.png"),
		game->GetTexture("Assets/Shadow02.png"),
		game->GetTexture("Assets/Shadow03.png"),
		game->GetTexture("Assets/Shadow04.png"),
		game->GetTexture("Assets/Shadow05.png"),
		game->GetTexture("Assets/Shadow06.png"),
		game->GetTexture("Assets/Shadow07.png"),
		game->GetTexture("Assets/Shadow08.png"),
		game->GetTexture("Assets/Shadow09.png"),
		game->GetTexture("Assets/Shadow10.png"),
		game->GetTexture("Assets/Shadow11.png"),
		game->GetTexture("Assets/Shadow12.png"),
		game->GetTexture("Assets/Shadow13.png"),
		game->GetTexture("Assets/Shadow14.png"),
		game->GetTexture("Assets/Shadow15.png"),
		game->GetTexture("Assets/Shadow16.png")
	};
	asc->SetAnimTextures(anims, 1, 4, true);
	
	//CircleComponent作成
	cc = new CircleComponent(this);
	cc->SetRadius(asc->GetTexWidth() / 2.0f);

	//MoveComponent作成
	//MoveComponent* mc = new MoveComponent(this);

	//NavComponent作成
	nc = new NavComponent(this);
	nc->SetForwardSpeed(speed);
	
}

void Shadow::ActorInput(const uint8_t* keyState)
{
	if (GetGame()->maze->gameStart == true) {
		//入力に応じて、アニメーションの設定と移動
		int bg = asc->GetAnimNumBeg();
		int ed = asc->GetAnimNumLast();
		float driftang = GetDriftAngle();
		if ((driftang >= -Math::Pi *0.75f) && (driftang < -Math::Pi * 0.25f))	//DOWN
		{
			if (bg != 1 || ed != 4) { asc->SetAnimNum(1, 4, true); }
		}
		else if ((driftang >= Math::Pi * 0.25f) && (driftang < Math::Pi * 0.75f))	//UP
		{
			if (bg != 5 || ed != 8) { asc->SetAnimNum(5, 8, true); }
		}
		else if ((driftang >= -Math::Pi * 0.25f) && (driftang < Math::Pi * 0.25f))	//RIGHT
		{
			if (bg != 9 || ed != 12) { asc->SetAnimNum(9, 12, true); }
		}
		else if (((driftang >= -Math::Pi * 0.75f) && (driftang < Math::Pi)) || ((driftang >= -Math::Pi) && (driftang < -Math::Pi *0.75)))	//LEFT
		{
			if (bg != 13 || ed != 16) { asc->SetAnimNum(13, 16, true); }
		}

		if (keyState[SDL_SCANCODE_R]) {
			//迷路をリセットするとき、スピードを上げる。
			speed += 5.0f;
			if (speed > GetGame()->brave->speed * 0.9f) { speed = GetGame()->brave->speed * 0.9f; }
			nc->SetForwardSpeed(speed);
		}
	}
	
}

void Shadow::UpdateActor(float deltaTime)
{
	if (GetGame()->maze->gameStart == true) {
		if (GetGame()->maze->gameStart == true) {
			std::vector<std::vector<Tile*>> tiles = GetGame()->tiles;
			for (int i = 0; i < tiles.size(); i++) {
				for (int j = 0; j < tiles[i].size(); j++) {
					if (tiles[i][j]->mTileState == Tile::EWall) {
						//壁衝突判定
						if (Intersect(*cc, *(tiles[i][j]->cc))) {

							while (Intersect(*cc, *(tiles[i][j]->cc))) {
								//位置を調整。差ベクトルのx,y成分の大きいほうを1とした分だけ離す
								Vector2 n = this->GetPosition() - tiles[i][j]->GetPosition();
								if (Math::Abs(n.x) >= Math::Abs(n.y)) { n = Vector2(n.x / Math::Abs(n.x), 0.0f); }
								else { n = Vector2(0.0f, n.y / Math::Abs(n.y)); }
								this->SetPosition(this->GetPosition() + n);
							}
						}
					}
				}
			}

			
		}
		
		//ゴール判定
		if (Intersect(*cc, *(GetGame()->maze->GetEndTile()->cc))) {
			GetGame()->maze->gameClear = true;
		}
	}
	

	
}
/*
Vector2 Shadow::GetIndex()
{
	//四捨五入版
	int xindex = std::round(GetPosition().x / asc->GetTexWidth()) - 1;
	int yindex = std::round(GetPosition().y / asc->GetTexWidth()) - 1;
	return Vector2(xindex * 1.0f, yindex * 1.0f);	//Vector2はfloatなのでわざとfloatとする。
}

Vector2 Shadow::GetIndex2()
{
	//indexのfloat値
	float xindex = GetPosition().x / GetGame()->tiles[0][0]->asc->GetTexWidth() - 1.0f;
	float yindex = GetPosition().y / GetGame()->tiles[0][0]->asc->GetTexWidth() - 1.0f;
	return Vector2(xindex, yindex);	
}
*/
void Shadow::SetPath()
{
	nc->StartPath(GetGame()->maze->GetStartTile());
}

