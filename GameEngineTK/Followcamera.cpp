#include "Followcamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//静的メンバ変数の初期化
const float Followcamera::CAMERA_DISTANCE = 5.0f;

Followcamera::Followcamera(int width, int height)
	:Camera(width,height)
{
	m_targetpos = Vector3::Zero;

	m_targetangle = 0.0f;

	m_keyboard = nullptr;

	m_isFPS = false;

}

//更新処理
void Followcamera::Update()
{
	//キーボードの状態を取得
	Keyboard::State keyboardstate = m_keyboard->GetState();
	m_KeyboardTracker.Update(keyboardstate);

	//Cキーを押すごとにTPSとFPSの切り替え
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keys::C))
	{
		//フラグの切り替え
		m_isFPS = !m_isFPS;

	}

	//視点、参照点
	Vector3 eyepos, refpos;
	if(m_isFPS)
	{//TPSカメラ処理

		//自機の上空2mを参照点とする
		refpos = m_targetpos + Vector3(0, 2.0f, 0);

		//参照点と視点の差分ベクトル
		Vector3 cameraV(0, 0, CAMERA_DISTANCE);

		//自機の後ろに回り込むための回転行列の生成
		Matrix rotmat = Matrix::CreateRotationY(m_targetangle);

		//差分ベクトルを回転させる
		cameraV = Vector3::TransformNormal(cameraV, rotmat);

		//視点座標を計算
		eyepos = refpos + cameraV;

		//視点を現在位置から補間する
		eyepos = m_eyepos + (eyepos - m_eyepos) * 0.05f;

		//参照点を現在位置から補間する
		refpos = m_refpos + (refpos - m_refpos) * 0.2f;

	}
	else
	{//FPSカメラ処理
		 //自機の目の位置当たりを視点とする
		 Vector3 position;
		 position = m_targetpos + Vector3(0, 0.2f, 0);
		 //eyepos = m_targetpos + Vector3(0, 0.2f, 0);

		 //参照点と視点の差分ベクトル
		 Vector3 cameraV(0, 0, -CAMERA_DISTANCE);

		 //自機の後ろに回り込むための回転行列の生成
		 Matrix rotmat = Matrix::CreateRotationY(m_targetangle);

		 //差分ベクトルを回転させる
		 cameraV = Vector3::TransformNormal(cameraV, rotmat);

		 eyepos = position + cameraV*0.1f;

		 //参照点座標を計算
		 refpos = position + cameraV;
	}

	//カメラに情報をセット
	SetEyepos(eyepos);
	SetRefpos(refpos);

	//基底クラスの更新
	Camera::Update();

}

//追従対象の座標をセット
void Followcamera::SetTargetpos(const DirectX::SimpleMath::Vector3 & targetpos)
{
	m_targetpos = targetpos;
}

//追従対象の角度をセット
void Followcamera::SetTargetAngle(float targetangle)
{
	m_targetangle = targetangle;
}

//キーボードをセット
void Followcamera::SetKeyboard(DirectX::Keyboard * keyboard)
{
	m_keyboard = keyboard;
}
