#include "skydome.h"
#include "MyMatrix.h"

void Skydome::Initialize(Model* model)
{


	//NULLポインタチェック
	assert(model);
	model_ = model;

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 1000,1000,1000 };
	worldTransform_.rotation_ = { 0,0,0 };
	worldTransform_.translation_ = { 0,0,0 };
}

void Skydome::Update()
{
	//行列計算
	worldTransform_.matWorld_ = Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= Rot(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= Transform(worldTransform_.translation_);

	//行列の再計算(更新)
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
