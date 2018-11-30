/**
 * @file
 * 太陽系モデルの宣言
 */
#ifndef __GALAXY_H__
#define __GALAXY_H__

#include "Planet.h"

/**
 * 太陽系を管理するクラス
 */
class Galaxy
{
    NO_COPY_CONSTRUCT(Galaxy);
    NO_OPERATOR_EQUAL(Galaxy);

public:
    // コンストラクタ
    Galaxy();

    // デストラクタ
    virtual ~Galaxy();

public:
    // 生成
    BOOL Create(LPDIRECT3DDEVICE9 pD3DDevice, const PLANET_INFO* pPlanetInfoTable);

    // 更新
    void Update(float timeSpan);

    // 破棄
    void Release();

    // 描画
    void Draw(LPDIRECT3DDEVICE9 pD3DDevice, D3DCAPS9* pD3DCaps);

    void Reset() { m_pPlanet->Reset(); }

public:
    // ステータス文字列を取得
    std::wstring GetStatus() const;

    // 惑星を取得
    Planet* GetPlanet() { return m_pPlanet; }

    float GetRevolutionScale() const { return m_revolutionScale; }

    BOOL IsFixedSize() const { return m_isFixedSize; }

private:
    Planet* m_pPlanet;

    D3DXMATRIX  m_mWorld;

    // 移動量
    float       m_moveX;    ///< X軸移動量
    float       m_moveY;    ///< Y軸移動量
    float       m_moveZ;    ///< Z軸移動量

    static const float  MOVE_SPEED;     ///< キー操作での移動速度

    float       m_revolutionScale;

    // 惑星サイズ固定切り替え用
    BOOL    m_isF6;         ///< F6キー初回押下フラグ
    BOOL    m_isFixedSize;

};

#endif // __GALAXY_H__
