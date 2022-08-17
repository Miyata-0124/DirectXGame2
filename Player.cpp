#include "Player.h"
#include "MyMatrix.h"

void Player::Initialize(Model* model)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;

	//�V���O���g���C���X�^���X���擾
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Player::Update()
{
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});
	//���񏈗�
	Translation();
	//�ړ�����
	Rotation();
	//�U������
	Attack();
	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}
	//�s��v�Z
	worldTransform_.matWorld_ = Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= Rot(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= Transform(worldTransform_.translation_);
	
	//�s��̍Čv�Z(�X�V)
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf("Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf("Rotation:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);
}

void Player::Rotation()
{
	//�L�����̉�]
	if (input_->TriggerKey(DIK_E)) {
		worldTransform_.rotation_.y = 1.55f;
	}
	else if (input_->TriggerKey(DIK_Q)) {
		worldTransform_.rotation_.y = -1.55f;
	}
	if (input_->TriggerKey(DIK_W)) {
		worldTransform_.rotation_.y = 0;
	}

}

void Player::Translation()
{

	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };
	const float playerSpeed = 0.15f;

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

	//�ړ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -moveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +moveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -moveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +moveLimitY);
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE)) {

		// �e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = BulletRot(velocity,worldTransform_.matWorld_);
		//�e�𐶐�,������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		// �e��o�^
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);

	//�e�̕`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}
