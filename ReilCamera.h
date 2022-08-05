#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DebugText.h"

/// <summary>
/// ���[���J����
/// </summary>
class RailCamera
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& rotation);
	/// <summary>
	/// 
	/// </summary>
	void Update();

	/// <summary>
	/// �r���[�v���W�F�N�V����
	/// </summary>
	/// <returns></returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	// ���[���h�s����擾
	WorldTransform* GetWorldMatrix() { return &worldTransform_; }
private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};
