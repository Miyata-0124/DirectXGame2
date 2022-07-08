#include "Enemy.h"
#include "MyMatrix.h"
#include <cassert>
#include "Player.h"


void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 5,0,40 };

	AppInitialize();
}

void Enemy::Update()
{

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

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
	});

	Translation();
	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
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
	const float speed = 0.05f;

	move.z -= speed;
	worldTransform_.translation_ += move;
}

void Enemy::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�̕`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Approach()
{
	//�ړ�
	worldTransform_.translation_ += apSpeed;
	//�K��l�̈ʒu�ɓ���������폜
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
	
	//���˃^�C�}�[���f�N�������g
	bulletTimer--;
	//�w�莞�ԂɒB������
	if (bulletTimer <= 0) {
		//�e����
		Fire();
		//���˃^�C�}�[��������
		bulletTimer = kFireInterval;
	}
}

void Enemy::AppInitialize()
{
	bulletTimer = kFireInterval;
}

void Enemy::Leave()
{
	//�ړ�
	worldTransform_.translation_ += leSpeed;
}

void Enemy::Fire()
{
	assert(player_);
	// �e�̑��x
	const float kBulletSpeed = 0.08f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	velocity = BulletRot(velocity, worldTransform_.matWorld_);

	Vector3 playerPos = player_->GetWorldPosition();
	Vector3 enemyPos = GetWorldPosition();
	Vector3 vecPos = Vectornorm(enemyPos, playerPos);
	velocity = Normalize(vecPos);
	vecPos *= kBulletSpeed;
	//�e�𐶐�,������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// �e��o�^
	bullets_.push_back(std::move(newBullet));
}

Vector3 Enemy::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
	


