#include "Shadow.h"
#include "Game.h"
#include "Maze.h"
#include "Tile.h"
#include "AnimSpriteComponent.h"
#include "CircleComponent.h"
#include "MoveComponent.h"
#include "Random.h"
#include "NavComponent.h"
#include <algorithm>
#include "Math.h"


Shadow::Shadow(Game* game) : Actor(game)
{
	SetPosition(Vector2(-100.0f, -100.0f));	//初期位置は画面外
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
	nc->SetForwardSpeed(150.0f);
	
	
	//左手法のとき。初期方向は上
	//ndir = 1;	//ndir%4 ==0:RIGHT,1:UP,2:LEFT,3:DOWN
	//turnLeftOk = true;
}

void Shadow::ActorInput(const uint8_t* keyState)
{
	if (GetGame()->GetMaze()->startOk == true) {
		//入力に応じて、アニメーションの設定と移動
		SetVelocity(Vector2::Zero);
		int bg = asc->GetAnimNumBeg();
		int ed = asc->GetAnimNumLast();
		if (ndir == 3)	//DOWN
		{
			if (bg != 1 || ed != 4) { asc->SetAnimNum(1, 4, true); }
			SetVelocity(Vector2(0.0f, 200.0f));
		}
		else if (ndir == 1)	//UP
		{
			if (bg != 5 || ed != 8) { asc->SetAnimNum(5, 8, true); }
			SetVelocity(Vector2(0.0f, -200.0f));
		}
		else if (ndir == 0)	//RIGHT
		{
			if (bg != 9 || ed != 12) { asc->SetAnimNum(9, 12, true); }
			SetVelocity(Vector2(200.0f, 0.0f));
		}
		else if (ndir == 2)	//LEFT
		{
			if (bg != 13 || ed != 16) { asc->SetAnimNum(13, 16, true); }
			SetVelocity(Vector2(-200.0f, 0.0f));
		}
	}
}

void Shadow::UpdateActor(float deltaTime)
{
	if (GetGame()->GetMaze()->startOk == true) {
		
		/*
		auto tiles = GetGame()->GetMaze()->GetTiles();
		for (int i = 0; i < tiles.size(); i++)
		{
			for (int j = 0; j < tiles[i].size(); j++)
			{
				if (tiles[i][j]->GetTileState() == Tile::EWall)
				{
					//壁衝突判定
					if (Intersect(*mCircle, *(tiles[i][j]->GetCircle())))
					{

						while (Intersect(*mCircle, *(tiles[i][j]->GetCircle())))
						{
							//位置を調整。差ベクトルのx,y成分の大きいほうを*とした分だけ離す
							Vector2 n = this->GetPosition() - tiles[i][j]->GetPosition();
							if (Math::Abs(n.x) >= Math::Abs(n.y)) { n = Vector2(n.x / Math::Abs(n.x), 0.0f); }
							else { n = 1.0*Vector2(0.0f, n.y / Math::Abs(n.y)); }
							this->SetPosition(this->GetPosition() + n);
						}
					}
				}
				//ゴール判定
				if (tiles[i][j]->GetTileState() == Tile::EGoal)
				{
					if (Intersect(*mCircle, *(tiles[i][j]->GetCircle())))
					{
						GetGame()->GetMaze()->SetGoal();
						break;
					}
				}

			}
		}

		// 左手法を実装。
		// 前が壁だったとき、右方向にVelocityを変更。
		// 左手が空いていたら左方向にVelocityを変更し、再び左手が壁になるまで前進。
		Vector2 index = GetIndex2();
		Vector2 indexDN = index - Vector2(static_cast<int>(index.x), static_cast<int>(index.y));
		if ((indexDN.x <= 0.25f || indexDN.x >= 0.75f)&& (indexDN.y <= 0.25f || indexDN.y >= 0.75f))
		{
			switch (ndir)
			{
				case 0:	//右向き
					//左が空いているとき
					if (tiles[std::round(index.x)][std::round(index.y) - 1]->GetTileState() != Tile::EWall)
					{
						if (turnLeftOk) {
							ndir = (ndir + 1) % 4;	//向かって左を向く 
							turnLeftOk = false;		//次に左が壁になるまでfalse
						}
					}
					// 左が壁のとき
					else {
						turnLeftOk = true;	//左が壁だったら次曲がって良し
						// 前方が壁のとき
						if (tiles[std::round(index.x) + 1][std::round(index.y)]->GetTileState() == Tile::EWall)
						{
							ndir = (ndir + 3) % 4;	//向かって右を向く 
						}
					}
					break;

				case 1:	//上向き
					//左が空いているとき
					if (tiles[std::round(index.x) - 1][std::round(index.y)]->GetTileState() != Tile::EWall)
					{
						if (turnLeftOk) {
							ndir = (ndir + 1) % 4;	//向かって左を向く 
							turnLeftOk = false;		//次に左が壁になるまでfalse
						}
					}
					// 左が壁のとき
					else {
						turnLeftOk = true;	//左が壁だったら次曲がって良し
						// 前方が壁のとき
						if (tiles[std::round(index.x)][std::round(index.y) - 1]->GetTileState() == Tile::EWall)
						{
							ndir = (ndir + 3) % 4;	//向かって右を向く 
						}
					}
					break;

				case 2:	//左向き
					//左が空いているとき
					if (tiles[std::round(index.x)][std::round(index.y) + 1]->GetTileState() != Tile::EWall)
					{
						if (turnLeftOk) {
							ndir = (ndir + 1) % 4;	//向かって左を向く 
							turnLeftOk = false;		//次に左が壁になるまでfalse
						}
					}
					// 左が壁のとき
					else {
						turnLeftOk = true;	//左が壁だったら次曲がって良し
						// 前方が壁のとき
						if (tiles[std::round(index.x) - 1][std::round(index.y)]->GetTileState() == Tile::EWall)
						{
							ndir = (ndir + 3) % 4;	//向かって右を向く 
						}
					}
					break;
				case 3:	//下向き
					//左が空いているとき
					if (tiles[std::round(index.x) + 1][std::round(index.y)]->GetTileState() != Tile::EWall)
					{
						if (turnLeftOk) {
							ndir = (ndir + 1) % 4;	//向かって左を向く 
							turnLeftOk = false;		//次に左が壁になるまでfalse
						}
					}
					// 左が壁のとき
					else {
						turnLeftOk = true;	//左が壁だったら次曲がって良し
						// 前方が壁のとき
						if (tiles[std::round(index.x)][std::round(index.y) + 1]->GetTileState() == Tile::EWall)
						{
							ndir = (ndir - 1) % 4;	//向かって右を向く 
						}
					}
					break;
			}
		}
		*/
	}
	



}

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
	float xindex = GetPosition().x / GetGame()->GetTiles()[0][0]->sc->GetTexWidth() - 1.0f;
	float yindex = GetPosition().y / GetGame()->GetTiles()[0][0]->sc->GetTexWidth() - 1.0f;
	return Vector2(xindex, yindex);	
}

void Shadow::SetPath()
{
	Tile* st = GetGame()->GetTiles()->GetStartTile();
	nc->StartPath(st);
}
