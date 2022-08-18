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
	void Initialize(Model* model);
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

	void OnCollision();

	void SetPlayer(Player* player) { player_ = player; }
	

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//�e���X�g���擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//���ˊԊu
	static const int kFireInterval = 30;

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	float GetRadius();
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;

	Vector3 apSpeed = { 0, 0, -0.05f };
	Vector3 leSpeed = { -0.1f,0.1f,-0.5f };

	int32_t bulletTimer = 0;
	Player* player_ = nullptr;

	//���a
	const float radius_ = 1.0f;

	enum class Phase {
		Approach, //�ڋ�
		Attack,	  //�U��
		Leave,	  //���E
	};
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
};
