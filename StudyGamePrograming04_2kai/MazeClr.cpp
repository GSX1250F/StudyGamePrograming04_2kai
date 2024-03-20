#include "MazeClr.h"
#include "SpriteComponent.h"
#include "Game.h"

MazeClr::MazeClr(Game* game) : Actor(game)
{
	SetPosition(Vector2(GetGame()->mWindowWidth / 2.0f , 2*GetGame()->mWindowHeight));	//初期位置は画面外
	//スプライトコンポーネント作成、テクスチャ設定
	SpriteComponent* sc = new SpriteComponent(this, 100);
	sc->SetTexture(game->GetTexture("Assets/MazeClr.png"));
}
