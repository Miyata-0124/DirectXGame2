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


/// <summary>
/// 自キャラ
/// </summary>
class Player
{
public:
	/// <summary>
	/// 初期化
	/// </summary>

	void Initialize(Model* model);
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
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;

	Input* input_ = nullptr;
	DebugText *debugText_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	
};

