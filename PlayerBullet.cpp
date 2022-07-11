#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& potision)
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
}

void PlayerBullet::Update()
{
	matrix_.UpdateMatrix(worldTransform_);
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
