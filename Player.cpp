#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);
	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
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

	//3Dモデルを描画
	model_->Draw(worldtransform_, viewProjection_, textureHandle_);
}

void Player::Move()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };
	//移動スピード
	const float moveSpeed = 0.4f;

	/*move.z += input_->PushKey(DIK_W) - input_->PushKey(DIK_S);
	move.x += input_->PushKey(DIK_D) - input_->PushKey(DIK_A);*/

	if (input_->PushKey(DIK_W))
	{
		move.z += moveSpeed;
	}

	matrix_.UpdateMatrix(worldtransform_);
}