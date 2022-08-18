#include "EnemyBullet.h"
#include "MyMatrix.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();
	//�����Ŏ󂯎�������������W���Z�b�g
	worldTransform_.translation_ = position;
	velocity_ = velocity;
}

void EnemyBullet::Update()
{
	worldTransform_.translation_ -= velocity_;

	worldTransform_.matWorld_ = Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= Rot(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= Transform(worldTransform_.translation_);

	//�s��̍Čv�Z(�X�V)
	worldTransform_.TransferMatrix();

	//���Ԍo�߂ŏ���
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}
Vector3 EnemyBullet::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

float EnemyBullet::GetRadius()
{
	return radius_;
}
void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}