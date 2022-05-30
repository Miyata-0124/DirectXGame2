#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

float GameScene::Angle(float angle)
{
	return angle * PI / 180;
}

//clamp関数
//float GameScene::MinNum(float num, float num2)
//{
//	if (num < num2)
//	{
//		return num;
//	}
//	return num2;
//}
//
//float GameScene::MaxNum(float num, float num2)
//{
//	if (num > num2)
//	{
//		return num;
//	}
//	return num2;
//}

float Clamp(float min, float max, float num)
{
	
	if (min > num)
	{
		return min;
	}
	else if (max < num)
	{
		return max;
	}
	return num;
}

//単位行列関数
Matrix4 Identity()
{
	Matrix4 matIdentity;
	/*
		1,0,0,0
		0,1,0,0
		0,0,1,0
		0,0,0,1
	*/
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	return matIdentity;
}
//回転行列
Matrix4 RotX(Vector3 worldTransform_)
{
	//各軸用回転行列を宣言
	Matrix4 matRotX;
	//x軸の回転行列宣言
	matRotX = Identity();
	/*
		1,0,0,0
		0,cos,sin,0
		0,-sin,cos,0
		0,0,0,1
	*/
	matRotX.m[1][1] = cos(worldTransform_.x);
	matRotX.m[1][2] = sin(worldTransform_.x);
	matRotX.m[2][1] = -sin(worldTransform_.x);
	matRotX.m[2][2] = cos(worldTransform_.x);
	return matRotX;
}
Matrix4 RotY(Vector3 worldTransform_)
{
	//y軸の回転行列宣言
	Matrix4 matRotY;
	matRotY = Identity();
	/*
		cos,0,-sin,0
		0,1,0,0
		sin,0,cos,0
		0,0,0,1
	*/
	matRotY.m[0][0] = cos(worldTransform_.y);
	matRotY.m[2][0] = sin(worldTransform_.y);
	matRotY.m[0][2] = -sin(worldTransform_.y);
	matRotY.m[2][2] = cos(worldTransform_.y);
	return matRotY;
}
Matrix4 RotZ(Vector3 worldTransform_)
{
	Matrix4 matRotZ;
	//z軸の回転行列宣言
	matRotZ = Identity();
	/*
		cos,sin,0,0
		-sin,cos,0,0
		0,0,1,0
		0,0,0,1
	*/
	matRotZ.m[0][0] = cos(worldTransform_.z);
	matRotZ.m[0][1] = sin(worldTransform_.z);
	matRotZ.m[1][0] = -sin(worldTransform_.z);
	matRotZ.m[1][1] = cos(worldTransform_.z);
	return matRotZ;
}

//スケール行列
Matrix4 Scale(Vector3 worldTransform_)
{
	//スケーリング行列を宣言
	Matrix4 matScale;
	matScale = Identity();
	/*
		sx,0,0,0
		0,sy,0,0
		0,0,sz,0
		0,0,0,1
	*/
	//スケーリング倍率を行列に設定する
	matScale.m[0][0] = worldTransform_.x;//sx
	matScale.m[1][1] = worldTransform_.y;//sy
	matScale.m[2][2] = worldTransform_.z;//sz
	matScale.m[3][3] = 1;
	return matScale;
}
//平行移動行列
Matrix4 Transform(Vector3 worldTransform_)
{
	//平行移動行列を宣言
	Matrix4 matTrans;
	matTrans = Identity();
	//移動量を行列に設定する
	//zの移動量設定
	matTrans.m[0][2] = 0;
	matTrans.m[1][2] = 0;
	matTrans.m[3][2] = worldTransform_.z;
	//xの移動量設定
	matTrans.m[1][0] = 0;
	matTrans.m[2][0] = 0;
	matTrans.m[3][0] = worldTransform_.x;
	//yの移動量設定
	matTrans.m[0][1] = 0;
	matTrans.m[2][1] = 0;
	matTrans.m[3][1] = worldTransform_.y;
	//行列[0][4]の設定
	matTrans.m[0][3] = 0;
	matTrans.m[1][3] = 0;
	matTrans.m[2][3] = 0;
	matTrans.m[3][3] = 1;
	return matTrans;
}


void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	//3Dモデルの生成
	model_ = Model::Create();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engin(seed_gen());
	//乱数範囲の指定
	std::uniform_real_distribution<float> dist(1, 100);
	//回転範囲の指定
	std::uniform_real_distribution<float> rot(0, 2 * PI);
	//座標範囲の指定
	std::uniform_real_distribution<float> tra(-10, +10);
	//乱数エンジンを渡し,指定範囲からランダムな数値を得る
	float value = dist(engin);

	//for (WorldTransform& worldTransform_ : worldTransforms_)
	//{
	//	//ワールドトランスフォームの初期化
	//	worldTransform_.Initialize();

		//キャラクターの大元(0)
		worldTransforms_[PartId::kRoot].Initialize();
		//脊髄(1)
		worldTransforms_[PartId::kSpine].Initialize();
		worldTransforms_[PartId::kSpine].translation_ = { 0,4.5f,0 };
		worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];

		//上半身
		worldTransforms_[PartId::kChest].Initialize(); // チェストの初期化
		worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine]; // 脊髄の子
		worldTransforms_[PartId::kChest].translation_ = { 0,2.5f,0 }; // チェストのローカル座標
		//頭
		worldTransforms_[PartId::kHead].Initialize(); // ヘッドの初期化
		worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest]; // チェストの子
		worldTransforms_[PartId::kHead].translation_ = { 0,3.5f,0 }; //ヘッドのローカル座標
		//左腕
		worldTransforms_[PartId::kArmL].Initialize(); //左腕の初期化
		worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest]; // チェストの子
		worldTransforms_[PartId::kArmL].translation_ = { -2.5f,2.5f,0 }; // 左腕のローカル座標
		//右腕
		worldTransforms_[PartId::kArmR].Initialize(); //右腕の初期化
		worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest]; //チェストの子
		worldTransforms_[PartId::kArmR].translation_ = { 2.5f,2.5f,0 }; // 右腕のローカルチェスト

		//下半身
		worldTransforms_[PartId::kHip].Initialize(); // 尻の初期化
		worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine]; //脊髄の子
		worldTransforms_[PartId::kHip].translation_ = { 0,-6.5f,0 }; // 尻のローカル座標
		//左足
		worldTransforms_[PartId::kLegL].Initialize(); // 左足の初期化
		worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip]; // 尻の子
		worldTransforms_[PartId::kLegL].translation_ = { -2.5f,2.5f,0 }; // 左足のローカル座標
		//右足
		worldTransforms_[PartId::kLegR].Initialize(); // 右足の初期化
		worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip]; // 尻の子
		worldTransforms_[PartId::kLegR].translation_ = { 2.5f,2.5f,0 }; // 右足のローカル座標
		//カメラ垂直方向視野角を設定
		viewProjection_.fovAngleY = Angle(20.0f);
#pragma region アスペクト比ニアファ
		////アスペクト比の設定
		//viewProjection_.aspectRatio = 1.0f;
		////ニアクリップ距離を設定
		//viewProjection_.nearZ = 52.0f;
		////ファークリップ距離を設定
		//viewProjection_.farZ = 53.0f;
#pragma endregion
		//ビュープロジェクションの初期化
		viewProjection_.Initialize();
		//デバッグカメラの生成
		debugCamera_ = new DebugCamera(1280, 720);


		//軸方向の表示の表示を有効にする
		AxisIndicator::GetInstance()->SetVisible(true);
		//軸方向表示が参照するビュープロジェクションを指定(アドレス渡し)
		AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
		//ライン描画が参照するビュープロジェクションを指定(アドレス渡し)
		PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_/*debugCamera_->GetViewProjection()*/);
		//x,y,z方向のスケーリング
		/*worldTransform_.scale_ = { 1,1,1 };*/
		//x,y,z軸の回転角設定
		//worldTransform_.rotation_ = { rot(engin) , rot(engin) , rot(engin) };//z軸
		//x,y,z軸の平行移動の設定
		/*worldTransform_.translation_ = { tra(engin),tra(engin),tra(engin) };*/

		//合成
		/*worldTransform_.matWorld_ *= Scale(worldTransform_.scale_);
		worldTransform_.matWorld_ *= RotZ(worldTransform_.rotation_);
		worldTransform_.matWorld_ *= RotX(worldTransform_.rotation_);
		worldTransform_.matWorld_ *= RotY(worldTransform_.rotation_);
		worldTransform_.matWorld_ *= Transform(worldTransform_.translation_);*/
	//}
}

void GameScene::Update() {
	debugCamera_->Update();
	//視点の移動ベクトル
	Vector3 move = { 0,0,0 };
	//キャラクタの移動ベクトル
	Vector3 move1 = { 0,0,0 };
	const float speed = 0.5f;
	//視点の移動速度
	const float kEyeSpeed = 0.01f;
	//上キーで視野角が上がる
	if (input_->PushKey(DIK_UP)) {
		/*viewProjection_.fovAngleY += kEyeSpeed;*/
		//ニアクリップ増
		/*viewProjection_.nearZ++;*/
	}
	//下キーで視野角が狭まる
	else if (input_->PushKey(DIK_DOWN)) {
		/*viewProjection_.fovAngleY -= kEyeSpeed;*/
		//ニアクリップ減
		/*viewProjection_.nearZ--;*/
	}
	viewProjection_.fovAngleY = Clamp(0.0f, PI, viewProjection_.fovAngleY);
	//キャラの移動変化
	if (input_->PushKey(DIK_LEFT)) {
		move1.x-=speed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move1.x+=speed;
	}
	//親にmoveを加算
	worldTransforms_[0].translation_ += move1;

	//親の行列計算
	worldTransforms_[0].matWorld_ = Transform(worldTransforms_[0].translation_);
	//子の行列計算
	worldTransforms_[1].matWorld_ = Transform(worldTransforms_[1].translation_);

	worldTransforms_[1].matWorld_ *= Transform(worldTransforms_[0].translation_);
	//親の行列転送
	worldTransforms_[0].TransferMatrix();
	//子の行列転送
	worldTransforms_[1].TransferMatrix();
#pragma region 連続移動処理
	//押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_W)) {
	//	move.z += kEyeSpeed;
	//}
	//else if (input_->PushKey(DIK_S)) {
	//	move.z -= kEyeSpeed;
	//}
	//if (input_->PushKey(DIK_LEFT)) {
	//	move.x -= kEyeSpeed;
	//}
	//else if (input_->PushKey(DIK_RIGHT)) {
	//	move.x += kEyeSpeed;
	//}

	////上方向の回転の速さ
	//const float kUpRotSpeed = 0.05f;
	////押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_SPACE)) {
	//	viewAngle += kUpRotSpeed;
	//	//2PIを超えると0に戻す
	//	viewAngle = fmodf(viewAngle, PI * 2.0f);
	//}

	////上方向ベクトルを計算(半径１の円周上の座標)
	//viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };
	////視点移動(ベクトルの加算)
	//viewProjection_.eye += move;
#pragma endregion

	//行列の再計算
	viewProjection_.UpdateMatrix();
	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf("up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	debugText_->SetPos(50, 110);
	debugText_->Printf("forAngleY(Degree):%f", (180/PI)*viewProjection_.fovAngleY);
	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
	debugText_->SetPos(50, 150);
	debugText_->Printf("worldTransforms[0]:%f", worldTransforms_[0]);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//3Dモデル描画
	//for (WorldTransform& worldTransform_ : worldTransforms_)
	//{
	//	model_->Draw(worldTransform_, viewProjection_/*debugCamera_->GetViewProjection()*/, textureHandle_);
	//}
	model_->Draw(worldTransforms_[0], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[1], viewProjection_, textureHandle_);
	

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
	//Y軸
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 0,0,0 }, { 0,10,0 }, { 0,1,0,1 });
	//X軸
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 0,0,0 }, { 10,0,0 }, { 1,0,0,1 });
	//Z軸
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 0,0,0 }, { 0,0,10 }, { 0,0,1,1 });
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
