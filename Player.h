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
	//初期化
	void Initialize(Model* model,uint32_t textureHandle);

	//更新
	void Update();

	//描画
	void Draw();

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
};

