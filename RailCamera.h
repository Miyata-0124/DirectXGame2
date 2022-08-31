#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DebugText.h"
class Enemy;

class RailCamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& rotation);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void OnCollision();

	//ワールド座標を取得
	Vector3 GetWorldPosition();
	/// <summary>
	/// ビュープロジェクション
	/// </summary>
	/// <returns></returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	// ワールド行列を取得
	WorldTransform* GetWorldMatrix() { return &worldTransform_; }

	//ゲッター
	bool IsDead() const { return isDead_; }

	float GetRadius();

	int32_t Hp = 10;

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	//半径
	const float radius_ = 1;
	
	//デスフラグ
	bool isDead_ = false;
};
