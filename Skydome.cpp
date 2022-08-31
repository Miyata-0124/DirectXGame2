#include "skydome.h"
#include "MyMatrix.h"

void Skydome::Initialize(Model* model)
{


	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 1000,1000,1000 };
	worldTransform_.rotation_ = { 0,0,0 };
	worldTransform_.translation_ = { 0,0,0 };
}

void Skydome::Update()
{
	//�s��v�Z
	worldTransform_.matWorld_ = Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= Rot(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= Transform(worldTransform_.translation_);

	//�s��̍Čv�Z(�X�V)
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
