#include "Enemy.h"
#include "MyMatrix.h"
#include <cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,20 };
}

void Enemy::Update()
{
	Translation();
	//�s��v�Z
	worldTransform_.matWorld_ = Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= Rot(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= Transform(worldTransform_.translation_);

	//�s��̍Čv�Z(�X�V)
	worldTransform_.TransferMatrix();
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
}