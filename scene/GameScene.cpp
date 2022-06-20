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
	isMoveChange = false;
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

	// 3Dモデルの生成
	model_ = Model::Create();

	//大元
	worldTransforms_[PartId::Root].Initialize();

	//頭
	worldTransforms_[PartId::Head].translation_ = { 0, 0, 2.0f };
	worldTransforms_[PartId::Head].parent_ = &worldTransforms_[PartId::Root];
	worldTransforms_[PartId::Head].Initialize();

	//左腕
	worldTransforms_[PartId::ArmL].translation_ = { -2.0f, 0, 0 };
	worldTransforms_[PartId::ArmL].parent_ = &worldTransforms_[PartId::Root];
	worldTransforms_[PartId::ArmL].Initialize();

	//右腕
	worldTransforms_[PartId::ArmR].translation_ = { 2.0f, 0, 0 };
	worldTransforms_[PartId::ArmR].parent_ = &worldTransforms_[PartId::Root];
	worldTransforms_[PartId::ArmR].Initialize();

	//置物
	worldTransforms_[96].translation_ = { 0.0, 0, 0 };
	worldTransforms_[96].Initialize();

	worldTransforms_[97].translation_ = { 0.0f, 0, 20.0f };
	worldTransforms_[97].Initialize();

	worldTransforms_[98].translation_ = { 20.0f, 0, 0.0f };
	worldTransforms_[98].Initialize();

	worldTransforms_[99].translation_ = { 20.0f, 0, 20.0f };
	worldTransforms_[99].Initialize();

	viewProjection_.eye = { 0, 10, -20 };

	viewProjection_.Initialize();
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
}

void GameScene::Update()
{
	if (input_->TriggerKey(DIK_Q))
	{
		if (!isMoveChange)isMoveChange = true;
		else isMoveChange = false;
	}

	if (isMoveChange == false)
	{
		const float moveSpeed = 0.05f;
		Vector3 frontVec;
		Vector3 target = viewProjection_.target;
		frontVec = target -= viewProjection_.eye;
		MathUtility::Vector3Normalize(frontVec);

		/*if (input_->PushKey(DIK_UP))
		{
			viewProjection_.eye.x += frontVec.x * moveSpeed;
			viewProjection_.eye.z += frontVec.z * moveSpeed;
		}
		if (input_->PushKey(DIK_DOWN))
		{
			viewProjection_.eye.x += -frontVec.x * moveSpeed;
			viewProjection_.eye.z += -frontVec.z * moveSpeed;
		}*/

		if (input_->PushKey(DIK_W))
		{
			worldTransforms_[PartId::Root].translation_.x += frontVec.x * moveSpeed;
			worldTransforms_[PartId::Root].translation_.z += frontVec.z * moveSpeed;
		}
		if (input_->PushKey(DIK_S))
		{
			worldTransforms_[PartId::Root].translation_.x += -frontVec.x * moveSpeed;
			worldTransforms_[PartId::Root].translation_.z += -frontVec.z * moveSpeed;
		}

		Vector3 rightVec;
		Vector3 kari = { 0,1,0 };
		rightVec = MathUtility::Vector3Cross(kari,frontVec);
		if (input_->PushKey(DIK_D))
		{
			worldTransforms_[PartId::Root].translation_.x += rightVec.x * moveSpeed;
			worldTransforms_[PartId::Root].translation_.z += rightVec.z * moveSpeed;
		}
		if (input_->PushKey(DIK_A))
		{
			worldTransforms_[PartId::Root].translation_.x += -rightVec.x * moveSpeed;
			worldTransforms_[PartId::Root].translation_.z += -rightVec.z * moveSpeed;
		}
	}

	if (isMoveChange == true)	//バイオ歩き
	{
		//プレイヤーの回転
		XMFLOAT3 move = { 0.0f,0.0f,0.0f };
		const float RotSpeed = 0.05f;
		if (input_->PushKey(DIK_A)) {
			move = { 0, -RotSpeed, 0 };
		}
		else if (input_->PushKey(DIK_D)) {
			move = { 0, RotSpeed, 0 };
		}

		worldTransforms_[PartId::Root].rotation_.y += move.y;

		XMFLOAT3 frontVec = { 0.0f, 0.0f, -1.0f };
		XMFLOAT3 resultVec = { 0.0f, 0.0f, 0.0f };

		//y軸回転をx,zに落とし込む
		resultVec.x = (
			cos(worldTransforms_[PartId::Root].rotation_.y) * frontVec.x +
			sin(worldTransforms_[PartId::Root].rotation_.y) * frontVec.z);
		resultVec.z =
			(-sinf(worldTransforms_[PartId::Root].rotation_.y) * frontVec.x +
				cosf(worldTransforms_[PartId::Root].rotation_.y) * frontVec.z);

		//プレイヤーの座標移動
		const float kCharacterSpeed = 0.1f;
		if (input_->PushKey(DIK_W)) {
			worldTransforms_[PartId::Root].translation_.x -= kCharacterSpeed * resultVec.x;
			worldTransforms_[PartId::Root].translation_.z -= kCharacterSpeed * resultVec.z;
		}
		else if (input_->PushKey(DIK_S)) {
			worldTransforms_[PartId::Root].translation_.x += kCharacterSpeed * resultVec.x;
			worldTransforms_[PartId::Root].translation_.z += kCharacterSpeed * resultVec.z;
		}

		////カメラのターゲットをプレイヤーにする
		//viewProjection_.target = worldTransforms_[PartId::Root].translation_;
		////カメラの座標移動
		//const float cameraDis = 15.0f;
		//viewProjection_.eye.x = worldTransforms_[PartId::Root].translation_.x + cameraDis * resultVec.x;
		//viewProjection_.eye.y = 10.0f;
		//viewProjection_.eye.z = worldTransforms_[PartId::Root].translation_.z + cameraDis * resultVec.z;
	}

	//UpdateMatrix
	matrix.UpdateMatrix(worldTransforms_[PartId::Root]);
	matrix.UpdateMatrix(worldTransforms_[PartId::Head]);
	matrix.UpdateMatrix(worldTransforms_[PartId::ArmL]);
	matrix.UpdateMatrix(worldTransforms_[PartId::ArmR]);
	matrix.UpdateMatrix(worldTransforms_[96]);
	matrix.UpdateMatrix(worldTransforms_[97]);
	matrix.UpdateMatrix(worldTransforms_[98]);
	matrix.UpdateMatrix(worldTransforms_[99]);

	viewProjection_.UpdateMatrix();
	

	if (input_->PushKey(DIK_RIGHT))
	{
		cameraRotSpeed += 0.1f;
	}
	if (input_->PushKey(DIK_LEFT))
	{
		cameraRotSpeed -= 0.1f;
	}

	//y軸回転をx,zに落とし込む
	resultVec.x = (
		cos(cameraRotSpeed) * frontVec.x +
		sin(cameraRotSpeed) * frontVec.z);
	resultVec.z =
		(-sinf(cameraRotSpeed) * frontVec.x +
			cosf(cameraRotSpeed) * frontVec.z);

	const float cameraSpeed = 0.1f;
	if (input_->PushKey(DIK_UP))
	{
		viewProjection_.eye.x -= cameraSpeed * resultVec.x;
		viewProjection_.eye.z -= cameraSpeed * resultVec.z;
		viewProjection_.target.x -= cameraSpeed * resultVec.x;
		viewProjection_.target.z -= cameraSpeed * resultVec.z;
	}
	if (input_->PushKey(DIK_DOWN))
	{
		viewProjection_.eye.x += cameraSpeed * resultVec.x;
		viewProjection_.eye.z += cameraSpeed * resultVec.z;
		viewProjection_.target.x += cameraSpeed * resultVec.x;
		viewProjection_.target.z += cameraSpeed * resultVec.z;
	}

	//カメラの座標移動
	const float cameraDis = 20.0f;
	viewProjection_.eye.x = cameraDis * resultVec.x;
	viewProjection_.eye.y = 10.0f;
	viewProjection_.eye.z = cameraDis * resultVec.z;

	//デバッグカメラの更新
	debugCamera_->Update();

	debugText_->SetPos(0, 0);
	debugText_->Printf(
		"isMoveChange : %d", isMoveChange);

	debugText_->SetPos(0, 30);
	debugText_->Printf(
		"target:%f,%f,%f", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

	debugText_->SetPos(0, 60);
	debugText_->Printf(
		"eye:%f,%f,%f", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// 
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

	model_->Draw(worldTransforms_[PartId::Root], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[PartId::Head], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[PartId::ArmL], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[PartId::ArmR], viewProjection_, textureHandle_);

	model_->Draw(worldTransforms_[96], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[97], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[98], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[99], viewProjection_, textureHandle_);

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
