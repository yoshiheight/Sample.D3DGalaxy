/**
 * @file
 * シーン管理の宣言
 */
#ifndef __SCENE_H__
#define __SCENE_H__

#include "util/TextureManager.h"
#include "camera/BaseCamera.h"
#include "camera/HomingCamera.h"
#include "camera/NormalCamera.h"
#include "scene/CoordAxis.h"
#include "model/Galaxy.h"
#include "model/Planet.h"
#include "model/Stardust.h"

/**
 * シーンを管理するクラス
 */
class Scene
{
    NO_COPY_CONSTRUCT(Scene);
    NO_OPERATOR_EQUAL(Scene);

public:
    // コンストラクタ
    Scene();

    // デストラクタ
    virtual ~Scene();

    // シーンを生成
    BOOL Create(LPDIRECT3DDEVICE9 pD3DDevice);

    // キーアサイン
    void Update(float timeSpan);

    // シーンの破棄
    void Release();

    // シーンを描画
    void Draw(LPDIRECT3DDEVICE9 pD3DDevice, D3DCAPS9* pD3DCaps);

    Planet* GetCurrentPlanet();

private:
    void DrawStatus(LPDIRECT3DDEVICE9 pD3DDevice);

private:
    // 操作モードの切替用
    BOOL        m_isF2;             ///< F2押下フラグ
    INT32       m_currentPlanetIndex;

    NormalCamera    m_NormCamera;   ///< カメラ
    HomingCamera    m_HomingCamera; ///< カメラ

    CoordAxis       m_CoordAxis;
    Galaxy          m_Galaxy;

    Stardust        m_Stardust;

    // 文字列描画用
    LPD3DXFONT      m_pD3DFont;     // フォント

    // 操作モードの切替用
    BOOL            m_isF3;         ///< F3押下フラグ

    // Fillモード切り替え用
    BOOL    m_isF5;                 ///< F5キー初回押下フラグ
    BOOL    m_isSolid;              ///< ソリッド描画フラグ

    // Reset用
    BOOL    m_isF7;                 ///< F7キー初回押下フラグ

    BaseCamera* m_pCurrentCamera;
};

#endif // __SCENE_H__
