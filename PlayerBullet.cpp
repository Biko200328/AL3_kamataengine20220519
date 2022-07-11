#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& potision)
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
}

void PlayerBullet::Update()
{
	matrix_.UpdateMatrix(worldTransform_);
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
