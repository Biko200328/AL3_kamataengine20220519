#pragma once
#include "WorldTransform.h"

struct MatResult
{
	Matrix4 Scale;
	Matrix4 Rot;
	Matrix4 Trans;
};

class Matrix
{
private:
	MatResult matResult;

public:
	Matrix();

	void InitMatrix(Matrix4& m);

	//scale
	void SetScale(WorldTransform& worldTransform, float X, float Y, float Z);
	/*void ChangeScale(WorldTransform worldTransform);*/

	//rotation
	void SetRot(WorldTransform& worldTransform, float X, float Y, float Z);
	/*void ChangeRot(WorldTransform worldTransform);*/

	//transform
	void SetTrans(WorldTransform& worldTransform, float X, float Y, float Z);
	/*void ChangeTrans(WorldTransform worldTransform);*/

	void UpdateMatrix(WorldTransform& worldTransform);
};

