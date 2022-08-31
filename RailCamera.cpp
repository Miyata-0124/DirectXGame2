#include "RailCamera.h"
#include "MyMatrix.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	//���[���h�g�����X�t�H�[���̏����ݒ�
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.Initialize();
}

void RailCamera::Update()
{
	Vector3 speed;
	//�ړ�(�x�N�g�������Z)
	if (worldTransform_.translation_.z < 50)
	{
		speed = { 0,0,0.1f };
	}
	else if (worldTransform_.translation_.z >= 50)
	{
		speed = { 0,0,0 };
	}
	worldTransform_.translation_ += speed;

	//�g�����X�t�H�[�����X�V
	worldTransform_.matWorld_ = Identity();
	worldTransform_.matWorld_ *= Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= Rot(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= Transform(worldTransform_.translation_);
	//���[���h�s��̕��s�ړ��������擾
	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];

	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);

	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection_.target = forward += viewProjection_.eye;

	//���[���h����x�N�g��
	Vector3 up(0, 1, 0);

	//���[���J�����̉�]�𔽉f (���[���J�����̏���x�N�g��)
	viewProjection_.up = BulletRot(up, worldTransform_.matWorld_);

	//�r���[�v���W�F�N�V�������X�V
	viewProjection_.UpdateMatrix();

	DebugText* debugText_ = DebugText::GetInstance();

	//�f�o�b�O�p�\��

}

void RailCamera::OnCollision()
{
	Hp -= 1;
}

Vector3 RailCamera::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

float RailCamera::GetRadius()
{
	return radius_;
}
