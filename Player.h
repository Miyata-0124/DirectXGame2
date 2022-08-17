#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include <cassert>
#include "PlayerBullet.h"
#include <memory>
#include <list>


/// <summary>
/// ���L����
/// </summary>
class Player
{
public:
	/// <summary>
	/// ������
	/// </summary>

	void Initialize(Model* model);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();

	/// <summary>
	/// �ړ�����
	/// </summary>
	void Translation();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;

	Input* input_ = nullptr;
	DebugText *debugText_ = nullptr;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	
};

