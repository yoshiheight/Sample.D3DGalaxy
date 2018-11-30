/**
 * @file
 * 標準カメラの定義
 */
#include "StdAfx.h"
#include "NormalCamera.h"

// スピード定数
const float NormalCamera::MOVE_SPEED    = 30.0f;
const float NormalCamera::ROTATE_SPEED  = 50.0f;
const float NormalCamera::ZOOM_SPEED    = 20.0f;

/**
 * コンストラクタ
 */
NormalCamera::NormalCamera()
    :   m_moveX(-100.0f),   // カメラのX軸移動量
        m_moveY(50.0f),     // カメラのY軸移動量
        m_moveZ(-10.0f),    // カメラのZ軸移動量
        m_rotateX(20.0f),   // カメラのX軸回転角度
        m_rotateY(90.0f)    // カメラのY軸回転角度
{
}

/**
 * デストラクタ
 */
NormalCamera::~NormalCamera()
{
}

/** 
 * カメラを生成
 * @param pD3DDevice DirectX3Dデバイス
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL NormalCamera::Init(LPDIRECT3DDEVICE9 pD3DDevice)
{
    // ビューポートを取得
    D3DVIEWPORT9 vp;
    if (FAILED(pD3DDevice->GetViewport(&vp))) {
        return FALSE;
    }

    // ビューポートのアスペクト比を取得
    m_aspect = (float) vp.Width / (float) vp.Height;

    return TRUE;
}

std::wstring NormalCamera::GetStatus()
{
    TCHAR szFPS[1000];
    SPRINTF(szFPS, 1000,
            _T("カメラ位置 ： X=%f, Y=%f, Z=%f\n")
            _T("カメラ回転 ： X=%f, Y=%f\n")
            _T("カメラ画角 ： %f\n"),
            m_moveX, m_moveY, m_moveZ,
            m_rotateX, m_rotateY,
            m_zoom);
    return szFPS;
}

/**
 * 更新
 */
void NormalCamera::Update(float timeSpan)
{
    BOOL isCtrl = (::GetKeyState(VK_CONTROL) < 0);

    // カメラ移動、回転、ズーム
    if (::GetKeyState(VK_NUMPAD1) < 0)
    {
        if (isCtrl)
        {
            m_rotateY -= timeSpan * ROTATE_SPEED;
        }
        else
        {
            m_moveX -= timeSpan * MOVE_SPEED;
        }
    }
    else if (::GetKeyState(VK_NUMPAD3) < 0)
    {
        if (isCtrl)
        {
            m_rotateY += timeSpan * ROTATE_SPEED;
        }
        else
        {
            m_moveX += timeSpan * MOVE_SPEED;
        }
    }
    else if (::GetKeyState(VK_NUMPAD2) < 0)
    {
        if (isCtrl)
        {
            m_rotateX += timeSpan * ROTATE_SPEED;
        }
        else
        {
            m_moveY -= timeSpan * MOVE_SPEED;
        }
    }
    else if (::GetKeyState(VK_NUMPAD5) < 0)
    {
        if (isCtrl)
        {
            m_rotateX -= timeSpan * ROTATE_SPEED;
        }
        else
        {
            m_moveY += timeSpan * MOVE_SPEED;
        }
    }
    else if (::GetKeyState(VK_NUMPAD6) < 0)
    {
        if (!isCtrl)
        {
            m_moveZ -= timeSpan * MOVE_SPEED;
        }
    }
    else if (::GetKeyState(VK_NUMPAD9) < 0)
    {
        if (!isCtrl)
        {
            m_moveZ += timeSpan * MOVE_SPEED;
        }
    }
    else if (::GetKeyState(VK_ADD) < 0)
    {
        m_zoom -= timeSpan * ZOOM_SPEED;
    }
    else if (::GetKeyState(VK_SUBTRACT) < 0)
    {
        m_zoom += timeSpan * ZOOM_SPEED;
    }
}

void NormalCamera::GetMatrix(D3DXMATRIX* pMatrix)
{
    // 回転
    D3DXMATRIX mRotateX;
    D3DXMatrixIdentity(&mRotateX);  // 単位行列
    ::D3DXMatrixRotationX(&mRotateX, D3DXToRadian(m_rotateX));

    D3DXMATRIX mRotateY;
    D3DXMatrixIdentity(&mRotateY);  // 単位行列
    ::D3DXMatrixRotationY(&mRotateY, D3DXToRadian(m_rotateY));

    D3DXMATRIX mRotate;
    D3DXMatrixIdentity(&mRotate);   // 単位行列

    mRotate *= mRotateX;
    mRotate *= mRotateY;

    // 注視点を算出
    D3DXVECTOR3 vLook(0.0f, 0.0f, 1.0f);
    ::D3DXVec3TransformCoord(&vLook, &vLook, &mRotate);
    vLook.x += m_moveX;
    vLook.y += m_moveY;
    vLook.z += m_moveZ;

    // LookAtLH
    ::D3DXMatrixIdentity(pMatrix);
    ::D3DXMatrixLookAtLH(pMatrix, &D3DXVECTOR3(m_moveX, m_moveY, m_moveZ), // カメラ位置
                                  &vLook,   // 注視点
                                  &D3DXVECTOR3(0.0f, 1.0f, 0.0f));  // カメラの上方向ベクトル
}

/** 
 * カメラを描画
 * @param pD3DDevice DirectX3Dデバイス
 */
void NormalCamera::Transform(LPDIRECT3DDEVICE9 pD3DDevice)
{
    // ビュー行列

    D3DXMATRIX mView;
    GetMatrix(&mView);

    pD3DDevice->SetTransform(D3DTS_VIEW, &mView);

    // 射影行列
    D3DXMATRIX mProj;
    D3DXMatrixIdentity(&mProj);
    D3DXMatrixPerspectiveFovLH(&mProj,
                            D3DXToRadian(m_zoom),   // 画角
                            m_aspect,               // アスペクト比
                            0.5f,                   // Nearプレーン
                            1000000.0f);            // Farプレーン

    pD3DDevice->SetTransform(D3DTS_PROJECTION, &mProj);
}
