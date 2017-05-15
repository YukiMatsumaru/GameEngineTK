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

}

//�X�V����
void Followcamera::Update()
{

	//���_�A�Q�Ɠ_
	Vector3 eyepos, refpos;

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
