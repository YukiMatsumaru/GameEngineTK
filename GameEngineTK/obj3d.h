/// <summary>
/// 3D�I�u�W�F�N�g�̃N���X
/// </summary>
/// 
#pragma once
#include <windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>

#include "Camera.h"

class obj3d
{
	//�ÓI�����o
private:
	//�f�o�C�X
	static Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;

	//�f�o�C�X�R���e�L�X�g
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;

	//�J����
	static Camera* m_Camera;

	//�ėp�`��States
	static std::unique_ptr<DirectX::CommonStates> m_states;

	//�G�t�F�N�g�t�@�N�g���[
	static std::unique_ptr<DirectX::EffectFactory> m_factory;

public:
	//�ÓI�����o�ϐ��̏�����
	static void InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice,
								 Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, 
		                         Camera* camera);

private:
	//3D���f��
	std::unique_ptr<DirectX::Model> m_model;

	//�X�P�[�����O (XYZ)
	DirectX::SimpleMath::Vector3	m_scale;

	//��]�p (XYZ)
	DirectX::SimpleMath::Vector3	m_rotation;

	//���s�ړ� (XYZ)
	DirectX::SimpleMath::Vector3	m_translation;

	//���[���h�s��
	DirectX::SimpleMath::Matrix		m_world;

	//�e�̂RD�I�u�W�F�N�g�̃|�C���^
	obj3d* m_parent;

public:
	//�R���X�g���N�^
	obj3d();

	//���f���̃��[�h
	void LoadModel(const wchar_t* fileName);

	//�X�V
	void Update();

	//�`��
	void Draw();

	//setter

	//�X�P�[�����O (XYZ)
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }

	//��]�p (XYZ)
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation) { m_rotation= rotation; }

	//���s�ړ� (XYZ)
	void SetTranslation(const DirectX::SimpleMath::Vector3& translation) { m_translation = translation; }

	//�e�̂RD�I�u�W�F�N�g�̃|�C���^
	void SetParent(obj3d* parent) {m_parent = parent; }

	//getter

	//�X�P�[�����O (XYZ)
	const DirectX::SimpleMath::Vector3& GetScale() { return m_scale; }

	//��]�p (XYZ)
	const DirectX::SimpleMath::Vector3& GetRotation() { return m_rotation; }


	//���s�ړ� (XYZ)
	const DirectX::SimpleMath::Vector3& GetTranslation() { return m_translation; }

	//�e�̂RD�I�u�W�F�N�g�̃|�C���^
	obj3d* GetParent() { return m_parent; }

	//���[���h�s��
	const DirectX::SimpleMath::Matrix& GetWorld() { return m_world; }


};