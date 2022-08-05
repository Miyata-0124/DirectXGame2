#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include <cassert>
#include "PlayerBullet.h"
#include <memory>
#include <list>

class RailCamera;

/// <summary>
/// 自キャラ
/// </summary>
class Player
{
public:
	/// <summary>
	/// 初期化
	/// </summary>

	void Initialize(Model* model, uint32_t textureHandle_);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Translation();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//レールカメラ取得用Setter
	void SetCamera(WorldTransform* camera) { worldTransform_.parent_ = camera; }

	float GetRadius();
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	DebugText *debugText_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//半径
	const float radius_ = 1.0f;
};

