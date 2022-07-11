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
	void Initialize(Model* model, const Vector3& potision);

	void Update();

	void Draw(const ViewProjection& viewProjection);

private:
	//���[���h�Ԋ҃f�[�^
	WorldTransform worldTransform_;

	//���f���̃|�C���^
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//�}�g���b�N�X
	Matrix matrix_;
};

