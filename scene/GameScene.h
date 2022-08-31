#pragma once
#include "Player.h"
#include "Enemy.h"
#include "RailCamera.h"
#include "skydome.h"
#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "MyMatrix.h"
#define PI 3.14

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void CheckAllCollision();

	void AddEnemyBullet(std::unique_ptr<EnemyBullet> enemybullet);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	float Angle(float angle);

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	uint32_t textureHandle_ = 0u;
	uint32_t textureHP_[2] = { 0u,0u };
	uint32_t texture_[4] = { 0u,0u,0u,0u };
	Sprite* sprite_[6] = {};
	//3Dモデル
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelBullet_ = nullptr;
	//GameSceneに持たせる
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	Skydome* sky_ = nullptr;
	std::unique_ptr<RailCamera> camera_;

	int scene;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
