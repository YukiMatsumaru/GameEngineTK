//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game() :
	m_window(0),
	m_outputWidth(800),
	m_outputHeight(600),
	m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateDevice();

    CreateResources();


    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

	//キーボードの初期化
	keyboard = std::make_unique<Keyboard>();

	m_Camera = std::make_unique<Followcamera>(m_outputWidth, m_outputHeight);

	m_Camera->SetKeyboard(keyboard.get());

	//3Dオブジェクトの静的メンバを初期化
	obj3d::InitializeStatic(m_d3dDevice, m_d3dContext, m_Camera.get());

	//プリミティブバッチの初期化
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(m_d3dContext.Get());

	//エフェクトの初期化
	
	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());

	/*m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
		m_outputWidth, m_outputHeight, 0, 0, 1));*/

	m_view = Matrix::CreateLookAt(Vector3(2.f, 2.f, 2.f),
		Vector3::Zero, Vector3::UnitY);

	m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(m_outputWidth) / float(m_outputHeight), 0.1f, 500.f);

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

	//デバックカメラ生成
	m_debugCamera = std::make_unique<DebugCamera>(m_outputWidth,m_outputHeight);

	//エフェクトファクトリ生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());

	//テクスチャの読み込みパス
	m_factory->SetDirectory(L"Resources");

	//モデルの読み込み
	m_modelGround = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/ground200m.cmo", *m_factory);
	m_objSkydome.LoadModel(L"Resources/skydome.cmo");
	m_modelBall = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/ball.cmo", *m_factory);

	/*m_modelTeapot = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/Teapot.cmo", *m_factory);

	m_modeltank = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/tanc.cmo", *m_factory);*/

	//球の回転変数の初期化
	rotateball = 0.0f;

	//ティーポットの平行移動の初期化
	for (int i = 0; i < 20; i++)
	{
		x[i] = 0.0f;
		z[i] = 0.0f;
	}

	//ティーポットの距離の初期化
	range = 0.0f;

	//ティーポットの角度の初期化
	angle = 0.0f;

	//タンクの角度の初期化
	tank_angle = 0.0f;

	//ティーポットの設定
	scalemat = Matrix::CreateScale(0.1f);

	//ワールド行列の計算
	for (int i = 0; i < 20; i++)
	{

		angle = rand() / XM_2PI;

		range = rand() % 100;

		x[i] = (float)cosf(angle)*range;

		z[i] = (float)sinf(angle)*range;

		rotmatY = Matrix::CreateRotationY(XMConvertToRadians(i * 0.5f));
		
		transmat = Matrix::CreateTranslation(x[i], 0, z[i]);

		m_worldTeapot[i] = scalemat *rotmatY* transmat;

	}

	//自機パーツの読み込み
	m_ObjPlayer.resize(PLAYER_PARTS_NUM);
	m_ObjPlayer[PLAYER_PARTS_TANC].LoadModel(L"Resources/tanc.cmo");
	m_ObjPlayer[PLAYER_PARTS_BODY].LoadModel(L"Resources/body.cmo");
	m_ObjPlayer[PLAYER_PARTS_WEPON_L].LoadModel(L"Resources/wepon.cmo");
	m_ObjPlayer[PLAYER_PARTS_WEPON_R].LoadModel(L"Resources/wepon.cmo");
	m_ObjPlayer[PLAYER_PARTS_FACE].LoadModel(L"Resources/face.cmo");

	//パーツの親子関係をセット
	m_ObjPlayer[PLAYER_PARTS_BODY].SetParent(&m_ObjPlayer[PLAYER_PARTS_TANC]);
	m_ObjPlayer[PLAYER_PARTS_WEPON_L].SetParent(&m_ObjPlayer[PLAYER_PARTS_BODY]);
	m_ObjPlayer[PLAYER_PARTS_WEPON_R].SetParent(&m_ObjPlayer[PLAYER_PARTS_BODY]);
	m_ObjPlayer[PLAYER_PARTS_FACE].SetParent(&m_ObjPlayer[PLAYER_PARTS_BODY]);

	//親からのオフセット(座標ずらし分)をセット
	m_ObjPlayer[PLAYER_PARTS_BODY].SetTranslation(Vector3(0.0f, 0.5f, 0.0f));
	m_ObjPlayer[PLAYER_PARTS_BODY].SetScale(Vector3(0.8f, 0.8f, 0.8f));

	m_ObjPlayer[PLAYER_PARTS_WEPON_L].SetTranslation(Vector3(-0.52f, 0.0f, -0.5f));
	m_ObjPlayer[PLAYER_PARTS_WEPON_R].SetTranslation(Vector3(0.52f, 0.0f, -0.5f));

	m_ObjPlayer[PLAYER_PARTS_FACE].SetTranslation(Vector3(0.0f, 0.0f, -0.45f));

	//ボールカウントの初期化
	ballcnt = 0;

	//フレームカウントの初期化
	frame = 0;

}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;

	//毎フレーム更新処理
	m_debugCamera->Update();
	
	//ビュー行列の取得
	//m_view = m_debugCamera->GetCameraMatrix();

	{//自機に追従するカメラ
		m_Camera->SetTargetpos(tank_pos);
		m_Camera->SetTargetAngle(tank_angle);

		//カメラの更新
		m_Camera->Update();
		m_view = m_Camera->GetView();
		m_proj = m_Camera->GetProj();
	}

	m_objSkydome.Update();
	
	/*//視点
	Vector3 eyepos(0,0,5.0f);
	//注視点
	Vector3 refpos(0, 0, 0);
	//上方向ベクトル
	static float angle = 0.0f;
	angle += 0.1f;
	Vector3 upvec(cosf(angle), sinf(angle), 0);

	upvec.Normalize();	//ベクトルの正規化(長さを1にする)

	//ビュー行列を生成
	m_view = Matrix::CreateLookAt(eyepos, refpos, upvec);*/

	/*//垂直方向視野角
	float fovY = XMConvertToRadians(60.0f);

	//アスペクト比(横・縦の比率)
	float aspect = (float)m_outputWidth / m_outputHeight;

	//手前の表示限界距離
	float nearclip = 0.1f;

	//奥の表示限界距離
	float farclip = 1000.0f;

	//射影行列の生成(透視投影)
	m_proj = Matrix::CreatePerspectiveFieldOfView(fovY, aspect, nearclip, farclip);*/
	
	//球のワールド行列の計算
	//スケーリング
	Matrix scalemat = Matrix::CreateScale(0.5f);

	//ロール
	Matrix rotmatZ = Matrix::CreateRotationZ(XMConvertToRadians(0.0f));

	//ピッチ（仰角）
	Matrix rotmatX = Matrix::CreateRotationX(XMConvertToRadians(0.0f));

	//ヨー（方位角）
	Matrix rotmatY = Matrix::CreateRotationY(XMConvertToRadians(0.0f));

	//回転行列（合成）
	Matrix rotmat = rotmatZ*rotmatX*rotmatY;

	//平行移動
	Matrix transmat = Matrix::CreateTranslation(20.0f, 0, .0);

	//球の回転
	rotateball += 0.5f;
	
	//ワールド行列の合成
	for(int i = 0; i < 20; i++)
	{
		if (i < 10)
		{
			//方位角の設定
			rotmatY = Matrix::CreateRotationY(XMConvertToRadians(i * 36.0f + rotateball));
		}
		else
		{
			//平行移動
			transmat = Matrix::CreateTranslation(40.0f, 0, .0);

			//方位角の設定
			rotmatY = Matrix::CreateRotationY(XMConvertToRadians(i * 36.0f - rotateball));
		}

		//回転行列（合成）
		Matrix rotmat = rotmatZ*rotmatX*rotmatY;

		//ワールド行列の計算
		m_worldBall[i] = scalemat * transmat * rotmat;
	}

	//ワールド行列の計算
	for (int i = 0; i < 20; i++)
	{

		rotmatY = Matrix::CreateRotationY(XMConvertToRadians(frame));

		transmat = Matrix::CreateTranslation(x[i], 0, z[i]);

		m_worldTeapot[i] = scalemat *rotmatY* transmat;

	}

	//フレームを増やす
	frame++;

	//自機パーツのギミック
	{
		//Vector3 angle = m_ObjPlayer[PLAYER_PARTS_HEAD].GetRotation();
		//m_ObjPlayer[PLAYER_PARTS_HEAD].SetRotation(angle += Vector3(0, 0.01f, 0));

		//Vector3 pos = m_ObjPlayer[PLAYER_PARTS_HEAD].GetTranslation();
		//m_ObjPlayer[PLAYER_PARTS_HEAD].SetRotation(pos += Vector3(0, 0.01f, 0));


	}
	
	//キーボードの状態取得処理
	Keyboard::State kb = keyboard->GetState();

	//Aキーが押されたら
	if (kb.A)
	{
		//自機の角度を変更
		//tank_angle += 0.03f;
		float angle = m_ObjPlayer[0].GetRotation().y;
		m_ObjPlayer[0].SetRotation(Vector3(0, angle+0.03f, 0));
	}

	//Dキーが押されたら
	if (kb.D)
	{
		//自機の角度を変更
		float angle = m_ObjPlayer[0].GetRotation().y;
		m_ObjPlayer[0].SetRotation(Vector3(0, angle - 0.03f, 0));
	}

	//Wキーが押されたら
	if (kb.W)
	{
		//移動ベクトル(Z座標後退)
		Vector3 moveV(0, 0, -0.1f);

		//移動量ベクトルを自機の角度分回転させる
		float angle = m_ObjPlayer[0].GetRotation().y;
		rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);

		//自機の座標を移動
		//tank_pos += moveV;
		Vector3 pos = m_ObjPlayer[0].GetTranslation();
		m_ObjPlayer[0].SetTranslation(pos + moveV);
	}

	//Sキーが押されたら
	if (kb.S)
	{
		//移動ベクトル(Z座標後退)
		Vector3 moveV(0, 0, 0.1f);

		//移動量ベクトルを自機の角度分回転させる
		float angle = m_ObjPlayer[0].GetRotation().y;
		rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);

		//自機の座標を移動
		//tank_pos += moveV;
		Vector3 pos = m_ObjPlayer[0].GetTranslation();
		m_ObjPlayer[0].SetTranslation(pos + moveV);
	}

	//スペースキーを押したら
	if (kb.Space)
	{
		m_Ball.push_back(new obj3d);
		m_Ball[ballcnt]->LoadModel(L"Resources/k_ball.cmo");

		Vector3 pos;

		pos = m_ObjPlayer[PLAYER_PARTS_FACE].GetTranslation() + m_ObjPlayer[PLAYER_PARTS_BODY].GetTranslation() + m_ObjPlayer[PLAYER_PARTS_TANC].GetTranslation();

		m_Ball[ballcnt]->SetTranslation(pos);

		Vector3 angle;
		angle = m_ObjPlayer[PLAYER_PARTS_TANC].GetRotation();

		m_Ball[ballcnt]->SetRotation(angle);

		Vector3 moveV(0.0f, 0.5f,-1.0f);

		moveV = Vector3::TransformNormal(moveV, Matrix::CreateRotationY(angle.y));

		m_Ball[ballcnt]->SetTranslation(pos + moveV);

		ballcnt++;

		/*Vector3 scale;
		Quaternion rotq;
		Vector3 pos;*/

		//m_ObjPlayer[PLAYER_PARTS_FACE].GetWorld().Decompose(scale, rotq, pos);

	}

	{//自機のワールド行列の計算
		//パーツ1(親)
		Matrix rotmat = Matrix::CreateRotationY(tank_angle);

		Matrix transmat = Matrix::CreateTranslation(tank_pos);

		//ワールド行列を合成
		/*tank_world = rotmat * transmat;

		//パーツ2(子)
		Matrix rotmat2 = Matrix::CreateRotationZ(XM_PIDIV2) * Matrix::CreateRotationY(0);

		Matrix transmat2 = Matrix::CreateTranslation(Vector3(0, 0.5f, 0));
		
		//ワールド行列を合成
		tank_world2 = rotmat2 * transmat2 * tank_world;*/

	}

	for (std::vector<obj3d>::iterator it = m_ObjPlayer.begin(); it != m_ObjPlayer.end(); it++)
	{
		it->Update();
	}

	//ボールのUpdate
	for (int i = 0; i < ballcnt; i++)
	{

		m_Ball[i]->Update();

	}
}

// Draws the scene.
void Game::Render()
{
	//頂点インデックス
	uint16_t indices[] =
	{
		0,1,2,
		2,1,3
	};

	//頂点座標
	VertexPositionNormal vertices[] =
	{				//座標					//法線方向ベクトル
		{ Vector3(-1.0f,1.0f ,0.0f), Vector3( 0.0f,0.0f,1.0f) },
		{ Vector3(-1.0f,-1.0f,0.0f), Vector3( 0.0f,0.0f,1.0f) },
		{ Vector3(1.0f,1.0f,  0.0f), Vector3( 0.0f,0.0f,1.0f) },
		{ Vector3(1.0f,-1.0f, 0.0f), Vector3( 0.0f,0.0f,1.0f) },
	};

    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    // TODO: Add your rendering code here.

	//描画処理
	DirectX::CommonStates m_states(m_d3dDevice.Get());

	m_d3dContext->OMSetBlendState(m_states.Opaque(), nullptr, 0xFFFFFFFF);
	m_d3dContext->OMSetDepthStencilState(m_states.DepthNone(), 0);
	m_d3dContext->RSSetState(m_states.CullNone());

	//m_effect->SetWorld(m_world);
	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

	m_effect->Apply(m_d3dContext.Get());
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	//地面モデルの描画
	m_modelGround->Draw(m_d3dContext.Get(),m_states,Matrix::Identity,m_view,m_proj);

	//天球モデルの描画
	m_objSkydome.Draw();

	//モデルの描画
	for (std::vector<obj3d>::iterator it = m_ObjPlayer.begin();it !=m_ObjPlayer.end();it++)
	{
		it->Draw();
	}

	//ボールの描画
	for (int i = 0; i < ballcnt; i++)
	{
  		m_Ball[i]->Draw();
	}

	//m_ObjPlayer.Draw();
	
	//タンクモデルの描画
	//m_ObjPlayer2.Draw();

	//ティーポットの描画
	/*for (int i = 0; i < 20; i++)
	{
		m_modelTeapot->Draw(m_d3dContext.Get(), m_states, m_worldTeapot[i], m_view, m_proj);
	}*/

	//球モデルの描画
	/*for (int i = 0; i < 20; i++)
	{
		m_modelBall->Draw(m_d3dContext.Get(), m_states, m_worldBall[i], m_view, m_proj);
	}*/

	m_batch->Begin();
	/*m_batch->DrawLine(
		VertexPositionColor(SimpleMath::Vector3(0,0,0),SimpleMath::Color(1,1,1)), 
		VertexPositionColor(SimpleMath::Vector3(800,600,0),SimpleMath::Color(1,1,1))
	);*/

	/*VertexPositionColor v1(Vector3(0, 500, 0), Colors::Red);
	VertexPositionColor v2(Vector3(500, 0, 0), Colors::Red);
	VertexPositionColor v3(Vector3(0, 0, 0), Colors::Red);*/

	/*VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::AliceBlue);
	VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::AliceBlue);
	VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::AliceBlue);*/

	//m_batch->DrawTriangle(v1, v2, v3);

	//m_batch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,indices, 6, vertices, 4);

	m_batch->End();

    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    HRESULT hr = D3D11CreateDevice(
        nullptr,                                // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        m_d3dDevice.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
        &m_featureLevel,                        // returns feature level of device created
        m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
        );

    if (hr == E_INVALIDARG)
    {
        // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
        hr = D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creationFlags,
            &featureLevels[1],
            _countof(featureLevels) - 1,
            D3D11_SDK_VERSION,
            m_d3dDevice.ReleaseAndGetAddressOf(),
            &m_featureLevel,
            m_d3dContext.ReleaseAndGetAddressOf()
            );
    }

    DX::ThrowIfFailed(hr);

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    // DirectX 11.1 if present
    if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
        (void)m_d3dContext.As(&m_d3dContext1);

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory1> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        ComPtr<IDXGIFactory2> dxgiFactory2;
        if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
        {
            // DirectX 11.1 or later

            // Create a descriptor for the swap chain.
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
            swapChainDesc.Width = backBufferWidth;
            swapChainDesc.Height = backBufferHeight;
            swapChainDesc.Format = backBufferFormat;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = backBufferCount;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
            fsSwapChainDesc.Windowed = TRUE;

            // Create a SwapChain from a Win32 window.
            DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
                m_d3dDevice.Get(),
                m_window,
                &swapChainDesc,
                &fsSwapChainDesc,
                nullptr,
                m_swapChain1.ReleaseAndGetAddressOf()
                ));

            DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
        }
        else
        {
            DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
            swapChainDesc.BufferCount = backBufferCount;
            swapChainDesc.BufferDesc.Width = backBufferWidth;
            swapChainDesc.BufferDesc.Height = backBufferHeight;
            swapChainDesc.BufferDesc.Format = backBufferFormat;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.OutputWindow = m_window;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.Windowed = TRUE;

            DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
        }

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain1.Reset();
    m_swapChain.Reset();
    m_d3dContext1.Reset();
    m_d3dContext.Reset();
    m_d3dDevice1.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}