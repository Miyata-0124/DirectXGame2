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
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
};