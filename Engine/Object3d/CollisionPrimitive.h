#pragma once
#include <DirectXMath.h>

using namespace DirectX;
//OBB
	class OBB
	{
	private:
		XMVECTOR m_Pos;//座標
		XMVECTOR m_NormalDirect[3];//方向ベクトル	
		float m_length[3];//各軸方向の長さ

	public:
		DirectX::XMVECTOR GetDirect(int elem) { return m_NormalDirect[elem]; } // 指定軸番号の方向ベクトルを取得
		float GetLen_W(int elem) { return m_length[elem]; } // 指定軸方向の長さを取得
		DirectX::XMVECTOR GetPos_W() { return m_Pos; }//位置を取得


		void SetParam_Pos(XMMATRIX posMat);
		void SetParam_Pos(XMFLOAT3 pos);
		void SetParam_Scl(XMFLOAT3 scl);
		void SetParam_Rot(DirectX::XMMATRIX rotMat);
	};
