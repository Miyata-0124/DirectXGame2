#include "Enemy.h"
#include "MyMatrix.h"
#include <cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 5,0,20 };
	Fire();
}

void Enemy::Update()
{
	Translation();
	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
	//行列計算
	worldTransform_.matWorld_ = Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= Rot(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= Transform(worldTransform_.translation_);

	//行列の再計算(更新)
	worldTransform_.TransferMatrix();
	switch (phase_)
	{
	case Phase::Approach:
	default:
		Approach();
		break;
	case Phase::Leave:
		Leave();
		break;
	}
	
}

void Enemy::Translation()
{
	Vector3 move = { 0,0,0 };
	const float speed = 0.5f;

	move.z -= speed;
	worldTransform_.translation_ += move;
}

void Enemy::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾の描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Approach()
{
	//移動
	worldTransform_.translation_ += apSpeed;
	//規定値の位置に到着したら削除
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave()
{
	//移動
	worldTransform_.translation_ += leSpeed;
}

void Enemy::Fire()
{
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = BulletRot(velocity, worldTransform_.matWorld_);
	//弾を生成,初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録
	bullets_.push_back(std::move(newBullet));
}


