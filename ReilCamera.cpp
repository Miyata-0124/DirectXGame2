#include "ReilCamera.h"
#include "MyMatrix.h"

void RailCamera::Initialize(const Vector3& position,const Vector3& rotation)
{
	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	//ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update()
{
	//移動(ベクトルを加算)
	worldTransform_.translation_ += Vector3(0, 0, 0.1f);
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
	//レールカメラの回転を反映(レールカメラの前方ベクトル)
	forward = BulletRot(forward, worldTransform_.matWorld_);

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
	debugText_->SetPos(20, 100);
	debugText_->Printf("RailCamera Pos(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

}
