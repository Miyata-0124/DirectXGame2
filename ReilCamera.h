#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DebugText.h"

/// <summary>
/// レールカメラ
/// </summary>
class RailCamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& rotation);
	/// <summary>
	/// 
	/// </summary>
	void Update();

	/// <summary>
	/// ビュープロジェクション
	/// </summary>
	/// <returns></returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	// ワールド行列を取得
	WorldTransform* GetWorldMatrix() { return &worldTransform_; }
private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};
