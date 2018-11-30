/**
 * @file
 * 惑星の宣言
 */
#ifndef __PLANET_H__
#define __PLANET_H__

#include <vector>
#include "util/TextureManager.h"
#include "camera/HomingCameraTarget.h"

class Galaxy;

// 惑星データテーブル用の構造体
struct PLANET_INFO
{
    INT32           nestLevel;          ///< 階層レベル
    float           diameter;           ///< 直径
    float           revolutionRadius;   ///< 公転半径
    float           revolutionCycle;    ///< 公転周期
    float           angleX;             ///< アングルX
    float           angleY;             ///< アングルY
    D3DXCOLOR       color;              ///< 色
    BOOL            hasRing;            ///< リング有無
    std::wstring    textureFileName;
};

/**
 * 惑星を管理するクラス
 */
class Planet : public IHomingCameraTarget
{
    NO_COPY_CONSTRUCT(Planet);
    NO_OPERATOR_EQUAL(Planet);

public:
    // コンストラクタ
    Planet(Galaxy* pGalaxy, const PLANET_INFO* pPlanetInfo);

    // デストラクタ
    virtual ~Planet();

public:
    // 生成
    BOOL Create(LPDIRECT3DDEVICE9 pD3DDevice);

    // 更新
    void Update(float timeSpan, LPD3DXMATRIX pParentMatrix);

    // 破棄
    void Release();

    // 描画
    void Draw(LPDIRECT3DDEVICE9 pD3DDevice, D3DCAPS9* pD3DCaps, LPD3DXMATRIX pParentMatrix);

    // ステータス文字列を取得
    std::wstring GetStatus() const;

    void Add(Planet* pPlanet) { m_ChildPlanets.push_back(pPlanet); }

    void Reset();

public:
    // 子の惑星数を取得
    inline INT32 GetSize() const { return (INT32) m_ChildPlanets.size(); }

    // 子の惑星を取得
    inline Planet* GetChild(INT32 index) { return m_ChildPlanets[index]; }

    // スケーリング後の惑星直径を取得
    float GetScaledDiameter();

    // スケーリング後の公転半径を取得
    float GetScaledRevolutionRadius();

    // 対象中心のワールド座標を取得
    inline virtual void GetWorldPos(D3DXVECTOR3* pvPos) { (*pvPos) = m_vPos; }

    // 対象とカメラの距離を取得
    virtual float GetDistance() { return GetScaledDiameter() * 5.0f; }

    // 対象のカメラアングルのＸ角度を取得
    virtual float GetAngleX() { return m_PlanetInfo.angleX; }

    /// 対象のカメラアングルのＹ角度を取得
    virtual float GetAngleY() { return m_PlanetInfo.angleY; }

private:
    // 公転軌道の頂点バッファ構造体
    struct CUSTOMVERTEX_LINE
    {
        D3DXVECTOR3 coord;  ///< 座標
        DWORD       color;  ///< 色
    };

    static const DWORD  CUSTOMVERTEX_FVF_LINE = (D3DFVF_XYZ | D3DFVF_DIFFUSE);
    CUSTOMVERTEX_LINE*  m_pVertexs;         ///< 公転軌道の頂点
    INT32               m_vertexCount;

    Galaxy*             m_pGalaxy;

    float                   m_revolutionDegree;         ///< 現在の公転角度
    D3DXMATRIX              m_mWorld;                   ///< ワールド行列
    D3DXVECTOR3             m_vPos;                     ///< ワールド座標
    std::vector<Planet*>    m_ChildPlanets;

    LPD3DXMESH                      m_pD3DSphere;       ///< 球メッシュ
    DWORD                           m_countOfMesh;
    LPD3DXMESH                      m_pD3DTorus;        ///< 輪メッシュ
    D3DMATERIAL9                    m_D3DMaterial;      ///< マテリアル
    PLANET_INFO                     m_PlanetInfo;
    TextureManager::TEXTURE_DATA*   m_pTexture;
    TextureManager::TEXTURE_DATA*   m_pRingTexture;
};

#endif // __PLANET_H__
