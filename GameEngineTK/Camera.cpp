#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// コンストラクタ
/// </summary>

Camera::Camera(int width, int height)
{
	//メンバ変数の初期化
	m_eyepos = Vector3(0, 0, 5.0f);
	m_refpos = Vector3(0, 0, 0);
	m_upvec = Vector3(0, 1, 0);

	m_fovY = XMConvertToRadians(60.0f);
	m_aspect = (float)width / height;
	m_nearclip = 0.1f;
	m_farclip = 1000.0f;

	//ビュー行列の生成
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);

	//射影行列の生成(透視投影)
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearclip, m_farclip);

}

Camera::~Camera()
{

}

void Camera::Update()
{
	//ビュー行列の生成
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);

	//射影行列の生成(透視投影)
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearclip, m_farclip);



}
//ビュー行列の取得
const DirectX::SimpleMath::Matrix& Camera::GetView()
{
	return m_view;
}

//プロジェクション行列の取得
const DirectX::SimpleMath::Matrix& Camera::GetProj()
{
	return m_proj;
}

//視点のセット
void Camera::SetEyepos(const DirectX::SimpleMath::Vector3& eyepos)
{
	m_eyepos = eyepos;
}

//注視点のセット
void Camera::SetRefpos(const DirectX::SimpleMath::Vector3& refpos)
{
	m_refpos = refpos;
}

//上方向ベクトルのセット
void Camera::SetUpvec(const DirectX::SimpleMath::Vector3& upvec)
{
	m_upvec = upvec;
}

//垂直方向視野角のセット
void Camera::SetFovY(float fovY)
{
	m_fovY = fovY;
}

//アスペクト比のセット
void Camera::SetAspect(float aspect)
{
	m_aspect = aspect;
}

//手前の表示限界距離のセット
void Camera::SetNearclip(float nearclip)
{
	m_nearclip = nearclip;
}

//奥の表示限界距離のセット
void Camera::SetFarclip(float farclip)
{
	m_farclip = farclip;
}
