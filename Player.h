#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"
#include <cassert>
#include "Matrix.h"

class Player
{
public:
	//������
	void Initialize(Model* model,uint32_t textureHandle);

	//�X�V
	void Update();

	//�`��
	void Draw();

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldtransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//Input
	Input* input_ = nullptr;
	
	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	//�}�g���b�N�X
	Matrix matrix_;
};

