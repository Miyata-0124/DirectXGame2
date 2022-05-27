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
	std::uniform_real_distribution<float> rot(0, 2*PI);
	//座標範囲の指定
	std::uniform_real_distribution<float> tra(-10, +10);
	//乱数エンジンを渡し,指定範囲からランダムな数値を得る
	float value = dist(engin);

	for (WorldTransform& worldTransform_ : worldTransforms_)
	{
		//ワールドトランスフォームの初期化
		worldTransform_.Initialize();
		//カメラ垂直方向視野角を設定
		viewProjection_.fovAngleY = Angle(20.0f);
		////アスペクト比の設定
		//viewProjection_.aspectRatio = 1.0f;
		//ニアクリップ距離を設定
		viewProjection_.nearZ = 52.0f;
		//ファークリップ距離を設定
		viewProjection_.farZ = 53.0f;
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
		worldTransform_.scale_ = { 1,1,1 };
		//x,y,z軸の回転角設定
		worldTransform_.rotation_ = { rot(engin) , rot(engin) , rot(engin)};//z軸
		//x,y,z軸の平行移動の設定
		worldTransform_.translation_ = { tra(engin),tra(engin),tra(engin) };


		//スケーリング行列を宣言
		Matrix4 matScale;
		/*
			sx,0,0,0
			0,sy,0,0
			0,0,sz,0
			0,0,0,1
		*/
		//スケーリング倍率を行列に設定する
		matScale.m[0][0] = worldTransform_.scale_.x;//sx
		matScale.m[1][1] = worldTransform_.scale_.y;//sy
		matScale.m[2][2] = worldTransform_.scale_.z;//sz
		matScale.m[3][3] = 1;


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

		//合成用回転行列を宣言
		Matrix4 matRot;
		//各軸用回転行列を宣言
		Matrix4 matRotX, matRotY, matRotZ;
		//z軸の回転行列宣言
		matRotZ = matIdentity;
		/*
			cos,sin,0,0
			-sin,cos,0,0
			0,0,1,0
			0,0,0,1
		*/
		matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
		matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
		matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
		matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);
		//x軸の回転行列宣言
		matRotX = matIdentity;
		/*
			1,0,0,0
			0,cos,sin,0
			0,-sin,cos,0
			0,0,0,1
		*/
		matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
		matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
		matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
		matRotX.m[2][2] = cos(worldTransform_.rotation_.x);
		//y軸の回転行列宣言
		matRotY = matIdentity;
		/*
			cos,0,-sin,0
			0,1,0,0
			sin,0,cos,0
			0,0,0,1
		*/
		matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
		matRotY.m[2][0] = sin(worldTransform_.rotation_.y);
		matRotY.m[0][2] = -sin(worldTransform_.rotation_.y);
		matRotY.m[2][2] = cos(worldTransform_.rotation_.y);

		//格軸の回転行列を合成
		matRot = matRotZ *= matRotX *= matRotY;


		//worldTransform_.matWorld_ *= matRotZ;//回転
		//worldTransform_.matWorld_ *= matRotX;
		//worldTransform_.matWorld_ *= matRotY;

		//平行移動行列を宣言
		Matrix4 matTrans = MathUtility::Matrix4Identity();
		//移動量を行列に設定する
		//zの移動量設定
		matTrans.m[0][2] = 0;
		matTrans.m[1][2] = 0;
		matTrans.m[3][2] = worldTransform_.translation_.z;
		//xの移動量設定
		matTrans.m[1][0] = 0;
		matTrans.m[2][0] = 0;
		matTrans.m[3][0] = worldTransform_.translation_.x;
		//yの移動量設定
		matTrans.m[0][1] = 0;
		matTrans.m[2][1] = 0;
		matTrans.m[3][1] = worldTransform_.translation_.y;
		//行列[0][4]の設定
		matTrans.m[0][3] = 0;
		matTrans.m[1][3] = 0;
		matTrans.m[2][3] = 0;
		matTrans.m[3][3] = 1;

		worldTransform_.matWorld_ = matIdentity;

		//行列の合成
		worldTransform_.matWorld_ *= matScale;	//大きさ
		worldTransform_.matWorld_ *= matRot;	//角度
		worldTransform_.matWorld_ *= matTrans;	//移動

		//行列の転送
		worldTransform_.TransferMatrix();
	}
	
}

void GameScene::Update() {
	debugCamera_->Update();
	//視点の移動ベクトル
	Vector3 move = { 0,0,0 };
	//視点の移動速度
	const float kEyeSpeed = 0.01f;
	//上キーで視野角が上がる
	if (input_->PushKey(DIK_UP)) {
		/*viewProjection_.fovAngleY += kEyeSpeed;*/
		//ニアクリップ増
		viewProjection_.nearZ++;
	}
	//下キーで視野角が狭まる
	else if (input_->PushKey(DIK_DOWN)) {
		/*viewProjection_.fovAngleY -= kEyeSpeed;*/
		//ニアクリップ減
		viewProjection_.nearZ--;
	}
	viewProjection_.fovAngleY = Clamp(0.0f, PI, viewProjection_.fovAngleY);
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
	for (WorldTransform& worldTransform_ : worldTransforms_)
	{
		model_->Draw(worldTransform_, viewProjection_/*debugCamera_->GetViewProjection()*/, textureHandle_);
	}
	

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
