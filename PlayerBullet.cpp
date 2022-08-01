#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& potision, const Vector3& velocity)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black1x1.png");

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	matrix_.UpdateMatrix(worldTransform_);

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = potision;

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
}

void PlayerBullet::Update()
{
	//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	worldTransform_.translation_ += velocity_;

	//���Ԍo�߂Ńf�X
	if (--deathTimer <= 0)
	{
		isDead_ = true;
	}

	//�s����X�V
	matrix_.UpdateMatrix(worldTransform_);
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
