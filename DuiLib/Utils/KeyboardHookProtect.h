#pragma once
#include <WinDef.h>
#include <atlbase.h>
namespace DuiLib
{
    /*
    ˫������ҹ�
    */
    class CKeyboardHookProtect
    {
    private:
        CKeyboardHookProtect():m_dwMainThreadId(0),m_hHOOK1(NULL),m_hHOOK2(NULL),m_bExit(FALSE)
        {
        }
        ~CKeyboardHookProtect()
        {
        }
        static CKeyboardHookProtect* m_pInstance;

    public:
        static CKeyboardHookProtect* GetInstance();
        static void DeleteInstance();
        void Init();
        void UnInit();
        BOOL StartHook();// ��װ���Ӻ���
        BOOL StopHook();// ж�ع��Ӻ���
        BOOL DoOnTimeProc();// ��ʱ����ռ����
    public:
        static LRESULT CALLBACK KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam)
        {
            return S_OK;
            //return CallNextHookEx(gHook, nCode, wParam, lParam);
        }
    private:
        DWORD m_dwMainThreadId;
        HHOOK m_hHOOK1;
        HHOOK m_hHOOK2;
        BOOL m_bExit;
    };
}