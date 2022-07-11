#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);
	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldtransform_.Initialize();
	matrix_.UpdateMatrix(worldtransform_);
}

void Player::Update()
{
	Move();
}

void Player::Draw(ViewProjection viewProjection_)
{
	debugText_->SetPos(30, 10);
	debugText_->Printf("pos : (%f,%f,%f)", worldtransform_.translation_.x, worldtransform_.translation_.y, worldtransform_.translation_.z);

	//3D���f����`��
	model_->Draw(worldtransform_, viewProjection_, textureHandle_);
}

void Player::Move()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };
	//�ړ��X�s�[�h
	const float moveSpeed = 0.3f;

	move.y += (input_->PushKey(DIK_W) - input_->PushKey(DIK_S)) * moveSpeed;
	move.x += (input_->PushKey(DIK_D) - input_->PushKey(DIK_A)) * moveSpeed;

	worldtransform_.translation_ += move;

	//�ړ��������W
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 19;

	//�͈͂𒴂��Ȃ�����
	worldtransform_.translation_.x = max(worldtransform_.translation_.x, -kMoveLimitX);
	worldtransform_.translation_.x = min(worldtransform_.translation_.x, kMoveLimitX);
	worldtransform_.translation_.y = max(worldtransform_.translation_.y, -kMoveLimitY);
	worldtransform_.translation_.y = min(worldtransform_.translation_.y, kMoveLimitY);

	matrix_.UpdateMatrix(worldtransform_);
}