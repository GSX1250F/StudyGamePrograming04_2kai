#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>

class Actor
{
public:
	enum State
	{
		EActive,		//稼働中
		EPaused,		//更新停止中
		EDead			//削除対象
	};

	Actor(class Game* game);
	virtual ~Actor();

	// ゲームから呼び出される更新関数(オーバーライド不可)
	void Update(float deltaTime);
	// アクターが持つ全コンポーネントを更新(オーバーライド不可)
	void UpdateComponents(float deltaTime);
	// アクター独自の更新処理(オーバーライド可能)
	virtual void UpdateActor(float deltaTime);

	// ゲームから呼び出されるProcess Input(オーバーライド可能)
	virtual void ProcessInput(const uint8_t* keyState);
	// アクター独自の入力処理(オーバーライド可能)
	virtual void ActorInput(const uint8_t* keyState);

	// Getters/setters
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	Vector2 GetForward() const { return Vector2(Math::Cos(mRotation + mDriftAngle), -Math::Sin(mRotation + mDriftAngle)); }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }


	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	//オリジナル
	//Vector2 GetDriftForward() const { return Vector2(Math::Cos(mDriftAngle), -Math::Sin(mDriftAngle)); }
	float GetDriftAngle() const { return mDriftAngle; }
	void SetDriftAngle(float angle) { mDriftAngle = angle; }
private:
	// アクターの状態
	State mState;

	// 移動
	Vector2 mPosition;		//位置x,y
	float mScale;			//大きさ
	float mRotation;		//スプライト方向 (rad)
	
	std::vector<class Component*> mComponents;
	class Game* mGame;

	//オリジナル
	float mDriftAngle;		//スプライト方向を変えないアクター方向（rad）

};
