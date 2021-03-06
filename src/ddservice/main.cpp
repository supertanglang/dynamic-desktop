#include <Windows.h>
#include <tchar.h>

#include <Win32Utils>

SERVICE_STATUS serviceStatus = { 0 };
SERVICE_STATUS_HANDLE serviceStatusHandle = nullptr;

VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
VOID WINAPI ServiceCtrlHandler(DWORD code);
DWORD WINAPI ServiceWorkerThread(LPVOID lpParam);

VOID Install();
VOID Uninstall();

#define SERVICE_NAME_DD TEXT("ddassvc")
#define SERVICE_DISPLAY_NAME_DD TEXT("Dynamic Desktop Auto Start Service")
#define SERVICE_DESCRIPTION_DD TEXT("Make Dynamic Desktop automatically run when the system starts. Dynamic Desktop will not auto start if you disabled this service.")

int _tmain(int argc, TCHAR *argv[])
{
    SERVICE_TABLE_ENTRY ServiceTable[] =
    {
        {const_cast<LPTSTR>(SERVICE_NAME_DD), (LPSERVICE_MAIN_FUNCTION)ServiceMain},
        {nullptr, nullptr}
    };
    for (unsigned int i = 1; i != argc; ++i)
        if ((_tcscmp(argv[i], TEXT("-i")) == 0) || (_tcscmp(argv[i], TEXT("-I")) == 0) || (_tcscmp(argv[i], TEXT("-install")) == 0) || (_tcscmp(argv[i], TEXT("-INSTALL")) == 0) || (_tcscmp(argv[i], TEXT("-Install")) == 0))
        {
            Install();
            break;
        }
        else if ((_tcscmp(argv[i], TEXT("-u")) == 0) || (_tcscmp(argv[i], TEXT("-U")) == 0) || (_tcscmp(argv[i], TEXT("-uninstall")) == 0) || (_tcscmp(argv[i], TEXT("-UNINSTALL")) == 0) || (_tcscmp(argv[i], TEXT("-Uninstall")) == 0))
        {
            Uninstall();
            break;
        }
    if (StartServiceCtrlDispatcher(ServiceTable) != TRUE)
        return GetLastError();
    return 0;
}

VOID Install()
{
    if (Win32Utils::isAutoStartServiceInstalled(SERVICE_NAME_DD))
    {
        OutputDebugString(TEXT("Service already installed. No need to install again."));
        return;
    }
    TCHAR filePath[MAX_PATH + 1] = { 0 };
    DWORD dwSize = GetModuleFileName(nullptr, filePath, MAX_PATH);
    filePath[dwSize] = 0;
    bool result = false;
    SC_HANDLE hSCM = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    if (hSCM != nullptr)
    {
        SC_HANDLE hService = CreateService(hSCM, SERVICE_NAME_DD, SERVICE_DISPLAY_NAME_DD, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, filePath, nullptr, nullptr, nullptr, nullptr, nullptr);
        if (hService != nullptr)
        {
            SERVICE_DESCRIPTION sdesc;
            sdesc.lpDescription = const_cast<LPTSTR>(SERVICE_DESCRIPTION_DD);
            result = ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &sdesc);
            CloseServiceHandle(hService);
        }
        CloseServiceHandle(hSCM);
    }
    if (result)
        OutputDebugString(TEXT("Installation succeeded."));
    else
        OutputDebugString(TEXT("Installation failed. Administrator privilege is needed."));
}

VOID Uninstall()
{
    if (!Win32Utils::isAutoStartServiceInstalled(SERVICE_NAME_DD))
    {
        OutputDebugString(TEXT("Service not installed. No need to uninstall."));
        return;
    }
    bool result = false;
    SC_HANDLE hSCM = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    if (hSCM != nullptr)
    {
        SC_HANDLE hService = OpenService(hSCM, SERVICE_NAME_DD, DELETE);
        if (hService != nullptr)
        {
            result = DeleteService(hService);
            CloseServiceHandle(hService);
        }
        CloseServiceHandle(hSCM);
    }
    if (result)
        OutputDebugString(TEXT("Uninstallation succeeded."));
    else
        OutputDebugString(TEXT("Uninstallation failed. Administrator privilege is needed."));
}

VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
    (void)argc;
    (void)argv;
    serviceStatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME_DD, ServiceCtrlHandler);
    if (serviceStatusHandle == nullptr)
        return;
    ZeroMemory(&serviceStatus, sizeof(serviceStatus));
    serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
    serviceStatus.dwControlsAccepted = 0;
    serviceStatus.dwCurrentState = SERVICE_START_PENDING;
    serviceStatus.dwWin32ExitCode = 0;
    serviceStatus.dwServiceSpecificExitCode = 0;
    serviceStatus.dwCheckPoint = 0;
    if (SetServiceStatus(serviceStatusHandle, &serviceStatus) != TRUE)
        return;
    serviceStatus.dwCurrentState = SERVICE_RUNNING;
    serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
    serviceStatus.dwWin32ExitCode = 0;
    serviceStatus.dwCheckPoint = 0;
    if (SetServiceStatus(serviceStatusHandle, &serviceStatus) != TRUE)
        return;
    HANDLE hThread = CreateThread(nullptr, 0, ServiceWorkerThread, nullptr, 0, nullptr);
    WaitForSingleObject(hThread, INFINITE);
    serviceStatus.dwCurrentState = SERVICE_STOPPED;
    serviceStatus.dwControlsAccepted = 0;
    serviceStatus.dwWin32ExitCode = 0;
    serviceStatus.dwCheckPoint = 3;
    SetServiceStatus(serviceStatusHandle, &serviceStatus);
}

VOID WINAPI ServiceCtrlHandler(DWORD code)
{
    switch (code)
    {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
        serviceStatus.dwCurrentState = SERVICE_STOPPED;
        serviceStatus.dwControlsAccepted = 0;
        serviceStatus.dwWin32ExitCode = 0;
        serviceStatus.dwCheckPoint = 3;
        break;
    case SERVICE_CONTROL_PAUSE:
        serviceStatus.dwCurrentState = SERVICE_PAUSED;
        break;
    case SERVICE_CONTROL_CONTINUE:
        serviceStatus.dwCurrentState = SERVICE_RUNNING;
        break;
    case SERVICE_CONTROL_INTERROGATE:
        break;
    default:
        break;
    };
    SetServiceStatus(serviceStatusHandle, &serviceStatus);
}

DWORD WINAPI ServiceWorkerThread(LPVOID lpParam)
{
    (void)lpParam;
    auto filePath = new TCHAR[MAX_PATH + 1];
    Win32Utils::getCurrentDir(filePath);
    _tcscat(filePath, TEXT("\\DDMain"));
#ifdef _DEBUG
    _tcscat(filePath, TEXT("d"));
#endif
    _tcscat(filePath, TEXT(".exe"));
    Win32Utils::launchSession1Process(filePath, nullptr);
    delete [] filePath;
    return ERROR_SUCCESS;
}
