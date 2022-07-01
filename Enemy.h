#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "EnemyBullet.h"

enum class Phase {
	Approach, //�ڋ�
	Leave,	  //���E
};

class Enemy
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �ړ�
	/// </summary>
	void Translation();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	void Approach();
	void Leave();

	/// <summary>
	/// �e����
	/// </summary>
	void Fire();
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Vector3 apSpeed = { 0,0,-0.01f };
	Vector3 leSpeed = { 0.0f,0.0f,-0.01f };
};