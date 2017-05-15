//--------------------------------
//�J�����𐧌䂷��N���X
//--------------------------------
#pragma once

#include<d3d11.h>
#include<SimpleMath.h>

class Camera
{
public:
	//�����o�֐�
	Camera(int width, int height);
	virtual ~Camera();

	virtual void Update();

	//�r���[�s��̎擾
	const DirectX::SimpleMath::Matrix& GetView();

	//�v���W�F�N�V�����s��̎擾
	const DirectX::SimpleMath::Matrix& GetProj();

	//���_�̃Z�b�g
	void SetEyepos(const DirectX::SimpleMath::Vector3& eyepos);

	//�����_�̃Z�b�g
	void SetRefpos(const DirectX::SimpleMath::Vector3& refpos);

	//������x�N�g���̃Z�b�g
	void SetUpvec(const DirectX::SimpleMath::Vector3& upvec);

	//������������p�̃Z�b�g
	void SetFovY(float fovY);

	//�A�X�y�N�g��̃Z�b�g
	void SetAspect(float aspect);

	//��O�̕\�����E�����̃Z�b�g
	void SetNearclip(float nearclip);

	//���̕\�����E�����̃Z�b�g
	void SetFarclip(float farclip);

protected:
	//�����o�ϐ�

	//�r���[�s��
	DirectX::SimpleMath::Matrix m_view;

	//���_
	DirectX::SimpleMath::Vector3 m_eyepos;

	//�����_
	DirectX::SimpleMath::Vector3 m_refpos;

	//������x�N�g��
	DirectX::SimpleMath::Vector3 m_upvec;

	//�v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;

	//������������p
	float m_fovY;

	//�A�X�y�N�g��
	float m_aspect;

	//��O�̕\�����E����
	float m_nearclip;

	//���̕\�����E����
	float m_farclip;

};
