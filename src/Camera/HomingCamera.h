/**
 * @file
 * 自動誘導カメラの宣言
 */
#ifndef __PLANETCAMERA_H__
#define __PLANETCAMERA_H__

#include "BaseCamera.h"
#include "camera/HomingCameraTarget.h"

/**
 * 自動誘導カメラ
 */
class HomingCamera : public BaseCamera
{
    NO_COPY_CONSTRUCT(HomingCamera);
    NO_OPERATOR_EQUAL(HomingCamera);

public:
    // コンストラクタ
    HomingCamera();

    // デストラクタ
    virtual ~HomingCamera();

    // カメラを生成
    BOOL Init(LPDIRECT3DDEVICE9 pD3DDevice, IHomingCameraTarget* pTarget);

    // 更新
    void Update(float timeSpan);

    // 変換行列設定
    void Transform(LPDIRECT3DDEVICE9 pD3DDevice);

    // ステータス文字列を取得
    std::wstring GetStatus();

    // ワールド座標系でのカメラ位置を取得
    void GetWorldPos(D3DXVECTOR3* pvPos) { (*pvPos) = m_vPos; }

    // 追跡対象の設定
    void SetTarget(IHomingCameraTarget* pTarget);

private:
    // 構造体
    struct POSITION
    {
        D3DXVECTOR3 vOrg;       ///< 注視点
        float       distance;   ///< 対象の境界球半径
        float       angleX;     ///< X軸回転角度
        float       angleY;     ///< Y軸回転角度
    };

    POSITION    m_CurrentPos;       ///< カメラの現在地点

    // 移動用
    BOOL        m_isMoving;         ///< 移動が必要かどうか
    POSITION    m_EndPos;           ///< 移動終了地点
    POSITION    m_StartPos;         ///< 移動開始地点
    float       m_currentTime;      ///< 移動経過秒
    float       m_startTime;        ///< 移動に要する秒

    // Update時の値保持用
    D3DXMATRIX  m_mView;
    D3DXVECTOR3 m_vPos;

    IHomingCameraTarget*    m_pTarget;      ///< 追跡対象

    // キー操作でのスピード定数
    static const float  MOVE_SPEED;     ///< キー操作での移動速度
    static const float  ROTATE_SPEED;   ///< キー操作での回転速度
    static const float  ZOOM_SPEED;     ///< キー操作でのズーム速度
};

#endif // __PLANETCAMERA_H__
