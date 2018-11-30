/**
 * @file
 * 星屑管理の定義
 */
#include "StdAfx.h"
#include "Stardust.h"

/**
 * コンストラクタ
 */
Stardust::Stardust()
    : m_pTexture(NULL)
{
    // ポイントスプライト
    for (int i = 0; i < STAR_MAX; i++)
    {
        float x = (::rand() % 600) / 10.0f - 30.0f;
        float y = (::rand() % 600) / 10.0f - 30.0f;
        float z = (::rand() % 600) / 10.0f - 30.0f;

        float r = ((::rand() % 4) + 7) / 10.0f;
        float g = ((::rand() % 4) + 7) / 10.0f;
        float b = ((::rand() % 4) + 7) / 10.0f;

        // 頂点
        m_Vertexs[i].coord = D3DXVECTOR3(x, y, z);
        m_Vertexs[i].color = D3DXCOLOR(r, g, b, 1.0f);
    }
}

/**
 * デストラクタ
 */
Stardust::~Stardust()
{
    Release();
}

/** 
 * 星屑を生成
 * @param pD3DDevice DirectX3Dデバイス
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL Stardust::Create(LPDIRECT3DDEVICE9 pD3DDevice)
{
    Release();

    // テクスチャの読み込み
    m_pTexture = TextureManager::GetInstance()->CreateTexture(pD3DDevice, _T("resource/stardust.png"));

    return TRUE;
}

/**
 * 星屑の破棄
 */
void Stardust::Release()
{
    TextureManager::GetInstance()->ReleaseTexture(m_pTexture);
    m_pTexture = NULL;
}

/**
 * 更新
 */
void Stardust::Update(float timeSpan, const D3DXVECTOR3* pvViewPos)
{
    m_moveX = pvViewPos->x;
    m_moveY = pvViewPos->y;
    m_moveZ = pvViewPos->z;
}

/** 
 * 星屑の描画
 * @param pD3DDevice DirectX3Dデバイス
 */
void Stardust::Draw(LPDIRECT3DDEVICE9 pD3DDevice)
{
    pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    // テクスチャステージの設定
    /*
    // カラー成分の設定（テクスチャARG1の色情報をそのまま出力）
    pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
    // アルファ成分の設定（テクスチャARG1のアルファ値をそのまま出力）
    //pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
    pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
    pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
    */
    pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

    // アルファテスト
    /*
    pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x20);
    pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    */

    // ブレンディング設定
    pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

    // ポイントスプライト設定
    pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);          // ポイントスプライトを有効
    pD3DDevice->SetRenderState(D3DRS_POINTSIZE, ToDWordBit(0.1f));      // サイズ
    pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);           // スケーリングを有効
    pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MAX, ToDWordBit(24.0f)); // 最大サイズ
    pD3DDevice->SetRenderState(D3DRS_POINTSCALE_A, ToDWordBit(0.0f));   // スケール係数
    pD3DDevice->SetRenderState(D3DRS_POINTSCALE_B, ToDWordBit(0.0f));
    pD3DDevice->SetRenderState(D3DRS_POINTSCALE_C, ToDWordBit(1.0f));

    // 移動行列
    D3DXMATRIX mMove;
    D3DXMatrixIdentity(&mMove); // 単位行列
    ::D3DXMatrixTranslation(&mMove, m_moveX, m_moveY, m_moveZ);
    pD3DDevice->SetTransform(D3DTS_WORLD, &mMove);

    // テクスチャー設定
    pD3DDevice->SetTexture(0, m_pTexture->pD3DTexture);

    // ポイントスプライトの描画
    pD3DDevice->SetFVF(POINTSPRITE_FVF);
    pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST,
                                STAR_MAX,
                                m_Vertexs, sizeof(m_Vertexs[0]));

    // 元に戻す
    pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
    pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    pD3DDevice->SetTexture(0, NULL);
}
