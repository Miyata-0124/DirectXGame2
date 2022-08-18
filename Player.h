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

class RailCamera;

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

	void OnCollision();

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//���[���J�����擾�pSetter
	void SetCamera(WorldTransform* camera) { worldTransform_.parent_ = camera; }

	float GetRadius();
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;

	Input* input_ = nullptr;
	DebugText *debugText_ = nullptr;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	//���a
	const float radius_ = 1.0f;
};

