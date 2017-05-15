//--------------------------------
//カメラを制御するクラス
//--------------------------------
#pragma once

#include<d3d11.h>
#include<SimpleMath.h>

class Camera
{
public:
	//メンバ関数
	Camera(int width, int height);
	virtual ~Camera();

	virtual void Update();

	//ビュー行列の取得
	const DirectX::SimpleMath::Matrix& GetView();

	//プロジェクション行列の取得
	const DirectX::SimpleMath::Matrix& GetProj();

	//視点のセット
	void SetEyepos(const DirectX::SimpleMath::Vector3& eyepos);

	//注視点のセット
	void SetRefpos(const DirectX::SimpleMath::Vector3& refpos);

	//上方向ベクトルのセット
	void SetUpvec(const DirectX::SimpleMath::Vector3& upvec);

	//垂直方向視野角のセット
	void SetFovY(float fovY);

	//アスペクト比のセット
	void SetAspect(float aspect);

	//手前の表示限界距離のセット
	void SetNearclip(float nearclip);

	//奥の表示限界距離のセット
	void SetFarclip(float farclip);

protected:
	//メンバ変数

	//ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	//視点
	DirectX::SimpleMath::Vector3 m_eyepos;

	//注視点
	DirectX::SimpleMath::Vector3 m_refpos;

	//上方向ベクトル
	DirectX::SimpleMath::Vector3 m_upvec;

	//プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;

	//垂直方向視野角
	float m_fovY;

	//アスペクト比
	float m_aspect;

	//手前の表示限界距離
	float m_nearclip;

	//奥の表示限界距離
	float m_farclip;

};
