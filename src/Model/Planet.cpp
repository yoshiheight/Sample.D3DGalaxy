/**
 * @file
 * 惑星の定義
 */
#include "StdAfx.h"
#include "Planet.h"
#include "Galaxy.h"

/**
 * コンストラクタ
 */
Planet::Planet(Galaxy* pGalaxy, const PLANET_INFO* pPlanetInfo)
    :   m_pGalaxy(pGalaxy),
        m_pD3DSphere(NULL),
        m_pD3DTorus(NULL),
        m_pTexture(NULL),
        m_pRingTexture(NULL),
        m_revolutionDegree(0.0f),
        m_pVertexs(NULL),
        m_vertexCount(0)
{
    m_PlanetInfo = *pPlanetInfo;

    // マテリアル初期化
    ::ZeroMemory(&m_D3DMaterial, sizeof(m_D3DMaterial));

    m_D3DMaterial.Diffuse.r = m_PlanetInfo.color.r;
    m_D3DMaterial.Diffuse.g = m_PlanetInfo.color.g;
    m_D3DMaterial.Diffuse.b = m_PlanetInfo.color.b;
    m_D3DMaterial.Diffuse.a = m_PlanetInfo.color.a;

//  m_D3DMaterial.Ambient = m_D3DMaterial.Diffuse;

    m_D3DMaterial.Specular.r = 1.0f;
    m_D3DMaterial.Specular.g = 1.0f;
    m_D3DMaterial.Specular.b = 1.0f;
    m_D3DMaterial.Specular.a = 1.0f;

    m_D3DMaterial.Emissive.r = 0.5f;
    m_D3DMaterial.Emissive.g = 0.5f;
    m_D3DMaterial.Emissive.b = 0.5f;
    m_D3DMaterial.Emissive.a = 1.0f;

//  m_D3DMaterial.Power = 100.0f;
}

/**
 * デストラクタ
 */
Planet::~Planet()
{
    Release();
}

/**
 * 惑星を生成
 * @param pD3DDevice DirectX3Dデバイス
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL Planet::Create(LPDIRECT3DDEVICE9 pD3DDevice)
{
    Release();

    if (FAILED(::D3DXLoadMeshFromX(_T("resource/sphere.x"),
                                D3DXMESH_SYSTEMMEM,
                                pD3DDevice,
                                NULL,
                                NULL,
                                NULL,
                                &m_countOfMesh,
                                &m_pD3DSphere)))
    {
        return FALSE;
    }

    // テクスチャの読み込み
    m_pTexture = TextureManager::GetInstance()->CreateTexture(pD3DDevice, m_PlanetInfo.textureFileName.c_str());
    if (m_pTexture != NULL)
    {
        m_D3DMaterial.Diffuse.r = 1.0f;
        m_D3DMaterial.Diffuse.g = 1.0f;
        m_D3DMaterial.Diffuse.b = 1.0f;
        m_D3DMaterial.Diffuse.a = 1.0f;
    }

    if (m_PlanetInfo.hasRing)
    {
        // オブジェクトのVertexBufferを生成
        if (FAILED(::D3DXCreateTorus(pD3DDevice,
                                    0.4f,
                                    2.0f,
                                    12, 24, &m_pD3DTorus, NULL)))
        {
            return FALSE;
        }

        m_pRingTexture = TextureManager::GetInstance()->CreateTexture(pD3DDevice, _T("resource/ring.png"));
    }

    // 公転軌道の描画頂点の生成
    // （適当に頂点数を算出）
    m_vertexCount = static_cast<INT32>(m_PlanetInfo.revolutionRadius * 20.0f);
    if (m_vertexCount < 1000)
    {
        m_vertexCount = 1000;
    }
    else if (10000 < m_vertexCount)
    {
        m_vertexCount = 10000;
    }
    m_pVertexs = new CUSTOMVERTEX_LINE[m_vertexCount];
    for (int i = 0; i < m_vertexCount; i++)
    {
        m_pVertexs[i].color = m_PlanetInfo.color;

        float x = COS(D3DXToRadian(i * (360.0f / (m_vertexCount - 1)))) * GetScaledRevolutionRadius();
        float z = SIN(D3DXToRadian(i * (360.0f / (m_vertexCount - 1)))) * GetScaledRevolutionRadius();
        m_pVertexs[i].coord = D3DXVECTOR3 (x, 0.0f, z);
    }

    return TRUE;
}

// ステータス文字列を取得
std::wstring Planet::GetStatus() const
{
    TCHAR szFPS[1000];
    SPRINTF(szFPS, 1000,
            _T("距離 ： %f\n")
            _T("角度 ： %f\n"),
            m_PlanetInfo.revolutionRadius,
            m_revolutionDegree);
    return szFPS;
}

/**
 * 惑星の破棄
 */
void Planet::Release()
{
    // 頂点バッファで描画用
    SAFE_RELEASE(m_pD3DSphere);     // 頂点バッファの解放
    SAFE_RELEASE(m_pD3DTorus);

    TextureManager::GetInstance()->ReleaseTexture(m_pTexture);
    m_pTexture = NULL;

    TextureManager::GetInstance()->ReleaseTexture(m_pRingTexture);
    m_pRingTexture = NULL;

    SAFE_DELETE(m_pVertexs);

    std::vector<Planet*>::iterator iterator;
    for (iterator = m_ChildPlanets.begin(); iterator != m_ChildPlanets.end(); iterator++)
    {
//      (*iterator)->Release();
        delete (*iterator);
    }
    m_ChildPlanets.clear();
}

void Planet::Reset()
{
    m_revolutionDegree = 0.0f;

    // 子更新
    std::vector<Planet*>::iterator iterator;
    for (iterator = m_ChildPlanets.begin(); iterator != m_ChildPlanets.end(); iterator++)
    {
        (*iterator)->Reset();
    }
}

/**
 * 更新
 */
void Planet::Update(float timeSpan, LPD3DXMATRIX pParentMatrix)
{
    // 公転角度の更新
    if (0.01f < m_PlanetInfo.revolutionCycle)
    {
//      float speed = 0.5f;
        float speed = 50.0f;
        m_revolutionDegree += (1.0f / m_PlanetInfo.revolutionCycle) * timeSpan * speed;
        if (360.0 < m_revolutionDegree)
        {
            m_revolutionDegree -= 360.0f;
        }
    }

    // 惑星のローカル座標系での現在位置
    float x = COS(D3DXToRadian(m_revolutionDegree)) * GetScaledRevolutionRadius();
    float z = SIN(D3DXToRadian(m_revolutionDegree)) * GetScaledRevolutionRadius();
    D3DXVECTOR3 vLocalPos(x, 0.0f, z);

    // 移動行列の生成
    D3DXMATRIX mMove;
    D3DXMatrixIdentity(&mMove);
    ::D3DXMatrixTranslation(&mMove, vLocalPos.x, vLocalPos.y, vLocalPos.z);

    // ワールド行列の生成
    D3DXMatrixIdentity(&m_mWorld);
    m_mWorld *= mMove;
    m_mWorld *= (*pParentMatrix);

    // ワールド座標の保持
    m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    ::D3DXVec3TransformCoord(&m_vPos, &m_vPos, &m_mWorld);

    // 子更新
    std::vector<Planet*>::iterator iterator;
    for (iterator = m_ChildPlanets.begin(); iterator != m_ChildPlanets.end(); iterator++)
    {
        (*iterator)->Update(timeSpan, &m_mWorld);
    }
}

/**
 * スケーリング後の惑星直径を取得
 */
float Planet::GetScaledDiameter()
{
    return (m_pGalaxy->IsFixedSize()) ? 1.0f : m_PlanetInfo.diameter;
}

/**
 * スケーリング後の公転半径を取得
 */
float Planet::GetScaledRevolutionRadius()
{
    float revolutionRadius = m_PlanetInfo.revolutionRadius * m_pGalaxy->GetRevolutionScale();

    if (0 < m_PlanetInfo.nestLevel && revolutionRadius < (GetScaledDiameter() * 3.0f))
    {
        revolutionRadius = GetScaledDiameter() * 3.0f;
    }

    return revolutionRadius;
}

/** 
 * 惑星を描画
 * @param pD3DDevice DirectX3Dデバイス
 */
void Planet::Draw(LPDIRECT3DDEVICE9 pD3DDevice, D3DCAPS9* pD3DCaps, LPD3DXMATRIX pParentMatrix)
{
    // 公転軌道の描画
    if (0 < m_PlanetInfo.nestLevel)
    {
        float scale = GetScaledRevolutionRadius() / m_PlanetInfo.revolutionRadius;

        D3DXMATRIX  mWorld;
        D3DXMatrixIdentity(&mWorld);
        ::D3DXMatrixScaling(&mWorld, scale, 0.0f, scale);
        mWorld *= (*pParentMatrix);

        pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
        pD3DDevice->SetTransform(D3DTS_WORLD, &mWorld);

        // 描画
        pD3DDevice->SetFVF(CUSTOMVERTEX_FVF_LINE);
        pD3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP,
                                     m_vertexCount - 1,
                                     m_pVertexs, sizeof(m_pVertexs[0]));

        pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    }

    // 並行光源
    if (m_PlanetInfo.nestLevel == 0)
    {
        pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

        // ライト
        D3DLIGHT9 d3dLight;
        ::ZeroMemory(&d3dLight, sizeof(d3dLight));

        d3dLight.Type = D3DLIGHT_DIRECTIONAL;

        d3dLight.Diffuse.r = 1.0f;
        d3dLight.Diffuse.g = 1.0f;
        d3dLight.Diffuse.b = 1.0f;

        d3dLight.Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

        pD3DDevice->LightEnable(0, TRUE);
        pD3DDevice->SetLight(0, &d3dLight);
    }

    // マテリアル
    pD3DDevice->SetMaterial(&m_D3DMaterial);

    // 惑星の大きさのスケール行列の生成
    D3DXMATRIX mScale;
    D3DXMatrixIdentity(&mScale);

    float diameter = GetScaledDiameter();
    ::D3DXMatrixScaling(&mScale, diameter, diameter, diameter);

    // 球の描画
    if (m_pD3DSphere != NULL)
    {
        // ワールド行列設定
        D3DXMATRIX mWorld;
        D3DXMatrixIdentity(&mWorld);
        mWorld *= mScale;
        mWorld *= m_mWorld;
        pD3DDevice->SetTransform(D3DTS_WORLD, &mWorld);

        if (m_pTexture != NULL)
        {
            pD3DDevice->SetTexture(0, m_pTexture->pD3DTexture);
        }

        for (DWORD i = 0; i < m_countOfMesh; i++)
        {
            m_pD3DSphere->DrawSubset(i);
        }
    }

    // 輪の描画
    if (m_pD3DTorus != NULL)
    {
        // スケーリング
        D3DXMATRIX mScaleTorus;
        D3DXMatrixIdentity(&mScaleTorus);
        ::D3DXMatrixScaling(&mScaleTorus, 1.0f, 1.0f, 0.01f);

        // 回転
        D3DXMATRIX mRotateX;
        D3DXMatrixIdentity(&mRotateX);
        ::D3DXMatrixRotationX(&mRotateX, D3DXToRadian(90.0f));

        // ワールド行列設定
        D3DXMATRIX mWorld;
        D3DXMatrixIdentity(&mWorld);
        mWorld *= mScale;
        mWorld *= mScaleTorus;
        mWorld *= mRotateX;
        mWorld *= m_mWorld;
        pD3DDevice->SetTransform(D3DTS_WORLD, &mWorld);

        pD3DDevice->SetTexture(0, m_pRingTexture->pD3DTexture);
        m_pD3DTorus->DrawSubset(0);
    }

    // 元に戻す
    pD3DDevice->SetTexture(0, NULL);

    // 点光源
    if (m_PlanetInfo.nestLevel == 0)
    {
        // 光源の設定
        pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

        // ライト
        D3DLIGHT9 d3dLight;
        ::ZeroMemory(&d3dLight, sizeof(d3dLight));

        d3dLight.Type = D3DLIGHT_POINT;
        
        d3dLight.Diffuse.r = 1.0f;
        d3dLight.Diffuse.g = 1.0f;
        d3dLight.Diffuse.b = 1.0f;

        d3dLight.Range = 10000.0f;
        d3dLight.Position = m_vPos;

        pD3DDevice->LightEnable(0, TRUE);
        pD3DDevice->SetLight(0, &d3dLight);
    }

    // 子
    std::vector<Planet*>::iterator iterator;
    for (iterator = m_ChildPlanets.begin(); iterator != m_ChildPlanets.end(); iterator++)
    {
        (*iterator)->Draw(pD3DDevice, pD3DCaps, &m_mWorld);
    }
}
