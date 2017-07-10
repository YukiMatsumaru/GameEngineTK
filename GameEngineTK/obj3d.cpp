/// <summary>
/// obj3d.cpp
/// </summary>

#include "obj3d.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�ÓI�����o�ϐ��̎���

//�f�o�C�X
Microsoft::WRL::ComPtr<ID3D11Device>            obj3d::m_d3dDevice;

//�f�o�C�X�R���e�L�X�g
Microsoft::WRL::ComPtr<ID3D11DeviceContext>     obj3d::m_d3dContext;

//�J����
Camera* obj3d::m_Camera;

//�ėp�`��States
std::unique_ptr<DirectX::CommonStates> obj3d::m_states;

//�G�t�F�N�g�t�@�N�g���[
std::unique_ptr<DirectX::EffectFactory> obj3d::m_factory;


void obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera * camera)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_Camera = camera;

	//�ėp�`��States�̐���
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	//�G�t�F�N�g�t�@�N�g������
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	
	//�e�N�X�`���̓ǂݍ��݃p�X
	m_factory->SetDirectory(L"Resources");

}

obj3d::obj3d()
{
	//�X�P�[��1�{���f�t�H���g
	m_scale = Vector3(1, 1, 1);
	
	m_parent = nullptr;
}

void obj3d::LoadModel(const wchar_t * fileName)
{
	//���f���̓ǂݍ���
	m_model = Model::CreateFromCMO(m_d3dDevice.Get(), fileName, *m_factory); 
}

void obj3d::Update()
{
	//��ɍs��̌v�Z

	//�X�P�[�����O�s��
	Matrix scalemat = Matrix::CreateScale(m_scale);

	//��]�s��
	Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);
	Matrix rotmatX = Matrix::CreateRotationY(m_rotation.x);
	Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);

	Matrix rotmat = rotmatZ * rotmatX * rotmatY;

	//���s�ړ��s��
	Matrix transmat = Matrix::CreateTranslation(m_translation);

	//���[���h�s�������
	m_world = scalemat * rotmat * transmat;

	//�e�̍s���������
	if (m_parent)
	{
		m_world *= m_parent->m_world;
	}

}

void obj3d::Draw()
{

	assert(m_Camera);

	if (m_model)
	{
		//�RD���f���̕`��
		m_model->Draw(m_d3dContext.Get(), *m_states, m_world, m_Camera->GetView(), m_Camera->GetProj());
	}

}
