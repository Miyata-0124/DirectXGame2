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
}

void Enemy::Approach()
{
	//�ړ�
	worldTransform_.translation_ += apSpeed;
	//�K��l�̈ʒu�ɓ���������폜
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave()
{
	//�ړ�
	worldTransform_.translation_ += leSpeed;
}
