#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>
#include "Input.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	/*delete debugCamera_;*/
	delete player_;
	for (int i = 0; i < 6; i++)
	{
		delete sprite_[i];
	}
}
//当たり判定
void GameScene::CheckAllCollision()
{

}

float GameScene::Angle(float angle)
{
	return angle * PI / 180;
}


void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	//3Dモデルの生成
	
	//自キャラ生成
	
	//敵キャラ生成
	
	//レ-ルカメラ
	camera_ = std::make_unique<RailCamera>();
	camera_->Initialize(Vector3(0, 0, -20), Vector3(0, 0, 0));

	//背景生成
	
	//シーン管理
	scene = 0;
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
	switch (scene)
	{
		case 0:
		{
			Initialize();
			if (input_->TriggerKey(DIK_SPACE))
			{
				scene = 1;
			}
			break;
		}
		case 1:
		{
			
			player_->Update();
			camera_->Update();
			CheckAllCollision();

			//railCameraをゲームシーンに適応させる
			viewProjection_.matView = camera_->GetViewProjection().matView;
			viewProjection_.matProjection = camera_->GetViewProjection().matProjection;
			//行列の再計算
			viewProjection_.TransferMatrix();

			break;
		}
		case 2:
		{
			if (input_->TriggerKey(DIK_R))
			{
				scene = 0;
			}
			break;
		}
		case 3:
		{
			if (input_->TriggerKey(DIK_R))
			{
				scene = 0;
			}
			break;
		}
	}

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
	switch(scene)
	{
		case 0:
		{
			break;
		}
		case 1:
		{
			player_->Draw(viewProjection_);
			break;
		}
		case 2:
		{
			break;
		}
		case 3:
		{
			break;
		}
	}
	// 3Dオブジェクト描画後処理

	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	/// 
	switch (scene)
	{
		case 0:
		{
			break;
		}
		case 1:
		{
			break;
		}
		case 2:
		{
			break;
		}
		case 3:
		{
			break;
		}
	}

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
