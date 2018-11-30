/**
 * @file
 * カメラ管理の定義
 */
#include "StdAfx.h"
#include "BaseCamera.h"

/**
 * コンストラクタ
 */
BaseCamera::BaseCamera()
    :   m_aspect(1.0f),
        m_zoom(45.0f)
{
}

/**
 * デストラクタ
 */
BaseCamera::~BaseCamera()
{
}
