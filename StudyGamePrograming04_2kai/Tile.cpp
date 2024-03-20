#include "Tile.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "CircleComponent.h"

Tile::Tile(class Game* game)
	:Actor(game)
	, mTileState(EDefault)
	, mParent(nullptr)
{
	//スプライトコンポーネント作成
	sc = new SpriteComponent(this , 10);
	SetTileState(EDefault);
	UpdateTexture();
	
	//CircleComponent作成
	cc = new CircleComponent(this);
	cc->SetRadius(sc->GetTexWidth() / 2.0f);
	
}

void Tile::SetTileState(TileState state)
{
	mTileState = state;
	UpdateTexture();
}

void Tile::UpdateTexture()
{
	std::string text;
	switch (mTileState)
	{
	case EDefault:
		text = "Assets/Default.png";
		break;

	case EPath:
		text = "Assets/Path.png";
		break;
	case EWall:
		text = "Assets/Wall.png";
		break;
	case EStart:
		text = "Assets/Default.png";
		break;
	case EGoal:
		text = "Assets/Treasure.png";
		break;
	}
	sc->SetTexture(GetGame()->GetTexture(text));
}