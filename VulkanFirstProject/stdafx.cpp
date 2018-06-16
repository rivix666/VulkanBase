#include "stdafx.h"
#include <atlbase.h>

CEngine* g_Engine = nullptr;

namespace utils
{
    bool FatalError(HWND hwnd, LPCWSTR msg)
    {
        if (hwnd)
            MessageBox(hwnd, msg, L"Fatal Error!", MB_ICONERROR);
        else
            LogD("Fatal Error - " << msg);
        return false;
    }

    bool FatalError(HWND hwnd, LPCSTR msg)
    {
        if (hwnd)
            MessageBoxA(hwnd, msg, "Fatal Error!", MB_ICONERROR);
        else
            LogD("Fatal Error - " << msg);
        return false;     
    }
    void UpdateWindowBar(HWND hwnd)
    {
        static CTimer spf_timer(ETimerType::MiliSeconds);

        //#REDESIGN
        //static ParticleBufferData* partData = g_Engine->GetParticleManager()->m_ParticlesData;

        std::wstring ws;
        ws = W_WINDOW_TITLE;
        ws += L" - Num of particles: ";
        //ws += std::to_wstring(partData->m_iCPUFree);
        ws += L" | Sorted on: ";
        //ws += g_Engine->m_SortOnGPU ? L"GPU" : L"CPU";
        ws += L" | SPF: ";
        ws += std::to_wstring(spf_timer.getElapsedTime());
        ws += L" | FPS: ";
        ws += std::to_wstring(1000.0 / spf_timer.getElapsedTime());

        SetWindowText(hwnd, ws.c_str());
        spf_timer.startTimer();
    }
}