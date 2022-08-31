#include "Enemy.h"
#include "MyMatrix.h"
#include <cassert>
#include "Player.h"
#include "DebugText.h"
#include <random>

void Enemy::Initialize(Model* model,Model* modelB)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);
	assert(modelB);
	model_ = model;
	modelB_ = modelB;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,300 };

	AppInitialize();
}

void Enemy::Update()
{
	if (phaseTimer != 0)
	{
		phaseTimer -= 0.01f;
	}
	
	
	switch (phase_)
	{
	case Phase::Approach:
	default:
		Approach();
		break;
	case Phase::Attack:
		Attack();
		break;
	case Phase::Leave:
		Leave();
		break;
	}

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

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

	DebugText* debugText_ = DebugText::GetInstance();

	//�f�o�b�O�p�\��
}

void Enemy::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);

	//�e�̕`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}
//��������
void Enemy::Approach()
{
	worldTransform_.translation_ -= apSpeed;
	
	//�K��l�̈ʒu�ɓ���������폜
	if (phaseTimer<=0) {
		phase_ = Phase::Attack;
	}

}

void Enemy::Leave()
{
	bulletTimer--;
	if (bulletTimer >= 0)
	{
		Fire();
	}
	else if (bulletTimer <= 0)
	{
		attackTimer--;
		if (attackTimer <= 0)
		{
			//���˃^�C�}�[��������
			bulletTimer = kFireInterval;
			attackTimer = 100;
		}
	}
	
	worldTransform_.translation_ += leSpeed;
	if (worldTransform_.translation_.x >= 20)
	{
		leSpeed.x = -leSpeed.x;
	}
	else if (worldTransform_.translation_.x <= -20)
	{
		leSpeed.x = -leSpeed.x;
	}

}

void Enemy::AppInitialize()
{
	bulletTimer = kFireInterval;
	wayBuletTimer = wFireInterval;
}

void Enemy::Attack()
{
	//���˃^�C�}�[���f�N�������g
	bulletTimer--;
	//�w�莞�ԂɒB������
	if (bulletTimer <= 0) {
		//�e����
		Fire();
		//���˃^�C�}�[��������
		bulletTimer = kFireInterval;
	}
	//2���ڂ̒e
	wayBuletTimer--;
	if (wayBuletTimer <= 0)
	{
		WayFire();
		wayBuletTimer = wFireInterval;
	}

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
	newBullet->Initialize(modelB_, worldTransform_.translation_, velocity);

	// �e��o�^
	bullets_.push_back(std::move(newBullet));
}

void Enemy::WayFire()
{
	//�����V�[�h
	std::random_device seed_gen;
	//�����Z���k�c�C�X�^�[
	std::mt19937_64 engin(seed_gen());
	//�����͈�
	std::uniform_real_distribution<float>posDist(-0.25f, 0.25f);
	//�e���x
	const float wBulletSpeed = 1;
	Vector3 velocity(posDist(engin), 0,wBulletSpeed);
	//�e�̐���
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(modelB_, worldTransform_.translation_, velocity);

	// �e��o�^
	bullets_.push_back(std::move(newBullet));
}

void Enemy::OnCollision()
{
	hp -= 1;
	if (hp <= 25)
	{
		phase_ = Phase::Leave;
	}
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

float Enemy::GetRadius()
{
	return radius_;
}
