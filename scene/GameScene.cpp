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

	//範囲forで全てのワールドトランスフォームを順に処理する
	for (WorldTransform& worldTransform : worldTransforms_)
	{
		//ワールドトランスフォームの初期化
		worldTransform.Initialize();
		matrix.SetScale(worldTransform, 1, 1, 1);
		matrix.SetRot(worldTransform, rotDist(engine), rotDist(engine), rotDist(engine));
		matrix.SetTrans(worldTransform, posDist(engine), posDist(engine), posDist(engine));
		matrix.UpdateMatrix(worldTransform);
	}

	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = ChangeRadian(10.0f);
	//アスペクト比を設定
	viewProjection_.aspectRatio = 1.0f;
	//ニアクリップ距離を設定
	viewProjection_.nearZ = 52.0f;
	//ファークリップ距離を設定
	viewProjection_.farZ = 53.0f;
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

	//視点の移動ベクトル
	Vector3 move = { 0,0,0 };
	//視点の移動速さ
	const float kEyeSpeed = 0.2f;
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W))
	{
		move.z += kEyeSpeed;
	}
	else if (input_->PushKey(DIK_S))
	{
		move.z -= kEyeSpeed;
	}
	//視点移動(ベクトルの加算)
	viewProjection_.eye += move;


	//注視点の移動ベクトル
	Vector3 move2 = { 0,0,0 };
	//注視点の移動速さ
	const float kTargetSpeed = 0.2f;
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT))
	{
		move2.x -= kTargetSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		move2.x += kTargetSpeed;
	}
	//注視点移動(ベクトルの加算
	viewProjection_.target += move2;

	//上方向の回転速さ[ラジアン/frame]
	const float kUpRotSpeed = 0.05f;
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_SPACE))
	{
		viewAngle += kUpRotSpeed;
		//2π超えたら0に戻す
		viewAngle = fmodf(viewAngle, MathUtility::PI * 2.0f);
	}
	//上方向ベクトルを計算(半径1の円周上の座標)
	viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };

	const float fovChangeSpeed = 0.01f;
	////上キーで視野角が広がる
	//if (input_->PushKey(DIK_UP))
	//{
	//	viewProjection_.fovAngleY += fovChangeSpeed;
	//	//πを上回らないようにする
	//	viewProjection_.fovAngleY = MathUtility::Clamp(viewProjection_.fovAngleY, MathUtility::PI, 0.01f);
	//}
	////下キーで視野角が狭まる
	//else if (input_->PushKey(DIK_DOWN))
	//{
	//	viewProjection_.fovAngleY -= fovChangeSpeed;
	//	//0.01を下回らないようにする
	//	viewProjection_.fovAngleY = MathUtility::Clamp(viewProjection_.fovAngleY, MathUtility::PI, 0.01f);
	//}

	const float nearChangeSpeed = 0.01f;
	if (input_->PushKey(DIK_UP))
	{
		viewProjection_.nearZ += nearChangeSpeed;
	}
	else if (input_->PushKey(DIK_DOWN))
	{
		viewProjection_.nearZ -= nearChangeSpeed;
	}


	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf(
		"target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
		"up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

	debugText_->SetPos(50, 110);
	debugText_->Printf(
		"fovAngleY(Degree):%f", ChangeRadian(viewProjection_.fovAngleY));

	debugText_->SetPos(50, 130);
	debugText_->Printf(
		"nearZ:%f", viewProjection_.nearZ);
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

	for (WorldTransform& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
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
