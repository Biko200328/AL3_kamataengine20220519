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
	//ワールド返還データ
	WorldTransform worldTransform_;

	//モデルのポインタ
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//マトリックス
	Matrix matrix_;
};

