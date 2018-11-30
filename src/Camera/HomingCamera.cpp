/**
 * @file
 * 自動誘導カメラの実装
 */
#include "StdAfx.h"
#include "HomingCamera.h"

// スピード定数
const float HomingCamera::MOVE_SPEED    = 3.0f;
const float HomingCamera::ROTATE_SPEED  = 50.0f;
const float HomingCamera::ZOOM_SPEED    = 50.0f;

/// 角度の正規化
static void NormalizeDegree(float* pDegree)
{
    // 0以上、360未満にする

    if ((*pDegree) < 0.0f)
    {
        while ((*pDegree) < 0.0f)
        {
            (*pDegree) += 360.0f;
        }
    }
    else
    {
        while (360.0f <= (*pDegree))
        {
            (*pDegree) -= 360.0f;
        }
    }
}

/**
 * コンストラクタ
 */
HomingCamera::HomingCamera()
    :   m_isMoving(FALSE),
        m_currentTime(0.0f)
{
}

/**
 * デストラクタ
 */
HomingCamera::~HomingCamera()
{
}

/** 
 * カメラを生成
 * @param pD3DDevice DirectX3Dデバイス
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL HomingCamera::Init(LPDIRECT3DDEVICE9 pD3DDevice, IHomingCameraTarget* pTarget)
{
    // ビューポートを取得
    D3DVIEWPORT9 vp;
    if (FAILED(pD3DDevice->GetViewport(&vp))) {
        return FALSE;
    }

    m_pTarget = pTarget;

    // ビューポートのアスペクト比を取得
    m_aspect = (float) vp.Width / (float) vp.Height;

    m_CurrentPos.vOrg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_CurrentPos.angleX = m_pTarget->GetAngleX();
    m_CurrentPos.angleY = m_pTarget->GetAngleY();
    m_CurrentPos.distance = m_pTarget->GetDistance();

    m_EndPos = m_CurrentPos;
    m_StartPos = m_CurrentPos;

    return TRUE;
}

std::wstring HomingCamera::GetStatus()
{
    D3DXVECTOR3 v = m_CurrentPos.vOrg - m_vPos;
    TCHAR szFPS[1000];
    SPRINTF(szFPS, 1000,
        _T("注視点 ： %f, %f, %f\n")
        _T("対象との距離 ： %f\n")
        _T("v ： %f, %f, %f\n")
        _T("時間 ： %f\n"),
        m_CurrentPos.vOrg.x, m_CurrentPos.vOrg.y, m_CurrentPos.vOrg.z,
        m_CurrentPos.distance,
        v.x, v.y, v.z,
        m_currentTime);
    return szFPS;
}

/**
 * 追跡対象の設定
 */
void HomingCamera::SetTarget(IHomingCameraTarget* pTarget)
{
    if (pTarget != m_pTarget)
    {
        m_isMoving = TRUE;
        m_startTime = 2.0f;
        m_currentTime = m_startTime;
        m_pTarget = pTarget;
        m_StartPos = m_CurrentPos;

        D3DXVECTOR3 vOrgNext;
        m_pTarget->GetWorldPos(&vOrgNext);
        m_EndPos.vOrg = vOrgNext;
        m_EndPos.distance = m_pTarget->GetDistance();
        m_EndPos.angleX = m_pTarget->GetAngleX();
        m_EndPos.angleY = m_pTarget->GetAngleY();

        // 角度を正規化
        ::NormalizeDegree(&(m_StartPos.angleX));
        ::NormalizeDegree(&(m_StartPos.angleY));
        ::NormalizeDegree(&(m_EndPos.angleX));
        ::NormalizeDegree(&(m_EndPos.angleY));

        // 短いルートで回転させる為の前処理
        if (180.0f < fabs(m_EndPos.angleX - m_StartPos.angleX))
        {
            INT32 sign = (m_StartPos.angleX < m_EndPos.angleX) ? -1 : 1;
            m_EndPos.angleX += (360.0f * sign);
        }

        if (180.0f < fabs(m_EndPos.angleY - m_StartPos.angleY))
        {
            INT32 sign = (m_StartPos.angleY < m_EndPos.angleY) ? -1 : 1;
            m_EndPos.angleY += (360.0f * sign);
        }
    }
}

/**
 * 更新
 */
void HomingCamera::Update(float timeSpan)
{
    if (m_isMoving)
    {
        // ゴール位置
        D3DXVECTOR3 vOrgNext;
        m_pTarget->GetWorldPos(&vOrgNext);
        m_EndPos.vOrg = vOrgNext;

        // 時間レート
        m_currentTime -= timeSpan;
        float late = (m_startTime - m_currentTime) / m_startTime;

        // 移動用に変換
        late = SIN(D3DXToRadian(late * 90.0f));

        // 移動距離
        D3DXVECTOR3 v = (m_EndPos.vOrg - m_StartPos.vOrg) * late;
        float offset = (m_EndPos.distance - m_StartPos.distance) * late;
        float offsetAngleX = (m_EndPos.angleX - m_StartPos.angleX) * late;
        float offsetAngleY = (m_EndPos.angleY - m_StartPos.angleY) * late;

        // 移動
        if (0.0f < m_currentTime)
        {
            m_CurrentPos.vOrg = m_StartPos.vOrg + v;
            m_CurrentPos.distance = m_StartPos.distance + offset;
            m_CurrentPos.angleX = m_StartPos.angleX + offsetAngleX;
            m_CurrentPos.angleY = m_StartPos.angleY + offsetAngleY;
        }
        else
        {
            m_CurrentPos = m_EndPos;
            m_isMoving = FALSE;
        }
    }
    else
    {
        // カメラ移動、回転、ズーム
        if (::GetKeyState(VK_NUMPAD1) < 0)
        {
            m_CurrentPos.angleY += timeSpan * ROTATE_SPEED;
        }
        else if (::GetKeyState(VK_NUMPAD3) < 0)
        {
            m_CurrentPos.angleY -= timeSpan * ROTATE_SPEED;
        }
        else if (::GetKeyState(VK_NUMPAD2) < 0)
        {
            m_CurrentPos.angleX -= timeSpan * ROTATE_SPEED;
        }
        else if (::GetKeyState(VK_NUMPAD5) < 0)
        {
            m_CurrentPos.angleX += timeSpan * ROTATE_SPEED;
        }
        else if (::GetKeyState(VK_NUMPAD6) < 0)
        {
//          m_CurrentPos.distance += m_CurrentPos.distance * timeSpan * MOVE_SPEED;
        }
        else if (::GetKeyState(VK_NUMPAD9) < 0)
        {
//          m_CurrentPos.distance -= m_CurrentPos.distance * timeSpan * MOVE_SPEED;
        }
        else if (::GetKeyState(VK_ADD) < 0)
        {
            m_zoom -= timeSpan * ZOOM_SPEED;
        }
        else if (::GetKeyState(VK_SUBTRACT) < 0)
        {
            m_zoom += timeSpan * ZOOM_SPEED;
        }

        // 現在の注視星のワールド座標
        m_pTarget->GetWorldPos(&(m_CurrentPos.vOrg));
        m_CurrentPos.distance = m_pTarget->GetDistance();
        /*
        m_CurrentPos.angleX = m_pTarget->GetAngleX();
        m_CurrentPos.angleY = m_pTarget->GetAngleY();
        */
    }

    // カメラ移動で使用する、実原点との差分移動の行列
    D3DXMATRIX mMove;
    ::D3DXMatrixIdentity(&mMove);
    ::D3DXMatrixTranslation(&mMove, m_CurrentPos.vOrg.x, m_CurrentPos.vOrg.y, m_CurrentPos.vOrg.z);

    // 回転
    D3DXMATRIX mRotate;
    ::D3DXMatrixIdentity(&mRotate);
    ::D3DXMatrixRotationYawPitchRoll(&mRotate,
                                    D3DXToRadian(m_CurrentPos.angleY),
                                    D3DXToRadian(m_CurrentPos.angleX),
                                    0.0f);

    D3DXMATRIX mWorld;
    ::D3DXMatrixIdentity(&mWorld);
    mWorld *= mRotate;
    mWorld *= mMove;

    // カメラ位置
    m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_vPos.z -= m_CurrentPos.distance;
    ::D3DXVec3TransformCoord(&m_vPos, &m_vPos, &mWorld);

    ::D3DXMatrixIdentity(&m_mView);
    ::D3DXMatrixLookAtLH(&m_mView,
                        &m_vPos,                            // カメラ位置
                        &m_CurrentPos.vOrg,                 // 注視点
                        &D3DXVECTOR3(0.0f, 1.0f, 0.0f));    // カメラの上方向ベクトル
}

/** 
 * カメラを描画
 * @param pD3DDevice DirectX3Dデバイス
 */
void HomingCamera::Transform(LPDIRECT3DDEVICE9 pD3DDevice)
{
    pD3DDevice->SetTransform(D3DTS_VIEW, &m_mView);

    // 射影行列
    D3DXMATRIX mProj;
    D3DXMatrixIdentity(&mProj);
    D3DXMatrixPerspectiveFovLH(&mProj,
                            D3DXToRadian(m_zoom),   // 画角
                            m_aspect,               // アスペクト比
                            0.001f,                 // Nearプレーン
                            10000.0f);              // Farプレーン
//                          0.005f,                 // Nearプレーン
//                          10.0f);                 // Farプレーン
    pD3DDevice->SetTransform(D3DTS_PROJECTION, &mProj);
}
