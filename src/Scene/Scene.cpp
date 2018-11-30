/**
 * @file
 * シーン管理の定義
 */
#include "StdAfx.h"
#include "GameApp.h"
#include "Scene.h"
/*
// 惑星テーブル
const PLANET_INFO g_PlanetInfoTable[] =
{
    // 階層 直径        公転半径        公転周期    AngleX  AngleY  色                                  輪
    { 0,    139.2f,     0.0f,           0.0f,       20.0f,  5.0f,   D3DCOLOR_ARGB(255, 255, 0, 0),      FALSE,  _T("") },       // 太陽
    { 1,    0.48794f,   5790.933548f,   0.241f,     20.0f,  350.0f, D3DCOLOR_ARGB(255, 153, 51, 0),     FALSE,  _T("resource/mercury.bmp") },   // 水星
    { 1,    1.21036f,   10820.86273f,   0.615f,     20.0f,  5.0f,   D3DCOLOR_ARGB(255, 255, 128, 64),   FALSE,  _T("resource/venus.bmp") },     // 金星
    { 1,    1.27563f,   14959.787f,     1.0f,       20.0f,  5.0f,   D3DCOLOR_ARGB(255, 51, 102, 255),   FALSE,  _T("resource/earth.bmp") },     // 地球
    { 2,    0.3474f,    38.44f,         0.0748f,    20.0f,  0.0f,   D3DCOLOR_ARGB(255, 255, 255, 0),    FALSE,  _T("resource/moon.bmp") },      // 月
    { 1,    0.67944f,   22793.62906f,   1.881f,     20.0f,  85.0f,  D3DCOLOR_ARGB(255, 204, 0, 0),      FALSE,  _T("resource/mars.bmp") },      // 火星
    { 1,    14.2984f,   77841.15728f,   11.86f,     20.0f,  5.0f,   D3DCOLOR_ARGB(255, 255, 153, 51),   FALSE,  _T("resource/jupiter.bmp") },   // 木星
    { 1,    12.0536f,   142672.5358f,   29.46f,     20.0f,  15.0f,  D3DCOLOR_ARGB(255, 255, 204, 0),    TRUE,   _T("resource/saturn.bmp") },    // 土星
    { 1,    5.1118f,    287098.957f,    84.01f,     20.0f,  85.0f,  D3DCOLOR_ARGB(255, 153, 204, 255),  FALSE,  _T("resource/uranus.bmp") },    // 天王星
    { 1,    4.9572f,    449825.2369f,   164.79f,    20.0f,  65.0f,  D3DCOLOR_ARGB(255, 0, 153, 255),    FALSE,  _T("resource/neptune.bmp") },   // 海王星
    { 1,    0.2306f,    590088.7982f,   248.1f,     20.0f,  0.0f,   D3DCOLOR_ARGB(255, 102, 51, 0),     FALSE,  _T("resource/pluto.bmp") },     // 冥王星
    { -1,   0.0f,       0.0f,           0.0f,       20.0f,  95.0f,  D3DCOLOR_ARGB(0, 0, 0, 0),          FALSE,  _T("") }
};
*/

/*
// 惑星テーブル
const PLANET_INFO g_PlanetInfoTable[] =
{
    // 階層 直径        公転半径        公転周期    AngleX  AngleY  色                                  輪
    { 0,    13.92f,     0.0f,           0.0f,       20.0f,  5.0f,   D3DCOLOR_ARGB(255, 255, 0, 0),      FALSE,  _T("") },       // 太陽
    { 1,    0.048794f,  579.0933548f,   0.241f,     20.0f,  350.0f, D3DCOLOR_ARGB(255, 153, 51, 0),     FALSE,  _T("resource/mercury.bmp") },   // 水星
    { 1,    0.121036f,  1082.086273f,   0.615f,     20.0f,  5.0f,   D3DCOLOR_ARGB(255, 255, 128, 64),   FALSE,  _T("resource/venus.bmp") },     // 金星
    { 1,    0.127563f,  1495.9787f,     1.0f,       20.0f,  5.0f,   D3DCOLOR_ARGB(255, 51, 102, 255),   FALSE,  _T("resource/earth.bmp") },     // 地球
    { 2,    0.03474f,   3.844f,         0.0748f,    20.0f,  0.0f,   D3DCOLOR_ARGB(255, 255, 255, 0),    FALSE,  _T("resource/moon.bmp") },      // 月
    { 1,    0.067944f,  2279.362906f,   1.881f,     20.0f,  85.0f,  D3DCOLOR_ARGB(255, 204, 0, 0),      FALSE,  _T("resource/mars.bmp") },      // 火星
    { 1,    1.42984f,   7784.115728f,   11.86f,     20.0f,  5.0f,   D3DCOLOR_ARGB(255, 255, 153, 51),   FALSE,  _T("resource/jupiter.bmp") },   // 木星
    { 1,    1.20536f,   14267.25358f,   29.46f,     20.0f,  15.0f,  D3DCOLOR_ARGB(255, 255, 204, 0),    TRUE,   _T("resource/saturn.bmp") },    // 土星
    { 1,    0.51118f,   28709.8957f,    84.01f,     20.0f,  85.0f,  D3DCOLOR_ARGB(255, 153, 204, 255),  FALSE,  _T("resource/uranus.bmp") },    // 天王星
    { 1,    0.49572f,   44982.52369f,   164.79f,    20.0f,  65.0f,  D3DCOLOR_ARGB(255, 0, 153, 255),    FALSE,  _T("resource/neptune.bmp") },   // 海王星
    { 1,    0.02306f,   59008.87982f,   248.1f,     20.0f,  0.0f,   D3DCOLOR_ARGB(255, 102, 51, 0),     FALSE,  _T("resource/pluto.bmp") },     // 冥王星
    { -1,   0.00f,      0.0f,           0.0f,       20.0f,  95.0f,  D3DCOLOR_ARGB(0, 0, 0, 0),          FALSE,  _T("") }
};
*/

// 惑星テーブル
const PLANET_INFO g_PlanetInfoTable[] =
{
    // 階層 直径            公転半径            公転周期        AngleX  AngleY  色                                  輪
    { 0,    1.392f,     0.0f,           0.0f,       20.0f,  5.0f,   D3DCOLOR_ARGB(255, 255, 0, 0),      FALSE,  _T("") },       // 太陽
    { 1,    0.0048794f, 57.90933548f,   0.241f,     -20.0f, 350.0f, D3DCOLOR_ARGB(255, 153, 51, 0),     FALSE,  _T("resource/mercury.bmp") },   // 水星
    { 1,    0.0121036f, 108.2086273f,   0.615f,     15.0f,  5.0f,   D3DCOLOR_ARGB(255, 255, 128, 64),   FALSE,  _T("resource/venus.bmp") },     // 金星
    { 1,    0.0127563f, 149.59787f,     1.0f,       5.0f,   -5.0f,  D3DCOLOR_ARGB(255, 51, 102, 255),   FALSE,  _T("resource/earth.bmp") },     // 地球
    { 2,    0.003474f,  0.3844f,        0.0748f,    0.0f,   0.0f,   D3DCOLOR_ARGB(255, 255, 255, 0),    FALSE,  _T("resource/moon.bmp") },      // 月
    { 1,    0.0067944f, 227.9362906f,   1.881f,     5.0f,   85.0f,  D3DCOLOR_ARGB(255, 204, 0, 0),      FALSE,  _T("resource/mars.bmp") },      // 火星
    { 1,    0.142984f,  778.4115728f,   11.86f,     30.0f,  5.0f,   D3DCOLOR_ARGB(255, 255, 153, 51),   FALSE,  _T("resource/jupiter.bmp") },   // 木星
    { 1,    0.120536f,  1426.725358f,   29.46f,     10.0f,  15.0f,  D3DCOLOR_ARGB(255, 255, 204, 0),    TRUE,   _T("resource/saturn.bmp") },    // 土星
    { 1,    0.051118f,  2870.98957f,    84.01f,     30.0f,  85.0f,  D3DCOLOR_ARGB(255, 153, 204, 255),  FALSE,  _T("resource/uranus.bmp") },    // 天王星
    { 1,    0.049572f,  4498.252369f,   164.79f,    10.0f,  65.0f,  D3DCOLOR_ARGB(255, 0, 153, 255),    FALSE,  _T("resource/neptune.bmp") },   // 海王星
    { 1,    0.002306f,  5900.887982f,   248.1f,     30.0f,  0.0f,   D3DCOLOR_ARGB(255, 102, 51, 0),     FALSE,  _T("resource/pluto.bmp") },     // 冥王星
    { -1,   0.000f,     0.0f,           0.0f,       10.0f,  95.0f,  D3DCOLOR_ARGB(0, 0, 0, 0),          FALSE,  _T("") }
};

/**
 * コンストラクタ
 */
Scene::Scene()
    :   m_pD3DFont(NULL),
        m_isF3(FALSE),
        m_isF5(FALSE),
        m_isF7(FALSE),
        m_isSolid(TRUE),
        m_currentPlanetIndex(-1),
        m_pCurrentCamera(NULL)
{
}

/**
 * デストラクタ
 */
Scene::~Scene()
{
    Release();
}

/** 
 * シーンを生成
 * @param pD3DDevice DirectX3Dデバイス
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL Scene::Create(LPDIRECT3DDEVICE9 pD3DDevice)
{
    Release();

    if(pD3DDevice == NULL) {
        return FALSE;
    }

    m_CoordAxis.Create(pD3DDevice);
    m_Galaxy.Create(pD3DDevice, g_PlanetInfoTable);

    m_Stardust.Create(pD3DDevice);
    m_HomingCamera.Init(pD3DDevice, GetCurrentPlanet());
    m_NormCamera.Init(pD3DDevice);
    m_pCurrentCamera = &m_HomingCamera;

    // フォントの生成
    if (FAILED(D3DXCreateFont(pD3DDevice,
                            16,                 // 高さ
                            0,                  // 平均文字幅
                            FW_NORMAL,          // 太さ
                            1,                  // ミップマップレベル
                            FALSE,              // 斜体 
                            SHIFTJIS_CHARSET,   // キャラクタ・セット
                            OUT_DEFAULT_PRECIS, // 出力精度
                            DEFAULT_QUALITY,    // 品質
                            DEFAULT_PITCH || FF_DONTCARE,   // ピッチ
                            _T("ＭＳ ゴシック"),            // フォント名
                            &m_pD3DFont)))
    {
        return FALSE;
    }

    return TRUE;
}

Planet* Scene::GetCurrentPlanet()
{
    if (m_Galaxy.GetPlanet()->GetSize() <= m_currentPlanetIndex)
    {
        m_currentPlanetIndex = -1;
    }

    if (m_currentPlanetIndex == -1)
    {
        return m_Galaxy.GetPlanet();
    }
    return m_Galaxy.GetPlanet()->GetChild(m_currentPlanetIndex);
}

/**
 * シーンの破棄
 */
void Scene::Release()
{
    m_Galaxy.Release();
    m_Stardust.Release();

    // フォントの開放
    SAFE_RELEASE(m_pD3DFont);
}

void Scene::Update(float timeSpan)
{
    // 注視点切り替え
    if (::GetKeyState(VK_F2) < 0)
    {
        // 初回のF2押下の場合
        if (!m_isF2)
        {
            // 移動開始
            if (m_pCurrentCamera == &m_HomingCamera)
            {
                m_currentPlanetIndex++;
                m_HomingCamera.SetTarget(GetCurrentPlanet());
            }
        }
        m_isF2 = TRUE;
    }
    else
    {
        m_isF2 = FALSE;
    }

    // 注視点切り替え
    if (::GetKeyState(VK_F3) < 0)
    {
        // 初回のF3押下の場合
        if (!m_isF3)
        {
            if (m_pCurrentCamera == &m_NormCamera)
            {
                m_pCurrentCamera = &m_HomingCamera;
            }
            else
            {
                m_pCurrentCamera = &m_NormCamera;
            }
        }
        m_isF3 = TRUE;
    }
    else
    {
        m_isF3 = FALSE;
    }

    // Fillモード使用可否切り替え
    if (::GetKeyState(VK_F5) < 0)
    {
        // 初回のF5押下の場合
        if (!m_isF5)
        {
            m_isSolid = !m_isSolid;
        }
        m_isF5 = TRUE;
    }
    else
    {
        m_isF5 = FALSE;
    }

    // Reset
    if (::GetKeyState(VK_F7) < 0)
    {
        // 初回のF7押下の場合
        if (!m_isF7)
        {
            m_Galaxy.Reset();
        }
        m_isF7 = TRUE;
    }
    else
    {
        m_isF7 = FALSE;
    }

    // 更新
    m_Galaxy.Update(timeSpan);
//  m_Light.Update(timeSpan);

    m_pCurrentCamera->Update(timeSpan);

    // 星背景の更新
    D3DXVECTOR3 vPos;
    m_pCurrentCamera->GetWorldPos(&vPos);
    m_Stardust.Update(timeSpan, &vPos);
}

/** 
 * シーンを描画
 * @param pD3DDevice DirectX3Dデバイス
 */
void Scene::Draw(LPDIRECT3DDEVICE9 pD3DDevice, D3DCAPS9* pD3DCaps)
{
    // カメラ設定
    m_pCurrentCamera->Transform(pD3DDevice);

    // Fillモード
    pD3DDevice->SetRenderState(D3DRS_FILLMODE, (m_isSolid) ? D3DFILL_SOLID : D3DFILL_WIREFRAME);
    pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    // 背景
    m_Stardust.Draw(pD3DDevice);
    pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

    // 座標軸
    m_CoordAxis.Draw(pD3DDevice, pD3DCaps);

    // フィールド
    m_Galaxy.Draw(pD3DDevice, pD3DCaps);

    // ステータス表示
    DrawStatus(pD3DDevice);

    // 元に戻す
    pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    //pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

/** 
 * シーンを描画
 * @param pD3DDevice DirectX3Dデバイス
 */
void Scene::DrawStatus(LPDIRECT3DDEVICE9 pD3DDevice)
{
    // 固定FPSの場合
    if (GameApp::GetInstance()->GetFPS()->IsFixedFPS())
    {
        std::wstring status;

        TCHAR szFPS[100];
        SPRINTF(szFPS, 100,
                _T("----- ステータス -----\n")
                _T("FPS:%f\n")
                _T("注視モード ： %s\n"),
                GameApp::GetInstance()->GetFPS()->GetFPS(),
                ((m_pCurrentCamera == &m_NormCamera) ? _T("標準カメラ"):_T("惑星注視カメラ")));

        status += szFPS;
        status += m_Galaxy.GetStatus();
        status += m_pCurrentCamera->GetStatus();

        RECT rect = {10, 10, 0, 0}; // 描画領域
        m_pD3DFont->DrawText(
                    NULL,                   // 描画に使うスプライト
                    status.c_str(),         // 文字列
                    -1,                     // 文字数(NULL終端まで表示)
                    &rect,                  // 描画領域
                    DT_LEFT | DT_NOCLIP,    // 左揃え＆クリップしない
                    D3DCOLOR_XRGB(0, 255, 255));    // 文字色

        // キーアサイン表示
        rect.top += 250;
        m_pD3DFont->DrawText(
                    NULL,                   // 描画に使うスプライト
                    _T("----- キーアサイン -----\n")
                    _T("カメラX軸移動 ： 1 or 3\n")
                    _T("カメラY軸移動 ： 5 or 2\n")
                    _T("カメラZ軸移動 ： 9 or 6\n")
                    _T("（回転:Ctrl併用）\n")
                    _T("カメラズーム ： + or -\n")
                    _T("\n")
                    _T("公転半径割合変更 ： Q or W\n")
                    _T("\n")
                    _T("ESC： 終了\n")
                    _T("F1 ： 固定FPS切替\n")
                    _T("F2 ： 惑星間移動\n")
                    _T("F3 ： カメラ切替\n")
                    _T("F4 ： ライトON/OFF\n")
                    _T("F5 ： FillMode:Solid/Wireframe\n")
                    _T("F6 ： 惑星サイズ固定切り替え\n")
                    _T("F7 ： 公転リセット\n"),
                    // 文字列
                    -1,                         // 文字数(NULL終端まで表示)
                    &rect,                      // 描画領域
                    DT_LEFT | DT_NOCLIP,        // 左揃え＆クリップしない
                    D3DCOLOR_XRGB(0, 255, 255));// 文字色
    }
    // 非固定FPSの場合
    else
    {
        // FPS表示
        TCHAR szFPS[100];
        SPRINTF(szFPS, 100, _T("FPS:%f\n"), GameApp::GetInstance()->GetFPS()->GetFPS());

        RECT rect = {10, 10, 0, 0}; // 描画領域
        m_pD3DFont->DrawText(
                    NULL,                       // 描画に使うスプライト
                    szFPS,                      // 文字列
                    -1,                         // 文字数
                    &rect,                      // 描画領域
                    DT_LEFT | DT_NOCLIP,        // 左揃え＆クリップしない
                    D3DCOLOR_XRGB(0, 255, 255));// 文字色
    }
}
