#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"
#include <cassert>
#include "Matrix.h"
#include "ViewProjection.h"

class PlayerBullet
{
public:
	void Initialize(Model* model, const Vector3& potision,const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

private:
	//���[���h�Ԋ҃f�[�^
	WorldTransform worldTransform_;

	//���f���̃|�C���^
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//�}�g���b�N�X
	Matrix matrix_;

	// ���x
	Vector3 velocity_;

	//����
	static const int32_t kLiteTime = 60 * 5;

	//�f�X�^�C�}�[
	int32_t deathTimer = kLiteTime;

	//�f�X�t���O
	bool isDead_ = false;
};

