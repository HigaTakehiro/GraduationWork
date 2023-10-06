#pragma once
#include <DirectXMath.h>

using namespace DirectX;
//OBB
	class OBB
	{
	private:
		XMVECTOR m_Pos;//���W
		XMVECTOR m_NormalDirect[3];//�����x�N�g��	
		float m_length[3];//�e�������̒���

	public:
		DirectX::XMVECTOR GetDirect(int elem) { return m_NormalDirect[elem]; } // �w�莲�ԍ��̕����x�N�g�����擾
		float GetLen_W(int elem) { return m_length[elem]; } // �w�莲�����̒������擾
		DirectX::XMVECTOR GetPos_W() { return m_Pos; }//�ʒu���擾


		void SetParam_Pos(XMMATRIX posMat);
		void SetParam_Pos(XMFLOAT3 pos);
		void SetParam_Scl(XMFLOAT3 scl);
		void SetParam_Rot(DirectX::XMMATRIX rotMat);
	};
