#pragma once
#include "Player.h"
#include "Enemy.h"
#include "skydome.h"
#include "ReilCamera.h"

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

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	float Angle(float angle);

	/*float MinNum(float num, float num2);
	float MaxNum(float num, float num2);*/

	//パーツID
	enum PartId
	{
		kRoot,	//大元
		kSpine, //脊髄
		kChest, //胸
		kHead,	//頭
		kArmL,	//左腕
		kArmR,	//右腕
		kHip,	//尻
		kLegL,	//左足
		kLegR,	//右足

		kNumPartId
	};
  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	uint32_t textureHandle_ = 0;
	uint32_t enemyHandle_ = 0;
	//3Dモデル
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	//各クラスを渡すもの
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	Skydome* sky_ = nullptr;
	std::unique_ptr<RailCamera> camera_;
	//ワールドトランスフォーム
	
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//デバッグカメラ
	//DebugCamera* debugCamera_ = nullptr;
	//float viewAngle = 0.0f;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
