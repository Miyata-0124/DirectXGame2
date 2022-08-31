#include "Enemy.h"
#include "MyMatrix.h"
#include <cassert>
#include "Player.h"
#include "DebugText.h"
#include <random>

void Enemy::Initialize(Model* model,Model* modelB)
{
	//NULLポインタチェック
	assert(model);
	assert(modelB);
	model_ = model;
	modelB_ = modelB;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,300 };

	AppInitialize();
}

void Enemy::Update()
{
	if (phaseTimer != 0)
	{
		phaseTimer -= 0.01f;
	}
	
	
	switch (phase_)
	{
	case Phase::Approach:
	default:
		Approach();
		break;
	case Phase::Attack:
		Attack();
		break;
	case Phase::Leave:
		Leave();
		break;
	}

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

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

	DebugText* debugText_ = DebugText::GetInstance();

	//デバッグ用表示
}

void Enemy::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);

	//弾の描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}
//距離放す
void Enemy::Approach()
{
	worldTransform_.translation_ -= apSpeed;
	
	//規定値の位置に到着したら削除
	if (phaseTimer<=0) {
		phase_ = Phase::Attack;
	}

}

void Enemy::Leave()
{
	bulletTimer--;
	if (bulletTimer >= 0)
	{
		Fire();
	}
	else if (bulletTimer <= 0)
	{
		attackTimer--;
		if (attackTimer <= 0)
		{
			//発射タイマーを初期化
			bulletTimer = kFireInterval;
			attackTimer = 100;
		}
	}
	
	worldTransform_.translation_ += leSpeed;
	if (worldTransform_.translation_.x >= 20)
	{
		leSpeed.x = -leSpeed.x;
	}
	else if (worldTransform_.translation_.x <= -20)
	{
		leSpeed.x = -leSpeed.x;
	}

}

void Enemy::AppInitialize()
{
	bulletTimer = kFireInterval;
	wayBuletTimer = wFireInterval;
}

void Enemy::Attack()
{
	//発射タイマーをデクリメント
	bulletTimer--;
	//指定時間に達したら
	if (bulletTimer <= 0) {
		//弾発射
		Fire();
		//発射タイマーを初期化
		bulletTimer = kFireInterval;
	}
	//2発目の弾
	wayBuletTimer--;
	if (wayBuletTimer <= 0)
	{
		WayFire();
		wayBuletTimer = wFireInterval;
	}

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
	newBullet->Initialize(modelB_, worldTransform_.translation_, velocity);

	// 弾を登録
	bullets_.push_back(std::move(newBullet));
}

void Enemy::WayFire()
{
	//乱数シード
	std::random_device seed_gen;
	//メルセンヌツイスター
	std::mt19937_64 engin(seed_gen());
	//乱数範囲
	std::uniform_real_distribution<float>posDist(-0.25f, 0.25f);
	//弾速度
	const float wBulletSpeed = 1;
	Vector3 velocity(posDist(engin), 0,wBulletSpeed);
	//弾の生成
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(modelB_, worldTransform_.translation_, velocity);

	// 弾を登録
	bullets_.push_back(std::move(newBullet));
}

void Enemy::OnCollision()
{
	hp -= 1;
	if (hp <= 25)
	{
		phase_ = Phase::Leave;
	}
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

float Enemy::GetRadius()
{
	return radius_;
}
