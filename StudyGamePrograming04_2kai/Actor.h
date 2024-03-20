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
	const Vector2& GetVelocity() const { return mVelocity; }		//速度ゲッターを追加
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	void SetVelocity(const Vector2& vel) { mVelocity = vel; }		//速度セッターを追加
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	Vector2 GetForward() const { return Vector2(Math::Cos(mSpin), -Math::Sin(mSpin)); }		//単位ベクトル方向を取得

	float GetSpin() const { return mSpin; }				//スピンゲッターを追加
	void SetSpin(float spin) { mSpin = spin; }			//スピンセッターを追加


	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }


	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	// アクターの状態
	State mState;

	// 移動
	Vector2 mPosition;		//位置x,y
	Vector2 mVelocity;		//速度x,y
	float mScale;			//大きさ
	float mRotation;		//運動方向 (rad)

	// 回転のみ
	float mSpin;			//回転方向 (rad)

	std::vector<class Component*> mComponents;
	class Game* mGame;
};
