#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>

//自機クラスの前方宣言
class Player;
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
	void AppInitialize();
	void Leave();

	void SetPlayer(Player* player) { player_ = player; }
	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	
	//フェーズ
	Phase phase_ = Phase::Approach;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//発射間隔
	static const int kFireInterval = 30;

	//ワールド座標を取得
	Vector3 GetWorldPosition();
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//自キャラ
	Player* player_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Vector3 apSpeed = { 0,0,-0.01f };
	Vector3 leSpeed = { 0.0f,0.0f,-0.01f };

	int32_t bulletTimer = 0;
};