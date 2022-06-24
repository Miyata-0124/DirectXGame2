#pragma once
#include "WorldTransform.h"
#include "DirectXCommon.h"

//単位行列関数
Matrix4 Identity();

//回転行列
Matrix4 Rot(Vector3 worldTransform_);

//スケール行列
Matrix4 Scale(Vector3 worldTransform_);

//平行移動行列
Matrix4 Transform(Vector3 worldTransform_);

//弾の回転用計算
Vector3 BulletRot(Vector3 velocity,Matrix4 transform);

