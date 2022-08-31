#include "Player.h"
#include "MyMatrix.h"

void Player::Initialize(Model* model,Model* modelB)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);
	assert(modelB);
	model_ = model;
	modelB_ = modelB;

	//�V���O���g���C���X�^���X���擾
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	//���[���h�ϊ��̏�����
	worldTransform_.translation_ = { 0,0,10 };
	worldTransform_.Initialize();
}

void Player::Update()
{
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});
	//�ړ�����
	Translation();
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
	
	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	//�s��̍Čv�Z(�X�V)
	worldTransform_.TransferMatrix();

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
	if (input_->PushKey(DIK_UP)) {
		worldTransform_.parent_->translation_.z += playerSpeed;
	}
	else if (input_->PushKey(DIK_DOWN)) {
		worldTransform_.parent_->translation_.z -= playerSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.parent_->translation_.x += playerSpeed;
	}
	else if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.parent_->translation_.x -= playerSpeed;
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
		const float kBulletSpeed = 0.05f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = BulletRot(velocity,worldTransform_.matWorld_);
		//�e�𐶐�,������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(modelB_, worldTransform_.matWorld_, velocity);

		// �e��o�^
		bullets_.push_back(std::move(newBullet));
	}
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

float Player::GetRadius()
{
	return radius_;
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);

	//�e�̕`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::OnCollision()
{
}
