#pragma once
#include <cstdint>

class Component
{
public:
	// コンストラクタ
	// updateOrderが小さいコンポーネントほど早く更新される
	Component(class Actor* owner, int updateOrder = 100);
	
	// デストラクタ
	virtual ~Component();
	
	// このコンポーネントをデルタタイムで更新する
	virtual void Update(float deltaTime);

	// 各コンポーネント入力処理（オーバーライド可能）
	virtual void ProcessInput(const uint8_t* keyState) {}

	// 更新順ゲッター
	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	// 所有アクターへのポインタ
	class Actor* mOwner;
	
	// コンポーネントの更新順序
	int mUpdateOrder;
};