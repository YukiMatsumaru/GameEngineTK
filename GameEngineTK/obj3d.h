/// <summary>
/// 3Dオブジェクトのクラス
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
	//静的メンバ
private:
	//デバイス
	static Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;

	//デバイスコンテキスト
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;

	//カメラ
	static Camera* m_Camera;

	//汎用描画States
	static std::unique_ptr<DirectX::CommonStates> m_states;

	//エフェクトファクトリー
	static std::unique_ptr<DirectX::EffectFactory> m_factory;

public:
	//静的メンバ変数の初期化
	static void InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice,
								 Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, 
		                         Camera* camera);

private:
	//3Dモデル
	std::unique_ptr<DirectX::Model> m_model;

	//スケーリング (XYZ)
	DirectX::SimpleMath::Vector3	m_scale;

	//回転角 (XYZ)
	DirectX::SimpleMath::Vector3	m_rotation;

	//平行移動 (XYZ)
	DirectX::SimpleMath::Vector3	m_translation;

	//ワールド行列
	DirectX::SimpleMath::Matrix		m_world;

	//親の３Dオブジェクトのポインタ
	obj3d* m_parent;

public:
	//コンストラクタ
	obj3d();

	//モデルのロード
	void LoadModel(const wchar_t* fileName);

	//更新
	void Update();

	//描画
	void Draw();

	//setter

	//スケーリング (XYZ)
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }

	//回転角 (XYZ)
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation) { m_rotation= rotation; }

	//平行移動 (XYZ)
	void SetTranslation(const DirectX::SimpleMath::Vector3& translation) { m_translation = translation; }

	//親の３Dオブジェクトのポインタ
	void SetParent(obj3d* parent) {m_parent = parent; }

	//getter

	//スケーリング (XYZ)
	const DirectX::SimpleMath::Vector3& GetScale() { return m_scale; }

	//回転角 (XYZ)
	const DirectX::SimpleMath::Vector3& GetRotation() { return m_rotation; }


	//平行移動 (XYZ)
	const DirectX::SimpleMath::Vector3& GetTranslation() { return m_translation; }

	//親の３Dオブジェクトのポインタ
	obj3d* GetParent() { return m_parent; }

	//ワールド行列
	const DirectX::SimpleMath::Matrix& GetWorld() { return m_world; }


};