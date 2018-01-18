#include "stdafx.h"
#include <windows.h>
#include <assert.h>
#include "DpiHelper.h"

namespace DuiLib
{
#define  CheckMainThread()  assert(GetCurrentThreadId() == m_dwMainThreadId)
CDPIHelper* CDPIHelper::m_pInstance = NULL;
CDPIHelper* CDPIHelper::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new CDPIHelper();
    }
    return m_pInstance;
}

void CDPIHelper::DeleteInstance()
{
    if (m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

CDPIHelper::CDPIHelper():m_dwMainThreadId(0)
{
    m_nScaleFactor = 0;
    m_nScaleFactorSDA = 0;
    m_uDPI = 0;
    m_bIsScaleFlag = TRUE;
    m_bDPIEnable = FALSE;
    m_Awareness = PROCESS_DPI_UNAWARE;
    m_dwMainThreadId = GetCurrentThreadId();
}

CDPIHelper::~CDPIHelper()
{
    UnInit();
}

void CDPIHelper::Init(__in UINT uDPI)
{
    m_uDPI = uDPI;
    SetScale(uDPI);
    if (uDPI > 96)
    {
        m_Awareness = PROCESS_SYSTEM_DPI_AWARE;
    }
}

void CDPIHelper::UnInit()
{

}

UINT CDPIHelper::GetScale()
{
    if (m_Awareness == PROCESS_DPI_UNAWARE)
    {
        return 100;
    }

    if (m_Awareness == PROCESS_SYSTEM_DPI_AWARE)
    {
        return m_nScaleFactorSDA;
    }

    return m_nScaleFactor;
}

void CDPIHelper::SetScale(__in UINT uDPI)
{
    m_nScaleFactor = MulDiv(uDPI, 100, 96);
    if (m_nScaleFactorSDA == 0)
    {
        m_nScaleFactorSDA = m_nScaleFactor;  // Save the first scale factor, which is all that SDA apps know about
    }
    return;
}

// scale Rect point int long
// Scale rectangle from raw pixels to relative pixels.
void CDPIHelper::Scale(__inout RECT *pRect, BOOL bIsForceUse)
{
    CheckMainThread();
    if (pRect != NULL)
    {
        if (m_bIsScaleFlag || bIsForceUse)
        {
            pRect->left = Scale(pRect->left);
            pRect->right = Scale(pRect->right);
            pRect->top = Scale(pRect->top);
            pRect->bottom = Scale(pRect->bottom);
        }
    }
}
void CDPIHelper::UnScale(__inout RECT *pRect, BOOL bIsForceUse)
{
    CheckMainThread();
    if (pRect != NULL)
    {
        if (m_bIsScaleFlag || bIsForceUse)
        {
            pRect->left = UnScale(pRect->left);
            pRect->right = UnScale(pRect->right);
            pRect->top = UnScale(pRect->top);
            pRect->bottom = UnScale(pRect->bottom);
        }
    }
}

// Scale Point from raw pixels to relative pixels.
void CDPIHelper::Scale(__inout POINT *pPoint, BOOL bIsForceUse)
{
    CheckMainThread();
    if (pPoint != NULL)
    {
        if (m_bIsScaleFlag || bIsForceUse)
        {
            pPoint->x = Scale(pPoint->x);
            pPoint->y = Scale(pPoint->y);
        }
    }
}
void CDPIHelper::UnScale(__inout POINT *pPoint, BOOL bIsForceUse)
{
    CheckMainThread();
    if (pPoint != NULL)
    {
        if (m_bIsScaleFlag || bIsForceUse)
        {
            pPoint->x = UnScale(pPoint->x);
            pPoint->y = UnScale(pPoint->y);
        }
    }
}

// Scale SIZE from raw pixels to relative pixels.
void CDPIHelper::Scale(__inout SIZE *pSize, BOOL bIsForceUse)
{
    CheckMainThread();
    if (pSize != NULL)
    {
        if (m_bIsScaleFlag || bIsForceUse)
        {
            pSize->cx = Scale(pSize->cx);
            pSize->cy = Scale(pSize->cy);
        }
    }
}
void CDPIHelper::UnScale(__inout SIZE *pSize, BOOL bIsForceUse)
{
    CheckMainThread();
    if (pSize != NULL)
    {
        if (m_bIsScaleFlag || bIsForceUse)
        {
            pSize->cx = UnScale(pSize->cx);
            pSize->cy = UnScale(pSize->cy);
        }
    }
}

// Scale X,Y from raw pixels to relative pixels.
void CDPIHelper::Scale(__inout int *pX, BOOL bIsForceUse)
{
    CheckMainThread();
    if (pX != NULL)
    {
        if (m_bIsScaleFlag || bIsForceUse)
        {
            int nTmp = *pX;
            if (m_Awareness == PROCESS_DPI_UNAWARE)
            {
            }
            else if (m_Awareness == PROCESS_SYSTEM_DPI_AWARE)
            {
                nTmp = IntegerScale(*pX, m_nScaleFactorSDA, 100);
            }
            else
            {
                nTmp = IntegerScale(*pX, m_nScaleFactor, 100);
            }
            *pX = nTmp;
        }
    }
}
void CDPIHelper::UnScale(__inout int *pX, BOOL bIsForceUse)
{
    CheckMainThread();
    if (pX != NULL)
    {
        if (m_bIsScaleFlag || bIsForceUse)
        {
            int nTmp = *pX;
            if (m_Awareness == PROCESS_DPI_UNAWARE)
            {
            }
            else if (m_Awareness == PROCESS_SYSTEM_DPI_AWARE)
            {
                nTmp = UnIntegerScale(*pX, 100, m_nScaleFactorSDA);
            }
            else
            {
                nTmp = UnIntegerScale(*pX, 100, m_nScaleFactor);
            }
            *pX = nTmp;
        }
    }
}

void CDPIHelper::Scale(__inout LONG *pX, BOOL bIsForceUse)
{
    CheckMainThread();
    int nTmp = (int)(*pX);
    Scale(&nTmp, bIsForceUse);
    *pX = (LONG)nTmp;
}
void CDPIHelper::UnScale(__inout LONG *pX, BOOL bIsForceUse)
{
    CheckMainThread();
    int nTmp = (int)(*pX);
    UnScale(&nTmp, bIsForceUse);
    *pX = (LONG)nTmp;
}

int CDPIHelper::Scale(int x)
{
    // DPI Unaware:  Return the input value with no scaling.
    // These apps are always virtualized to 96 DPI and scaled by the system for the DPI of the monitor where shown.
    if (m_Awareness == PROCESS_DPI_UNAWARE)
    {
        return x;
    }
    // System DPI Aware:  Return the input value scaled by the factor determined by the system DPI when the app was launched.
    // These apps render themselves according to the DPI of the display where they are launched, and they expect that scaling
    // to remain constant for all displays on the system.
    // These apps are scaled up or down when moved to a display with a different DPI from the system DPI.
    if (m_Awareness == PROCESS_SYSTEM_DPI_AWARE)
    {
        return IntegerScale(x, m_nScaleFactorSDA, 100);
    }
    // Per-Monitor DPI Aware:  Return the input value scaled by the factor for the display which contains most of the window.
    // These apps render themselves for any DPI, and re-render when the DPI changes (as indicated by the WM_DPICHANGED window message).
    return IntegerScale(x, m_nScaleFactor, 100);
}
int CDPIHelper::UnScale(int x)
{
    if (m_Awareness == PROCESS_DPI_UNAWARE)
    {
        return x;
    }
    if (m_Awareness == PROCESS_SYSTEM_DPI_AWARE)
    {
        return UnIntegerScale(x, 100, m_nScaleFactorSDA);
    }
    return UnIntegerScale(x, 100, m_nScaleFactor);
}

// ScaleMax
void CDPIHelper::ScaleMax(__inout RECT *pRect, unsigned int uMaxDPI/* = 192*/)
{
    CheckMainThread();
    if (pRect != NULL)
    {
        if (m_bIsScaleFlag)
        {
            pRect->left = ScaleMax(pRect->left, uMaxDPI);
            pRect->right = ScaleMax(pRect->right, uMaxDPI);
            pRect->top = ScaleMax(pRect->top, uMaxDPI);
            pRect->bottom = ScaleMax(pRect->bottom, uMaxDPI);
        }
    }
}
void CDPIHelper::UnScaleMax(__inout RECT *pRect, unsigned int uMaxDPI/* = 192*/)
{
    CheckMainThread();
    if (pRect != NULL)
    {
        if (m_bIsScaleFlag)
        {
            pRect->left = UnScaleMax(pRect->left, uMaxDPI);
            pRect->right = UnScaleMax(pRect->right, uMaxDPI);
            pRect->top = UnScaleMax(pRect->top, uMaxDPI);
            pRect->bottom = UnScaleMax(pRect->bottom, uMaxDPI);
        }
    }
}

int CDPIHelper::ScaleMax(int x, unsigned int uMaxDPI)
{
    if (m_Awareness == PROCESS_DPI_UNAWARE)
    {
        return x;
    }
    unsigned int uScaleFactorSDA = GetScaleFactorSDA();
    return IntegerScale(x, uScaleFactorSDA, 100);
}
int CDPIHelper::UnScaleMax(int x, unsigned int uMaxDPI)
{
    if (m_Awareness == PROCESS_DPI_UNAWARE)
    {
        return x;
    }
    unsigned int uScaleFactorSDA = GetScaleFactorSDA();
    return UnIntegerScale(x, 100, uScaleFactorSDA);
}

UINT CDPIHelper::GetScaleFactorSDA()
{
    unsigned int uScaleFactorSDA = 100;
    if (m_uDPI == DEFAULT_DPI || m_uDPI == SCALE125_DPI || m_uDPI == SCALE150_DPI || m_uDPI == SCALE200_DPI)
    {
        uScaleFactorSDA = m_nScaleFactorSDA;
    }
    else
    {
        uScaleFactorSDA = 200;
    }
    return uScaleFactorSDA;
}

std::wstring CDPIHelper::GetDpiFileName(const std::wstring& strFileName)
{
    CheckMainThread();
    std::wstring strDpiFileName = strFileName;
    if (m_nScaleFactorSDA == 0 || m_nScaleFactorSDA == 100 || !m_bDPIEnable)
    {
        return strDpiFileName;
    }
    std::wstring strName;
    std::wstring strSuffix;
    std::size_t nIndex = strFileName.rfind(_T("."));
    if (nIndex != std::wstring::npos)
    {
        strName = strFileName.substr(0, nIndex);
        strSuffix = strFileName.substr(nIndex);//包括.
    }
    else
    {
        return strDpiFileName;
    }

    BOOL Is96Dpi = FALSE;
    //strDpiFileName = _T("DPI200\\") + strDpiFileName;//在DPI200文件夹中
    if (m_nScaleFactorSDA >= 200)
    {
        strDpiFileName = strName + _T("_@200") + strSuffix;
    }
    else if (m_nScaleFactorSDA == 150)
    {
        strDpiFileName = strName + _T("_@150") + strSuffix;
    }
    else if (m_nScaleFactorSDA == 125)
    {
        strDpiFileName = strName + _T("_@125") + strSuffix;
    }
    else
    {
        //other DPI
        strDpiFileName = strFileName;
        Is96Dpi = TRUE;
    }
    if (!Is96Dpi)
    {
        //not exist
        strDpiFileName = strFileName;
    }
    return strDpiFileName;
}

// private
int CDPIHelper::IntegerScale(int nSrcX, int nDestScaleFactor, int nSrcScaleFactor)
{
    if (nSrcX * nDestScaleFactor % nSrcScaleFactor == 0)
    {
        return MulDiv(nSrcX, nDestScaleFactor, nSrcScaleFactor);
    }
    else
    {
        return (nSrcX * nDestScaleFactor / nSrcScaleFactor + 1);
    }
}

int CDPIHelper::UnIntegerScale(int nSrcX, int nDestScaleFactor, int nSrcScaleFactor)
{
    return (nSrcX * nDestScaleFactor / nSrcScaleFactor);
}

}
