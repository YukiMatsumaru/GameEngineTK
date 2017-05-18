#include "Followcamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�ÓI�����o�ϐ��̏�����
const float Followcamera::CAMERA_DISTANCE = 5.0f;

Followcamera::Followcamera(int width, int height)
	:Camera(width,height)
{
	m_targetpos = Vector3::Zero;

	m_targetangle = 0.0f;

	m_keyboard = nullptr;

	m_isFPS = false;

}

//�X�V����
void Followcamera::Update()
{
	//�L�[�{�[�h�̏�Ԃ��擾
	Keyboard::State keyboardstate = m_keyboard->GetState();
	m_KeyboardTracker.Update(keyboardstate);

	//C�L�[���������Ƃ�TPS��FPS�̐؂�ւ�
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keys::C))
	{
		//�t���O�̐؂�ւ�
		m_isFPS = !m_isFPS;

	}

	//���_�A�Q�Ɠ_
	Vector3 eyepos, refpos;
	if(m_isFPS)
	{//TPS�J��������

		//���@�̏��2m���Q�Ɠ_�Ƃ���
		refpos = m_targetpos + Vector3(0, 2.0f, 0);

		//�Q�Ɠ_�Ǝ��_�̍����x�N�g��
		Vector3 cameraV(0, 0, CAMERA_DISTANCE);

		//���@�̌��ɉ�荞�ނ��߂̉�]�s��̐���
		Matrix rotmat = Matrix::CreateRotationY(m_targetangle);

		//�����x�N�g������]������
		cameraV = Vector3::TransformNormal(cameraV, rotmat);

		//���_���W���v�Z
		eyepos = refpos + cameraV;

		//���_�����݈ʒu�����Ԃ���
		eyepos = m_eyepos + (eyepos - m_eyepos) * 0.05f;

		//�Q�Ɠ_�����݈ʒu�����Ԃ���
		refpos = m_refpos + (refpos - m_refpos) * 0.2f;

	}
	else
	{//FPS�J��������
		 //���@�̖ڂ̈ʒu����������_�Ƃ���
		 Vector3 position;
		 position = m_targetpos + Vector3(0, 0.2f, 0);
		 //eyepos = m_targetpos + Vector3(0, 0.2f, 0);

		 //�Q�Ɠ_�Ǝ��_�̍����x�N�g��
		 Vector3 cameraV(0, 0, -CAMERA_DISTANCE);

		 //���@�̌��ɉ�荞�ނ��߂̉�]�s��̐���
		 Matrix rotmat = Matrix::CreateRotationY(m_targetangle);

		 //�����x�N�g������]������
		 cameraV = Vector3::TransformNormal(cameraV, rotmat);

		 eyepos = position + cameraV*0.1f;

		 //�Q�Ɠ_���W���v�Z
		 refpos = position + cameraV;
	}

	//�J�����ɏ����Z�b�g
	SetEyepos(eyepos);
	SetRefpos(refpos);

	//���N���X�̍X�V
	Camera::Update();

}

//�Ǐ]�Ώۂ̍��W���Z�b�g
void Followcamera::SetTargetpos(const DirectX::SimpleMath::Vector3 & targetpos)
{
	m_targetpos = targetpos;
}

//�Ǐ]�Ώۂ̊p�x���Z�b�g
void Followcamera::SetTargetAngle(float targetangle)
{
	m_targetangle = targetangle;
}

//�L�[�{�[�h���Z�b�g
void Followcamera::SetKeyboard(DirectX::Keyboard * keyboard)
{
	m_keyboard = keyboard;
}
