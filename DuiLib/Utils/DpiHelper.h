
/*
ע���̰߳�ȫ�ԣ����������̵߳��ã����̵߳������ر�ע��
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
        �漰С���ģ�scale��λ,unscaleȡ��
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
        // ��̬��ȥͼƬ
        // ����ͼƬ���������dpi200�ļ�����***_@200.png�����򷵻�ԭ��ͼƬ��
        // std::wstring GetDpiFileName(const TCHAR* pFileName);
        std::wstring GetDpiFileName(const std::wstring& strFileName);
    private:
        //���溯��Ҫ����һ������޸�
        int IntegerScale(int nSrcX, int nDestScaleFactor, int nSrcScaleFactor);
        int UnIntegerScale(int nSrcX, int nDestScaleFactor, int nSrcScaleFactor);
    private:
        int m_dwMainThreadId;
        BOOL m_bDPIEnable;      // �Ƿ���DPI��صĶ���,Falseȫ��ʧЧ
        UINT m_nScaleFactor;    // PROCESS_PER_MONITOR_DPI_AWAREʹ��
        UINT m_nScaleFactorSDA; // PROCESS_SYSTEM_DPI_AWAREʹ�ã�Ŀǰʹ�ø�״̬
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

    // ��DPI_AWARE��DPI_UNAWAREӰ�죻�̲߳���ȫ
    #define DPI_SCALE(p96Src)            \
    do{\
        CDPIHelper::getInstance()->Scale(p96Src);\
    } while (0);\

    #define DPI_UNSCALE(pPhysicalDest)            \
    do{\
        CDPIHelper::getInstance()->UnScale(pPhysicalDest);\
    } while (0);\

    // ����ֵ�ģ�����DPI_AWARE��DPI_UNAWAREӰ�죻�̰߳�ȫ
    #define DPI_SCALE_FORCE(n96Src)                 CDPIHelper::getInstance()->Scale(n96Src)
    #define DPI_UNSCALE_FORCE(nPhysicalDest)        CDPIHelper::getInstance()->UnScale(nPhysicalDest)
    #define DPI_SCALE_FORCE_MAX(n96Src)             CDPIHelper::getInstance()->ScaleMax(n96Src)

    // ͼƬ��Դ���ţ�ֻ֧�ֵ�200����������200��Դ����
    #define DPI_SCALE_MAX(p96Src)            \
    do{\
        CDPIHelper::getInstance()->ScaleMax(p96Src);\
    } while (0);\

    #define DPI_UNSCALE_MAX(pPhysicalDest)            \
    do{\
        CDPIHelper::getInstance()->UnScaleMax(pPhysicalDest);\
    } while (0);\

    // Ĭ���Ǵ����ŵģ�һ�㲻��Ҫ��������
    #define DPI_AWARE()     CDPISwitch DPISwitch(TRUE);
    #define DPI_UNAWARE()   CDPISwitch DPISwitch(FALSE);

    #define DPI_GETDPIIMAGE(strImageName)   CDPIHelper::getInstance()->GetDpiFileName(strImageName)
}
