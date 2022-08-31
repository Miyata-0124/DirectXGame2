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
	void Initialize(Model* model,Model* modelB);
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
	void WayFire();

	void OnCollision();

	void SetPlayer(Player* player) { player_ = player; }

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//発射間隔
	static const int kFireInterval = 50;
	static const int wFireInterval = 30;

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//ゲッター
	bool IsDead() const { return isDead_; }

	float GetRadius();

	int32_t hp = 50;
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	Model* modelB_ = nullptr;

	Vector3 apSpeed = { 0, 0, 1 };
	Vector3 leSpeed = { 0.05f,0,0.05f };

	int32_t bulletTimer = 0;
	int32_t wayBuletTimer = 0;
	int32_t phaseTimer = 200;
	int32_t attackTimer = 100;
	
	Player* player_ = nullptr;

	//半径
	const float radius_ = 1.0f;

	enum class Phase {
		Approach, //接近
		Attack,	  //攻撃
		Leave,	  //離脱
		End,	  //終了
	};
	//フェーズ
	Phase phase_ = Phase::Approach;

	//デスフラグ
	bool isDead_ = false;
};
