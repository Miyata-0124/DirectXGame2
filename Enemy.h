#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"

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
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Vector3 apSpeed = { 0,0,-1.0f };
	Vector3 leSpeed = { -0.1f,0.1f,-0.5f };
};