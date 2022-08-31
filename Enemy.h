#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>

class Player;

class Enemy
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model,Model* modelB);
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	void Approach();
	void AppInitialize();
	void Attack();
	void Leave();

	/// <summary>
	/// �e����
	/// </summary>
	void Fire();
	void WayFire();

	void OnCollision();

	void SetPlayer(Player* player) { player_ = player; }

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//�e���X�g���擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//���ˊԊu
	static const int kFireInterval = 50;
	static const int wFireInterval = 30;

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Q�b�^�[
	bool IsDead() const { return isDead_; }

	float GetRadius();

	int32_t hp = 50;
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	Model* modelB_ = nullptr;

	Vector3 apSpeed = { 0, 0, 1 };
	Vector3 leSpeed = { 0.05f,0,0.05f };

	int32_t bulletTimer = 0;
	int32_t wayBuletTimer = 0;
	int32_t phaseTimer = 200;
	int32_t attackTimer = 100;
	
	Player* player_ = nullptr;

	//���a
	const float radius_ = 1.0f;

	enum class Phase {
		Approach, //�ڋ�
		Attack,	  //�U��
		Leave,	  //���E
		End,	  //�I��
	};
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	//�f�X�t���O
	bool isDead_ = false;
};
