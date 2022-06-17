#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include <cassert>
#include "PlayerBullet.h"


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

	PlayerBullet* bullet_ = nullptr;
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

	
};

