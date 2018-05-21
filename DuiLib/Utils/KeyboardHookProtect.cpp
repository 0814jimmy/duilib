#include "StdAfx.h"
#include "KeyboardHookProtect.h"
namespace DuiLib
{
    //static
    CKeyboardHookProtect* CKeyboardHookProtect::m_pInstance = NULL;
    CComAutoCriticalSection csCreateObj;
    CKeyboardHookProtect* CKeyboardHookProtect::GetInstance()
    {
        CComCritSecLock<CComAutoCriticalSection> lock(csCreateObj);
        if (NULL == m_pInstance)
        {
            m_pInstance = new CKeyboardHookProtect();
            m_pInstance->Init();
        }
        return m_pInstance;
    }

    void CKeyboardHookProtect::DeleteInstance()
    {
        CComCritSecLock<CComAutoCriticalSection> lock(csCreateObj);
        if (m_pInstance)
        {
            m_pInstance->UnInit();
            delete m_pInstance;
            m_pInstance = NULL;
        }
    }

    void CKeyboardHookProtect::Init()
    {
        m_dwMainThreadId = GetCurrentThreadId();
    }

    void CKeyboardHookProtect::UnInit()
    {
        m_bExit = TRUE;
        StopHook();
    }

    BOOL CKeyboardHookProtect::StartHook()
    {
        BOOL bResult=FALSE;
        if (!m_hHOOK1)
        {
            // 线程钩子
            //m_hHOOK1 = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, hModuleInst, 0);WH_KEYBOARD_LL必须是全局钩子
            m_hHOOK1 = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, 0, m_dwMainThreadId);
        }
        if (!m_hHOOK2)
        {
            m_hHOOK2 = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, NULL, m_dwMainThreadId);
        }
        if (m_hHOOK1 && m_hHOOK2)
        {
            bResult = TRUE;
        }
        return bResult;
    }

    BOOL CKeyboardHookProtect::StopHook()
    {
        if (m_hHOOK1)
        {
            UnhookWindowsHookEx(m_hHOOK1);
            m_hHOOK1 = NULL;
        }
        if (m_hHOOK2)
        {
            UnhookWindowsHookEx(m_hHOOK2);
            m_hHOOK2 = NULL;
        }
        return TRUE;
    }

    BOOL CKeyboardHookProtect::DoOnTimeProc()
    {
        if (m_bExit)
        {
            return FALSE;
        }
        BOOL bResult=FALSE;
        if (m_hHOOK1)
        {
            UnhookWindowsHookEx(m_hHOOK1);
            m_hHOOK1 = NULL;
        }
        m_hHOOK1 = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, NULL, m_dwMainThreadId);
        if (m_hHOOK2)
        {
            UnhookWindowsHookEx(m_hHOOK2);
            m_hHOOK2 = NULL;
        }
        m_hHOOK2 = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, NULL, m_dwMainThreadId);
        if (m_hHOOK1 && m_hHOOK2)
        {
            bResult = TRUE;
        }
        return bResult;
    }
}