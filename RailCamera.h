#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DebugText.h"
class Enemy;

class RailCamera
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& rotation);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	void OnCollision();

	//���[���h���W���擾
	Vector3 GetWorldPosition();
	/// <summary>
	/// �r���[�v���W�F�N�V����
	/// </summary>
	/// <returns></returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	// ���[���h�s����擾
	WorldTransform* GetWorldMatrix() { return &worldTransform_; }

	//�Q�b�^�[
	bool IsDead() const { return isDead_; }

	float GetRadius();

	int32_t Hp = 10;

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	//���a
	const float radius_ = 1;
	
	//�f�X�t���O
	bool isDead_ = false;
};
