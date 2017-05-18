/// <summary>
/// ���@�ɒǏ]����J����
/// </summary>
/// 
#pragma once

#include "Camera.h"
#include <Keyboard.h>

class Followcamera :public Camera
{
protected:
	//�Ǐ]�Ώۂ̍��W
	DirectX::SimpleMath::Vector3 m_targetpos;

	//�Ǐ]�Ώۂ̉�]�p
	float m_targetangle;

	//FPS�J�����t���O
	bool m_isFPS;

	//�L�[�{�[�h
	DirectX::Keyboard* m_keyboard;

	//�L�[�{�[�h(�g���b�J�[)
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

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

	//�L�[�{�[�h���Z�b�g
	void SetKeyboard(DirectX::Keyboard* keyboard);

};