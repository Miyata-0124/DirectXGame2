#include "RailCamera.h"
#include "MyMatrix.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update()
{
	Vector3 speed;
	//移動(ベクトルを加算)
	if (worldTransform_.translation_.z < 50)
	{
		speed = { 0,0,0.1f };
	}
	else if (worldTransform_.translation_.z >= 50)
	{
		speed = { 0,0,0 };
	}
	worldTransform_.translation_ += speed;

	//トランスフォームを更新
	worldTransform_.matWorld_ = Identity();
	worldTransform_.matWorld_ *= Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= Rot(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= Transform(worldTransform_.translation_);
	//ワールド行列の平行移動成分を取得
	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];

	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);

	//視点から前方に適当な距離進んだ位置が注視点
	viewProjection_.target = forward += viewProjection_.eye;

	//ワールド上方ベクトル
	Vector3 up(0, 1, 0);

	//レールカメラの回転を反映 (レールカメラの上方ベクトル)
	viewProjection_.up = BulletRot(up, worldTransform_.matWorld_);

	//ビュープロジェクションを更新
	viewProjection_.UpdateMatrix();

	DebugText* debugText_ = DebugText::GetInstance();

	//デバッグ用表示

}

void RailCamera::OnCollision()
{
	Hp -= 1;
}

Vector3 RailCamera::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

float RailCamera::GetRadius()
{
	return radius_;
}
