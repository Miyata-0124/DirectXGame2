#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	/*delete debugCamera_;*/
	delete player_;
	delete enemy_;
}

void GameScene::CheckAllCollisions()
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
		float radius = player_->GetRadius()+bullet->GetRadius();
		if (dis<=radius)
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
	textureHandle_ = TextureManager::Load("mario.jpg");
	enemyHandle_ = TextureManager::Load("A.jpg");
	//3Dモデルの生成
	model_ = Model::Create();
	//自キャラ生成
	player_ = new Player();
	player_->Initialize(model_,textureHandle_);
	
	//敵キャラ生成
	enemy_ = new Enemy();
	enemy_->Initialize(model_, enemyHandle_);
	enemy_->SetPlayer(player_);
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
		/*debugCamera_ = new DebugCamera(1280, 720);*/


		//軸方向の表示の表示を有効にする
		AxisIndicator::GetInstance()->SetVisible(true);
		//軸方向表示が参照するビュープロジェクションを指定(アドレス渡し)
	/*	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());*/
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

	player_->Update();
	enemy_->Update();
	CheckAllCollisions();
	/*debugCamera_->Update();*/
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
	
	enemy_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
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
