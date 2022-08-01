#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "Input.h"
#include <cassert>
#include "Matrix.h"
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

class Player
{
public:
	//初期化
	void Initialize(Model* model,uint32_t textureHandle);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection_);

private:
	
	//ワールド変換データ
	WorldTransform worldtransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//Input
	Input* input_ = nullptr;
	
	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	//マトリックス
	Matrix matrix_;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//移動
	void Move();

	//回転
	void Rotate();

	//攻撃
	void Attack();
};

