#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>

GameScene::GameScene()
{
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	//3Dモデルの生成
	model_ = Model::Create();
	//デバッグカメラの生成
debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
}

GameScene::~GameScene()
{
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数範囲の指定 (回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, ChangeRadian(360));
	//乱数範囲の指定 (座標用)
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	////範囲forで全てのワールドトランスフォームを順に処理する
	//for (WorldTransform& worldTransform : worldTransforms_)
	//{
	//	//ワールドトランスフォームの初期化
	//	worldTransform.Initialize();
	//	matrix.SetScale(worldTransform, 1, 1, 1);
	//	matrix.SetRot(worldTransform, rotDist(engine), rotDist(engine), rotDist(engine));
	//	matrix.SetTrans(worldTransform, posDist(engine), posDist(engine), posDist(engine));
	//	matrix.UpdateMatrix(worldTransform);
	//}

	//親(0番)
	worldTransforms_[PartID::kRoot].Initialize();
	//脊髄
	worldTransforms_[PartID::kSpine].Initialize();
	worldTransforms_[PartID::kSpine].parent_ = &worldTransforms_[PartID::kRoot];
	//胸
	worldTransforms_[PartID::kChest].Initialize();
	worldTransforms_[PartID::kChest].parent_ = &worldTransforms_[PartID::kSpine];
	//頭
	worldTransforms_[PartID::kHead].Initialize();
	worldTransforms_[PartID::kHead].translation_ = { 0,4.5f,0 };
	worldTransforms_[PartID::kHead].parent_ = &worldTransforms_[PartID::kChest];
	//左腕
	worldTransforms_[PartID::kArmL].Initialize();
	worldTransforms_[PartID::kArmL].translation_ = { -4.5f,0,0 };
	worldTransforms_[PartID::kArmL].parent_ = &worldTransforms_[PartID::kChest];
	//右腕
	worldTransforms_[PartID::kArmR].Initialize();
	worldTransforms_[PartID::kArmR].translation_ = { 4.5f,0,0 };
	worldTransforms_[PartID::kArmR].parent_ = &worldTransforms_[PartID::kChest];

	//尻
	worldTransforms_[PartID::kHip].Initialize();
	worldTransforms_[PartID::kHip].translation_ = { 0,-4.5f,0 };
	worldTransforms_[PartID::kHip].parent_ = &worldTransforms_[PartID::kSpine];
	//左足
	worldTransforms_[PartID::kLegL].Initialize();
	worldTransforms_[PartID::kLegL].translation_ = { -4.5f,-4.5f,0 };
	worldTransforms_[PartID::kLegL].parent_ = &worldTransforms_[PartID::kHip];
	//右足
	worldTransforms_[PartID::kLegR].Initialize();
	worldTransforms_[PartID::kLegR].translation_ = { 4.5f,-4.5f,0 };
	worldTransforms_[PartID::kLegR].parent_ = &worldTransforms_[PartID::kHip];


	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update()
{
	debugCamera_->Update();

	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	if (input_->PushKey(DIK_LEFT))
	{
		move.x = -0.2f;
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		move.x = 0.2f;
	}

	worldTransforms_[0].translation_ += move;

	const float kChestRotSpeed = 0.1f;
	if (input_->PushKey(DIK_U))
	{
		worldTransforms_[PartID::kChest].rotation_.y -= kChestRotSpeed;
	}
	else if (input_->PushKey(DIK_I))
	{
		worldTransforms_[PartID::kChest].rotation_.y += kChestRotSpeed;
	}

	const float kHipRotSpeed = 0.1f;
	if (input_->PushKey(DIK_J))
	{
		worldTransforms_[PartID::kHip].rotation_.y -= kChestRotSpeed;
	}
	else if (input_->PushKey(DIK_K))
	{
		worldTransforms_[PartID::kHip].rotation_.y += kChestRotSpeed;
	}
	
	for (int i = 0; i < kNumPartID; i++)
	{
		matrix.UpdateMatrix(worldTransforms_[i]);
	}

	debugText_->SetPos(10, 10);
	debugText_->Printf(
		"worldTransform_[0].trans:(%f,%f,%f)", worldTransforms_[0].translation_.x, worldTransforms_[0].translation_.y, worldTransforms_[0].translation_.z);

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	//3Dモデル描画
	/*model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);*/

	/*for (WorldTransform& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}*/

	for (int i = 0; i < kNumPartID; i++)
	{
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//int maxLine = 32;
	//int size = 1;
	//for (int i = 0; i < maxLine; i++)
	//{
	//	//X (Red)
	//	PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(0, i * size, 0), Vector3(maxLine * size - size, i * size, 0), Vector4(1, 0, 0, 1));
	//	PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(0, 0, i * size), Vector3(maxLine * size - size, 0, i * size), Vector4(1, 0, 0, 1));

	//	//Y (Green)
	//	PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(i * size, 0, 0), Vector3(i * size, maxLine * size - size, 0), Vector4(0, 1, 0, 1));
	//	PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(0, 0, i * size), Vector3(0, maxLine * size - size, i * size), Vector4(0, 1, 0, 1));

	//	//Z (Blue)
	//	PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(i * size, 0, 0), Vector3(i * size, 0, maxLine * size - size), Vector4(0, 0, 1, 1));
	//	PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(0, i * size, 0), Vector3(0, i * size, maxLine * size - size), Vector4(0, 0, 1, 1));
	//}

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
