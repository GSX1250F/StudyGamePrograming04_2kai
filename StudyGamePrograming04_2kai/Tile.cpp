#include "Tile.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "CircleComponent.h"

Tile::Tile(class Game* game)
	:Actor(game)
	, mSprite(nullptr)
	, mTileState(EDefault)
	, mParent(nullptr)
	, f(0.0f)
	, g(0.0f)
	, h(0.0f)
{
	mSprite = new SpriteComponent(this , 10);
	SetScale(0.8f);
	SetState(EPaused);
	SetTileState(EDefault);
	UpdateTexture();
	mTexSize = mSprite->GetTexWidth() * GetScale();
	
	//CircleComponentì¬
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(mTexSize / 2.0f);
	
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
	mSprite->SetTexture(GetGame()->GetTexture(text));
}