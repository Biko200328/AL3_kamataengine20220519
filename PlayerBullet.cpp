#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& potision, const Vector3& velocity)
{
	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black1x1.png");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	matrix_.UpdateMatrix(worldTransform_);

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = potision;

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void PlayerBullet::Update()
{
	//座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ += velocity_;

	//時間経過でデス
	if (--deathTimer <= 0)
	{
		isDead_ = true;
	}

	//行列を更新
	matrix_.UpdateMatrix(worldTransform_);
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
