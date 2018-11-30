/**
 * @file
 * 星屑管理の宣言
 */
#ifndef __STARDUST_H__
#define __STARDUST_H__

#include "util/TextureManager.h"

/**
 * 星屑を管理するクラス
 */
class Stardust
{
    NO_COPY_CONSTRUCT(Stardust);
    NO_OPERATOR_EQUAL(Stardust);

public:
    // コンストラクタ
    Stardust();

    // デストラクタ
    virtual ~Stardust();

    // 生成
    BOOL Create(LPDIRECT3DDEVICE9 pD3DDevice);

    // 更新
    void Update(float timeSpan, const D3DXVECTOR3* pvViewPos);

    // 破棄
    void Release();

    // 描画
    void Draw(LPDIRECT3DDEVICE9 pD3DDevice);

private:
    // TODO 使い方が間違ってるが、とりあえずポイントスプライトで星屑を表現しておく

    // 頂点バッファの構造体
    struct POINTSPRITE_VERTEX
    {
        D3DXVECTOR3 coord;  ///< 座標
        DWORD       color;  ///< 色
    };

    static const DWORD POINTSPRITE_FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE);

    static const INT32 STAR_MAX = 5001;

    POINTSPRITE_VERTEX m_Vertexs[STAR_MAX]; ///< 星頂点

    TextureManager::TEXTURE_DATA* m_pTexture;

    // 移動量
    float   m_moveX;    ///< X軸移動量
    float   m_moveY;    ///< Y軸移動量
    float   m_moveZ;    ///< Z軸移動量
};

#endif // __STARDUST_H__
