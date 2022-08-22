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
	delete enemy_;
}
//当たり判定
void GameScene::CheckAllCollision()
{
	// 判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();
#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();
		Vector3 len = Vectornorm(posA, posB);
		float dis = Length(len);
		float radius = player_->GetRadius() + bullet->GetRadius();
		if (dis <= radius)
		{
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	posA = enemy_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();
		Vector3 len = Vectornorm(posA, posB);
		float dis = Length(len);
		float radius = enemy_->GetRadius() + bullet->GetRadius();
		if (dis <= radius)
		{
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	// 自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet1 : enemyBullets) {
		posA = bullet1->GetWorldPosition();

		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			//自弾の座標
			posB = bullet->GetWorldPosition();
			Vector3 len = Vectornorm(posA, posB);
			float dis = Length(len);
			float radius = enemy_->GetRadius() + bullet->GetRadius();
			if (dis <= radius)
			{
				bullet1->OnCollision();
				bullet->OnCollision();
			}
		}
	}
#pragma endregion
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
	enemy_->SetPlayer(player_);
	//レ-ルカメラ
	camera_ = std::make_unique<RailCamera>();
	camera_->Initialize(Vector3(0, 0, -20), Vector3(0, 0, 0));
	//レールカメラとプレイヤーの親子構造
	player_->SetCamera(camera_->GetWorldMatrix());
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
	camera_->Update();
	//railCameraをゲームシーンに適応させる
	viewProjection_.matView = camera_->GetViewProjection().matView;
	viewProjection_.matProjection = camera_->GetViewProjection().matProjection;
	//行列の再計算
	viewProjection_.TransferMatrix();
	sky_->Update();
	CheckAllCollision();

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
