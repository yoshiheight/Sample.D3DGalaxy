/**
 * @file
 * 太陽系モデルの定義
 */
#include "StdAfx.h"
#include "Galaxy.h"
#include "vector"

// スピード定数
const float Galaxy::MOVE_SPEED = 100.0f;

/**
 * コンストラクタ
 */
Galaxy::Galaxy()
    :   m_pPlanet(NULL),
        m_moveX(0.0f),          // X軸移動量
        m_moveY(0.0f),          // Y軸移動量
        m_moveZ(0.0f),          // Z軸移動量
        m_revolutionScale(1.000f),
        m_isF6(FALSE),
        m_isFixedSize(FALSE)
{
}

/**
 * デストラクタ
 */
Galaxy::~Galaxy()
{
    Release();
}

/** 
 * 太陽系を生成
 * @param pD3DDevice DirectX3Dデバイス
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL Galaxy::Create(LPDIRECT3DDEVICE9 pD3DDevice, const PLANET_INFO* pPlanetInfoTable)
{
    Release();

    std::map<INT32, Planet*> nestMap;
    for (INT32 i = 0; 0 <= pPlanetInfoTable[i].nestLevel; i++)
    {
        // 惑星を生成
        Planet* pPlanet = new Planet(this, &pPlanetInfoTable[i]);
        if (!pPlanet->Create(pD3DDevice))
        {
            return FALSE;
        }

        // 惑星を階層マップに登録
        nestMap[pPlanetInfoTable[i].nestLevel] = pPlanet;

        // 惑星を親に追加
        if (0 < pPlanetInfoTable[i].nestLevel)
        {
            nestMap[pPlanetInfoTable[i].nestLevel - 1]->Add(pPlanet);
        }
    }

    // ルート惑星を保持
    m_pPlanet = nestMap[0];

    return TRUE;
}

// ステータス文字列を取得
std::wstring Galaxy::GetStatus() const
{
    /*
    TCHAR szFPS[1000];
    SPRINTF(szFPS, 1000,
            _T("モデル位置 ： X=%f, Y=%f, Z=%f\n")
            _T("モデル回転 ： X=%f, Y=%f, Z=%f\n"),
            m_moveX, m_moveY, m_moveZ,
            m_rotateX, m_rotateY, m_rotateZ);

    return szFPS;
    */
    return _T("");
}

/**
 * フィールドの破棄
 */
void Galaxy::Release()
{
    SAFE_DELETE(m_pPlanet);
}

/**
 * 更新
 */
void Galaxy::Update(float timeSpan)
{
    // 公転半径スケール
    if (::GetKeyState('Q') < 0)
    {
        m_revolutionScale -= timeSpan * 0.1f;
    }
    else if (::GetKeyState('W') < 0)
    {
        m_revolutionScale += timeSpan * 0.1f;
    }

    // 切り替え
    if (::GetKeyState(VK_F6) < 0)
    {
        // 初回のF6押下の場合
        if (!m_isF6)
        {
            m_isFixedSize = !m_isFixedSize;
        }
        m_isF6 = TRUE;
    }
    else
    {
        m_isF6 = FALSE;
    }

    // 移動
    if (::GetKeyState(VK_LEFT) < 0)
    {
        m_moveX -= timeSpan * MOVE_SPEED;
    }
    else if (::GetKeyState(VK_RIGHT) < 0)
    {
        m_moveX += timeSpan * MOVE_SPEED;
    }
    else if (::GetKeyState(VK_DOWN) < 0)
    {
        m_moveY -= timeSpan * MOVE_SPEED;
    }
    else if (::GetKeyState(VK_UP) < 0)
    {
        m_moveY += timeSpan * MOVE_SPEED;
    }
    else if (::GetKeyState(VK_NEXT) < 0)
    {
        m_moveZ -= timeSpan * MOVE_SPEED;
    }
    else if (::GetKeyState(VK_PRIOR) < 0)
    {
        m_moveZ += timeSpan * MOVE_SPEED;
    }

    // 移動行列
    D3DXMatrixIdentity(&m_mWorld); // 単位行列
    ::D3DXMatrixTranslation(&m_mWorld, m_moveX, m_moveY, m_moveZ);

    m_pPlanet->Update(timeSpan, &m_mWorld);
}

/** 
 * フィールドを描画
 * @param pD3DDevice DirectX3Dデバイス
 */
void Galaxy::Draw(LPDIRECT3DDEVICE9 pD3DDevice, D3DCAPS9* pD3DCaps)
{
    //pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
    //pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

    // スペキュラーON
//  pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

    // 惑星を描画
    m_pPlanet->Draw(pD3DDevice, pD3DCaps, &m_mWorld);

    // 元に戻す
//  pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
}
