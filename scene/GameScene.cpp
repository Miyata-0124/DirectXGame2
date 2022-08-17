#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete modelSkydome_;
	/*delete debugCamera_;*/
	delete player_;
}

float GameScene::Angle(float angle)
{
	return angle * PI / 180;
}

//clamp関数
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
	//3Dモデルの生成
	model_ = Model::CreateFromOBJ("Player", true);
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	//自キャラ生成
	player_ = new Player();
	player_->Initialize(model_);
	//敵キャラ生成
	enemy_ = new Enemy();
	enemy_->Initialize(model_);
	//背景生成
	sky_ = new Skydome();
	sky_->Initialize(modelSkydome_);
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
		
	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = Angle(20.0f);

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//軸方向の表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//ライン描画が参照するビュープロジェクションを指定(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_/*debugCamera_->GetViewProjection()*/);

}

void GameScene::Update() {

	player_->Update();
	enemy_->Update();
	sky_->Update();

	//行列の再計算
	viewProjection_.UpdateMatrix();
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
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	sky_->Draw(viewProjection_);
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
