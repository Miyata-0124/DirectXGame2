#include "Enemy.h"
#include "MyMatrix.h"
#include <cassert>
#include "Player.h"


void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 5,0,40 };

	AppInitialize();
}

void Enemy::Update()
{

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

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
	});

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

}

void Enemy::Translation()
{
	Vector3 move = { 0,0,0 };
	const float speed = 0.05f;

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
	
	//発射タイマーをデクリメント
	bulletTimer--;
	//指定時間に達したら
	if (bulletTimer <= 0) {
		//弾発射
		Fire();
		//発射タイマーを初期化
		bulletTimer = kFireInterval;
	}
}

void Enemy::AppInitialize()
{
	bulletTimer = kFireInterval;
}

void Enemy::Leave()
{
	//移動
	worldTransform_.translation_ += leSpeed;
}

void Enemy::Fire()
{
	assert(player_);
	// 弾の速度
	const float kBulletSpeed = 0.08f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = BulletRot(velocity, worldTransform_.matWorld_);

	Vector3 playerPos = player_->GetWorldPosition();
	Vector3 enemyPos = GetWorldPosition();
	Vector3 vecPos = Vectornorm(enemyPos, playerPos);
	velocity = Normalize(vecPos);
	vecPos *= kBulletSpeed;
	//弾を生成,初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録
	bullets_.push_back(std::move(newBullet));
}

Vector3 Enemy::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
	


