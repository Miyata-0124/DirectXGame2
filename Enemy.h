#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "EnemyBullet.h"

enum class Phase {
	Approach, //接近
	Leave,	  //離脱
};

class Enemy
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 移動
	/// </summary>
	void Translation();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	void Approach();
	void Leave();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();
	//フェーズ
	Phase phase_ = Phase::Approach;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Vector3 apSpeed = { 0,0,-0.01f };
	Vector3 leSpeed = { 0.0f,0.0f,-0.01f };
};