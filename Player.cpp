#include "Player.h"
#include "MyMatrix.h"

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update()
{
	//旋回処理
	Translation();
	//移動処理
	Rotation();
	//攻撃処理
	Attack();
	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}
	//行列計算
	worldTransform_.matWorld_ = Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= Rot(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= Transform(worldTransform_.translation_);
	
	//行列の再計算(更新)
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf("Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf("Rotation:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);
}

void Player::Rotation()
{
	const float rotSpeed = 0.05f;

	//キャラの回転
	if (input_->PushKey(DIK_E)) {
		worldTransform_.rotation_.y += rotSpeed;
	}
	else if (input_->PushKey(DIK_Q)) {
		worldTransform_.rotation_.y -= rotSpeed;
	}

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
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= playerSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x += playerSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += playerSpeed;
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move.y -= playerSpeed;
	}

	//加算
	worldTransform_.translation_ += move;

	//移動制限
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -moveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +moveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -moveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +moveLimitY);
}

void Player::Attack()
{
	if (input_->PushKey(DIK_SPACE)) {

		//弾を生成,初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_);

		//弾を登録
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾の描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}
