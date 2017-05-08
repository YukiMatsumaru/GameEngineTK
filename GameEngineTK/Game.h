//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "DebugCamera.h"

#include <VertexTypes.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>
#include <Keyboard.h>

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

    Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>>m_batch;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	//�f�o�b�N�J����
	std::unique_ptr<DebugCamera>m_debugCamera;

	//�G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> m_factory;

	//�n�ʃ��f��
	std::unique_ptr<DirectX::Model> m_modelGround;

	//�V�����f��
	std::unique_ptr<DirectX::Model> m_modelSkydome;

	//�����f��
	std::unique_ptr<DirectX::Model> m_modelBall;

	//�e�B�[�|�b�g���f��
	std::unique_ptr<DirectX::Model> m_modelTeapot;

	//�^���N���f��
	std::unique_ptr<DirectX::Model> m_modeltank;

	//���[���h�s��
	DirectX::SimpleMath::Matrix m_worldBall[20];

	//���[���h�s��
	DirectX::SimpleMath::Matrix m_worldTeapot[20];

	//���̉�]�ϐ�
	float rotateball;

	//�e�B�[�|�b�g�̕��s�ړ�
	float x[20],z[20];

	//�e�B�[�|�b�g�̋���
	float range;

	//�e�B�[�|�b�g�̊p�x
	float angle;

	//�t���[���J�E���g
	int frame;
	
	//�L�[�{�[�h
	std::unique_ptr<DirectX::Keyboard> m_keyboard;

	//���@�̍��W
	DirectX::SimpleMath::Vector3 tank_pos;

	//���@�̉�]�p
	float tank_angle;

	//���@�̃��[���h�s��
	DirectX::SimpleMath::Matrix tank_world;

	DirectX::SimpleMath::Matrix scalemat;

	DirectX::SimpleMath::Matrix rotmatZ;
	DirectX::SimpleMath::Matrix rotmatX;
	DirectX::SimpleMath::Matrix rotmatY;

	DirectX::SimpleMath::Matrix rotmat;
	DirectX::SimpleMath::Matrix transmat;

	

};