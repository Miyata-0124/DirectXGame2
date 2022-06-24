#include "MyMatrix.h"

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
Matrix4 Rot(Vector3 worldTransform_)
{
	//各軸用回転行列を宣言
	Matrix4 matRot;
	Matrix4 matRotX;
	Matrix4 matRotY;
	Matrix4 matRotZ;
	//x軸の回転行列宣言
	matRotX = Identity();
	//y軸の回転行列宣言
	matRotY = Identity();
	//z軸の回転行列宣言
	matRotZ = Identity();
	//x
	matRotX.m[1][1] = cos(worldTransform_.x);
	matRotX.m[1][2] = sin(worldTransform_.x);
	matRotX.m[2][1] = -sin(worldTransform_.x);
	matRotX.m[2][2] = cos(worldTransform_.x);
	//y
	matRotY.m[0][0] = cos(worldTransform_.y);
	matRotY.m[2][0] = sin(worldTransform_.y);
	matRotY.m[0][2] = -sin(worldTransform_.y);
	matRotY.m[2][2] = cos(worldTransform_.y);
	//z
	matRotZ.m[0][0] = cos(worldTransform_.z);
	matRotZ.m[0][1] = sin(worldTransform_.z);
	matRotZ.m[1][0] = -sin(worldTransform_.z);
	matRotZ.m[1][1] = cos(worldTransform_.z);
	return matRot = matRotZ *= matRotX *= matRotY;
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

Vector3 BulletRot(Vector3 velocity, Matrix4 transform)
{
	Vector3 bulletMath;

	bulletMath.x  = velocity.x * transform.m[0][0];
	bulletMath.x += velocity.y * transform.m[1][0];
	bulletMath.x += velocity.z * transform.m[2][0];
	bulletMath.x += velocity.x * transform.m[3][0];

	bulletMath.y  = velocity.x * transform.m[0][1];
	bulletMath.y += velocity.y * transform.m[1][1];
	bulletMath.y += velocity.z * transform.m[2][1];
	bulletMath.y += velocity.y * transform.m[3][1];

	bulletMath.z  = velocity.x * transform.m[0][2];
	bulletMath.z += velocity.y * transform.m[1][2];
	bulletMath.z += velocity.z * transform.m[2][2];
	bulletMath.z += velocity.z * transform.m[3][2];

	return bulletMath;
}
