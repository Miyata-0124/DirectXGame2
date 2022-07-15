#include "PlayerBullet.h"
#include "MyMatrix.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& verosity)
{
	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();
	//引数で受け取った初期化座標をセット
	worldTransform_.translation_ = position;
	velocity_ = verosity;
}

void PlayerBullet::Update()
{
	worldTransform_.translation_ += velocity_;

	worldTransform_.matWorld_  = Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= Rot(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= Transform(worldTransform_.translation_);

	//行列の再計算(更新)
	worldTransform_.TransferMatrix();

	//時間経過で消去
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

float PlayerBullet::GetRadius()
{
	return radius_;
}
