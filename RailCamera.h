#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include <cassert>
#include <memory>
#include <list>
class RailCamera
{
public:
	void Initialize(Vector3 worldtransform, Vector3 rotation);
	void Update();
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;
};