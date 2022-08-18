#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>

class Player;



class Enemy
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	void Approach();
	void AppInitialize();
	void Attack();
	void Leave();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	void OnCollision();

	void SetPlayer(Player* player) { player_ = player; }
	

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//発射間隔
	static const int kFireInterval = 30;

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	float GetRadius();
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;

	Vector3 apSpeed = { 0, 0, -0.05f };
	Vector3 leSpeed = { -0.1f,0.1f,-0.5f };

	int32_t bulletTimer = 0;
	Player* player_ = nullptr;

	//半径
	const float radius_ = 1.0f;

	enum class Phase {
		Approach, //接近
		Attack,	  //攻撃
		Leave,	  //離脱
	};
	//フェーズ
	Phase phase_ = Phase::Approach;
};
