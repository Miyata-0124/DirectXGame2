#include "Player.h"
#include "MyMatrix.h"

void Player::Initialize(Model* model,Model* modelB)
{
	//NULLポインタチェック
	assert(model);
	assert(modelB);
	model_ = model;
	modelB_ = modelB;

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	//ワールド変換の初期化
	worldTransform_.translation_ = { 0,0,10 };
	worldTransform_.Initialize();
}

void Player::Update()
{
	//移動処理
	Translation();

	//行列計算
	worldTransform_.matWorld_ = Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= Rot(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= Transform(worldTransform_.translation_);
	
	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	//行列の再計算(更新)
	worldTransform_.TransferMatrix();

}

void Player::Translation()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };
	const float playerSpeed = 0.15f;

	//移動限界
	const float moveLimitX = 13.0f;
	const float moveLimitY = 7.0f;

	//キャラの移動変化
	if (input_->PushKey(DIK_UP)) {
		worldTransform_.parent_->translation_.z += playerSpeed;
	}
	else if (input_->PushKey(DIK_DOWN)) {
		worldTransform_.parent_->translation_.z -= playerSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.parent_->translation_.x += playerSpeed;
	}
	else if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.parent_->translation_.x -= playerSpeed;
	}

	//加算
	worldTransform_.translation_ += move;

	//移動制限
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -moveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +moveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -moveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +moveLimitY);
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

float Player::GetRadius()
{
	return radius_;
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void Player::OnCollision()
{
}
