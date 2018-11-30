/**
 * @file
 * 標準カメラの宣言
 */
#ifndef __NORMALCAMERA_H__
#define __NORMALCAMERA_H__

#include "BaseCamera.h"

/**
 * 標準カメラを管理するクラス
 */
class NormalCamera : public BaseCamera
{
    NO_COPY_CONSTRUCT(NormalCamera);
    NO_OPERATOR_EQUAL(NormalCamera);

public:
    // コンストラクタ
    NormalCamera();

    // デストラクタ
    virtual ~NormalCamera();

    // カメラを生成
    BOOL Init(LPDIRECT3DDEVICE9 pD3DDevice);

    // 更新
    void Update(float timeSpan);

    // 変換行列設定
    void Transform(LPDIRECT3DDEVICE9 pD3DDevice);

    void GetMatrix(D3DXMATRIX* pMatrix);

    void GetWorldPos(D3DXVECTOR3* pvPos)
    {
        pvPos->x = m_moveX;
        pvPos->y = m_moveY;
        pvPos->z = m_moveZ;
    }

    // ステータス文字列を取得
    std::wstring GetStatus();

private:
    // フリー操作用
    float       m_moveX;    ///< ワールド座標系のX軸移動量
    float       m_moveY;    ///< ワールド座標系のY軸移動量
    float       m_moveZ;    ///< ワールド座標系のZ軸移動量
    float       m_rotateX;  ///< ワールド座標系のX軸回転角度
    float       m_rotateY;  ///< ワールド座標系のY軸回転角度

    // キー操作でのスピード定数
    static const float  MOVE_SPEED;     ///< キー操作での移動速度
    static const float  ROTATE_SPEED;   ///< キー操作での回転速度
    static const float  ZOOM_SPEED;     ///< キー操作でのズーム速度
};

#endif // __NORMALCAMERA_H__
