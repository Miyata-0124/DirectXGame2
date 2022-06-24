#include "PlayerBullet.h"
#include "MyMatrix.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& verosity)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();
	//�����Ŏ󂯎�������������W���Z�b�g
	worldTransform_.translation_ = position;
	velocity_ = verosity;
}

void PlayerBullet::Update()
{
	worldTransform_.translation_ += velocity_;

	worldTransform_.matWorld_  = Scale(worldTransform_.scale_);
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

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
