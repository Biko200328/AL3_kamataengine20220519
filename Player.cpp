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
	//キャラクターの移動
	Move();
	//キャラクターの旋回処理
	Rotate();
	//キャラクターの攻撃処理
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}

void Player::Draw(const ViewProjection& viewProjection_)
{
	debugText_->SetPos(30, 10);
	debugText_->Printf("pos : (%f,%f,%f)", worldtransform_.translation_.x, worldtransform_.translation_.y, worldtransform_.translation_.z);

	//3Dモデルを描画
	model_->Draw(worldtransform_, viewProjection_, textureHandle_);

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
}

void Player::Move()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };
	//移動スピード
	const float moveSpeed = 0.3f;
	//押した方向に動く
	move.y += (input_->PushKey(DIK_W) - input_->PushKey(DIK_S)) * moveSpeed;
	move.x += (input_->PushKey(DIK_D) - input_->PushKey(DIK_A)) * moveSpeed;
	//代入
	worldtransform_.translation_ += move;
	//移動制限座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 19;
	//範囲を超えない処理
	worldtransform_.translation_.x = max(worldtransform_.translation_.x, -kMoveLimitX);
	worldtransform_.translation_.x = min(worldtransform_.translation_.x, kMoveLimitX);
	worldtransform_.translation_.y = max(worldtransform_.translation_.y, -kMoveLimitY);
	worldtransform_.translation_.y = min(worldtransform_.translation_.y, kMoveLimitY);

	matrix_.UpdateMatrix(worldtransform_);
}

void Player::Rotate()
{
	//キャラクターの回転ベクトル
	Vector3 rot = { 0,0,0 };
	//回転スピード
	const float rotSpeed = 0.05f;
	//押した方向に回転する
	rot.y += (input_->PushKey(DIK_RIGHT) - input_->PushKey(DIK_LEFT)) * rotSpeed;
	//代入
	worldtransform_.rotation_ += rot;

	matrix_.UpdateMatrix(worldtransform_);
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldtransform_.translation_);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
}