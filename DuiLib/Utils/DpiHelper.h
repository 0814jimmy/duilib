
/*
注意线程安全性，建议在主线程调用；多线程调用需特别注意
*/
#pragma once
#include <string>
#include <tchar.h>
namespace DuiLib
{
    #define DEFAULT_DPI    96
    #define SCALE125_DPI  120
    #define SCALE150_DPI  144
    #define SCALE200_DPI  192
    #define SCALE250_DPI  240
    #define SCALE300_DPI  288
    #define SCALE350_DPI  336
    #define SCALE400_DPI  384
    typedef enum _PROCESS_DPI_AWARENESS { 
        PROCESS_DPI_UNAWARE            = 0,
        PROCESS_SYSTEM_DPI_AWARE       = 1,
        PROCESS_PER_MONITOR_DPI_AWARE  = 2
    } PROCESS_DPI_AWARENESS;

    class DUILIB_API CDPIHelper
    {
    private:
        CDPIHelper();
        static CDPIHelper* m_pInstance;
        ~CDPIHelper();
    public:
        static CDPIHelper* getInstance();
        static void DeleteInstance();
        PROCESS_DPI_AWARENESS GetAwareness() { return m_Awareness; }
        void Init(__in UINT uDPI);
        void UnInit();

        UINT GetScale();
        void SetScale(__in UINT uDPI);
        /*
        scale Rect point int long
        涉及小数的，scale进位,unscale取整
        */
        // Scale rectangle from raw pixels to relative pixels.
        void Scale(__inout RECT *pRect, BOOL bIsForceUse = FALSE);
        void UnScale(__inout RECT *pRect, BOOL bIsForceUse = FALSE);

        // Scale Point from raw pixels to relative pixels.
        void Scale(__inout POINT *pPoint, BOOL bIsForceUse = FALSE);
        void UnScale(__inout POINT *pPoint, BOOL bIsForceUse = FALSE);

        // Scale SIZE from raw pixels to relative pixels.
        void Scale(__inout SIZE *pSize, BOOL bIsForceUse = FALSE);
        void UnScale(__inout SIZE *pSize, BOOL bIsForceUse = FALSE);

        // Scale LONG X,Y from raw pixels to relative pixels.
        void Scale(__inout LONG *pX, BOOL bIsForceUse = FALSE);
        void UnScale(__inout LONG *pX, BOOL bIsForceUse = FALSE);

        // Scale int X,Y from raw pixels to relative pixels.
        void Scale(__inout int *pX, BOOL bIsForceUse = FALSE);
        void UnScale(__inout int *pX, BOOL bIsForceUse = FALSE);

        void SetIsScaleFlag(BOOL bIsScale){ m_bIsScaleFlag = bIsScale; }
        BOOL GetScaleFlag(){ return m_bIsScaleFlag; }

        int Scale(int x);
        int UnScale(int x);

        void ScaleMax(__inout RECT *pRect, unsigned int uMaxDPI = 192);
        void UnScaleMax(__inout RECT *pRect, unsigned int uMaxDPI = 192);
        int ScaleMax(int x, unsigned int uMaxDPI = 192);
        int UnScaleMax(int x, unsigned int uMaxDPI = 192);
        UINT GetScaleFactorSDA();
        // 动态回去图片
        // 传入图片名，如果有dpi200的即返回***_@200.png，否则返回原有图片名
        // std::wstring GetDpiFileName(const TCHAR* pFileName);
        std::wstring GetDpiFileName(const std::wstring& strFileName);
    private:
        //下面函数要二合一，后边修改
        int IntegerScale(int nSrcX, int nDestScaleFactor, int nSrcScaleFactor);
        int UnIntegerScale(int nSrcX, int nDestScaleFactor, int nSrcScaleFactor);
    private:
        int m_dwMainThreadId;
        BOOL m_bDPIEnable;      // 是否处理DPI相关的东西,False全部失效
        UINT m_nScaleFactor;    // PROCESS_PER_MONITOR_DPI_AWARE使用
        UINT m_nScaleFactorSDA; // PROCESS_SYSTEM_DPI_AWARE使用，目前使用该状态
        BOOL m_bIsScaleFlag;
        UINT m_uDPI;
        PROCESS_DPI_AWARENESS m_Awareness;
    };


    class DUILIB_API CDPISwitch
    {
    public:
        CDPISwitch(BOOL bIsDpiAware):bOldDpiAware(TRUE)
        {
            bOldDpiAware = CDPIHelper::getInstance()->GetScaleFlag();
            CDPIHelper::getInstance()->SetIsScaleFlag(bIsDpiAware);
        }
        ~CDPISwitch()
        {
            CDPIHelper::getInstance()->SetIsScaleFlag(bOldDpiAware);
        }
    private:
        BOOL bOldDpiAware;
    };

    // 受DPI_AWARE和DPI_UNAWARE影响；线程不安全
    #define DPI_SCALE(p96Src)            \
    do{\
        CDPIHelper::getInstance()->Scale(p96Src);\
    } while (0);\

    #define DPI_UNSCALE(pPhysicalDest)            \
    do{\
        CDPIHelper::getInstance()->UnScale(pPhysicalDest);\
    } while (0);\

    // 返回值的，不受DPI_AWARE和DPI_UNAWARE影响；线程安全
    #define DPI_SCALE_FORCE(n96Src)                 CDPIHelper::getInstance()->Scale(n96Src)
    #define DPI_UNSCALE_FORCE(nPhysicalDest)        CDPIHelper::getInstance()->UnScale(nPhysicalDest)
    #define DPI_SCALE_FORCE_MAX(n96Src)             CDPIHelper::getInstance()->ScaleMax(n96Src)

    // 图片资源缩放，只支持到200，超过的以200资源缩放
    #define DPI_SCALE_MAX(p96Src)            \
    do{\
        CDPIHelper::getInstance()->ScaleMax(p96Src);\
    } while (0);\

    #define DPI_UNSCALE_MAX(pPhysicalDest)            \
    do{\
        CDPIHelper::getInstance()->UnScaleMax(pPhysicalDest);\
    } while (0);\

    // 默认是打开缩放的，一般不需要主动调用
    #define DPI_AWARE()     CDPISwitch DPISwitch(TRUE);
    #define DPI_UNAWARE()   CDPISwitch DPISwitch(FALSE);

    #define DPI_GETDPIIMAGE(strImageName)   CDPIHelper::getInstance()->GetDpiFileName(strImageName)
}
