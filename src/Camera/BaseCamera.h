/**
 * @file
 * カメラ管理の宣言
 */
#ifndef __BASECAMERA_H__
#define __BASECAMERA_H__

/**
 * カメラ管理クラス
 */
class BaseCamera
{
    NO_COPY_CONSTRUCT(BaseCamera);
    NO_OPERATOR_EQUAL(BaseCamera);

public:
    // コンストラクタ
    BaseCamera();

    // デストラクタ
    virtual ~BaseCamera();

    // 更新
    virtual void Update(float timeSpan) = 0;

    // 変換行列設定
    virtual void Transform(LPDIRECT3DDEVICE9 pD3DDevice) = 0;

    virtual void GetWorldPos(D3DXVECTOR3* pvPos) = 0;

    // ステータス文字列を取得
    virtual std::wstring GetStatus() = 0;

protected:
    float   m_aspect;   ///< アスペクト比
    float   m_zoom;     ///< ズーム値
};

#endif // __BASECAMERA_H__
