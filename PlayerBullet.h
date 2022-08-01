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
	//ワールド返還データ
	WorldTransform worldTransform_;

	//モデルのポインタ
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//マトリックス
	Matrix matrix_;

	// 速度
	Vector3 velocity_;

	//寿命
	static const int32_t kLiteTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer = kLiteTime;

	//デスフラグ
	bool isDead_ = false;
};

