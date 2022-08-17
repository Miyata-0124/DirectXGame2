#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"

enum class Phase {
	Approach, //接近
	Leave,	  //離脱
};

class Enemy
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 移動
	/// </summary>
	void Translation();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	void Approach();
	void Leave();
	//フェーズ
	Phase phase_ = Phase::Approach;
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;

	Vector3 apSpeed = { 0,0,-1.0f };
	Vector3 leSpeed = { -0.1f,0.1f,-0.5f };
};
