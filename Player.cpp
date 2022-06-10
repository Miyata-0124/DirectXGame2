#include "Player.h"
#include "MyMatrix.h"

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Player::Update()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };
	const float playerSpeed = 0.5f;
	//�ړ����E
	const float moveLimitX = 13.0f;
	const float moveLimitY = 7.0f;
	//�L�����̈ړ��ω�
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= playerSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x += playerSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += playerSpeed;
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move.y -= playerSpeed;
	}

	//���Z
	worldTransform_.translation_ += move;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -moveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +moveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -moveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +moveLimitY);
	//�s��v�Z
	worldTransform_.matWorld_ = Transform(worldTransform_.translation_);
	
	//�s��̍Čv�Z(�X�V)
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf("Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
