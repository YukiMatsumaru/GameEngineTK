/// <summary>
/// ���@�ɒǏ]����J����
/// </summary>
/// 
#pragma once

#include "Camera.h"

class Followcamera :public Camera
{
private:
	//�Ǐ]�Ώۂ̍��W
	DirectX::SimpleMath::Vector3 m_targetpos;

	//�Ǐ]�Ώۂ̉�]�p
	float m_targetangle;

public:
	//���@�ƃJ�����̋���
	static const float CAMERA_DISTANCE;

	//�R���X�g���N�^
	Followcamera(int width, int height);

	//���t���[���X�V
	void Update() override;

	//�Ǐ]�Ώۂ̍��W���Z�b�g
	void SetTargetpos(const DirectX::SimpleMath::Vector3& targetpos);

	//�Ǐ]�Ώۂ̊p�x���Z�b�g
	void SetTargetAngle(float targetangle);

};