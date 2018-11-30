/**
 * @file
 * 自動誘導目標インターフェースの宣言
 */
#ifndef __HOMINGTARGET_H__
#define __HOMINGTARGET_H__

/**
 * 自動誘導目標インターフェース
 */
class IHomingCameraTarget
{
    NO_COPY_CONSTRUCT(IHomingCameraTarget);
    NO_OPERATOR_EQUAL(IHomingCameraTarget);

public:
    // コンストラクタ
    IHomingCameraTarget() {}

    // デストラクタ
    virtual ~IHomingCameraTarget() {}

    // 対象中心のワールド座標を取得
    virtual void GetWorldPos(D3DXVECTOR3* pvPos) = 0;

    // 対象とカメラの距離を取得
    virtual float GetDistance() = 0;

    // 対象のカメラアングルのＸ角度を取得
    virtual float GetAngleX() = 0;

    // 対象のカメラアングルのＹ角度を取得
    virtual float GetAngleY() = 0;
};

#endif // __HOMINGTARGET_H__
