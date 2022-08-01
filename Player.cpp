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
	//�L�����N�^�[�̈ړ�
	Move();
	//�L�����N�^�[�̐��񏈗�
	Rotate();
	//�L�����N�^�[�̍U������
	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}

void Player::Draw(const ViewProjection& viewProjection_)
{
	debugText_->SetPos(30, 10);
	debugText_->Printf("pos : (%f,%f,%f)", worldtransform_.translation_.x, worldtransform_.translation_.y, worldtransform_.translation_.z);

	//3D���f����`��
	model_->Draw(worldtransform_, viewProjection_, textureHandle_);

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
}

void Player::Move()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };
	//�ړ��X�s�[�h
	const float moveSpeed = 0.3f;
	//�����������ɓ���
	move.y += (input_->PushKey(DIK_W) - input_->PushKey(DIK_S)) * moveSpeed;
	move.x += (input_->PushKey(DIK_D) - input_->PushKey(DIK_A)) * moveSpeed;
	//���
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

void Player::Rotate()
{
	//�L�����N�^�[�̉�]�x�N�g��
	Vector3 rot = { 0,0,0 };
	//��]�X�s�[�h
	const float rotSpeed = 0.05f;
	//�����������ɉ�]����
	rot.y += (input_->PushKey(DIK_RIGHT) - input_->PushKey(DIK_LEFT)) * rotSpeed;
	//���
	worldtransform_.rotation_ += rot;

	matrix_.UpdateMatrix(worldtransform_);
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldtransform_.translation_);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}