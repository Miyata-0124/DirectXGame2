#pragma once
#include "WorldTransform.h"
#include "DirectXCommon.h"

//�P�ʍs��֐�
Matrix4 Identity();

//��]�s��
Matrix4 Rot(Vector3 worldTransform_);

//�X�P�[���s��
Matrix4 Scale(Vector3 worldTransform_);


//���s�ړ��s��
Matrix4 Transform(Vector3 worldTransform_);
