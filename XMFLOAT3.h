#pragma once
class XMFLOAT3
{
public:
	float x; // x成分
	float y; // y成分
	float z; // z成分
	// コンストラクタ
	XMFLOAT3();								//零ベクトルとして生成
	XMFLOAT3(float x,float y, float z);		//x,y,z成分を指定して生成
}; 

