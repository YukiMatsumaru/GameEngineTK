/// <summary>
/// 自機に追従するカメラ
/// </summary>
/// 
#pragma once

#include "Camera.h"
#include <Keyboard.h>

class Followcamera :public Camera
{
protected:
	//追従対象の座標
	DirectX::SimpleMath::Vector3 m_targetpos;

	//追従対象の回転角
	float m_targetangle;

	//FPSカメラフラグ
	bool m_isFPS;

	//キーボード
	DirectX::Keyboard* m_keyboard;

	//キーボード(トラッカー)
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

public:
	//自機とカメラの距離
	static const float CAMERA_DISTANCE;

	//コンストラクタ
	Followcamera(int width, int height);

	//毎フレーム更新
	void Update() override;

	//追従対象の座標をセット
	void SetTargetpos(const DirectX::SimpleMath::Vector3& targetpos);

	//追従対象の角度をセット
	void SetTargetAngle(float targetangle);

	//キーボードをセット
	void SetKeyboard(DirectX::Keyboard* keyboard);

};