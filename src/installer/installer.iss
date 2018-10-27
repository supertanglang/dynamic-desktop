#define DDName "Dynamic Desktop"
#ifndef DDVersion
#define DDVersion "1.0.0.0"
#endif
#define DDPublisher "wangwenx190"
#define DDURL "https://github.com/wangwenx190/dynamic-desktop"
#ifdef _DEBUG
#define DDExeName "launcherd.exe"
#else
#define DDExeName "launcher.exe"
#endif
#ifdef WIN64
#define DDInstallerName "dd_x64"
#define DDBinDir "bin64"
#else
#define DDInstallerName "dd_x86"
#define DDBinDir "bin"
#endif

[Setup]
AppId={{45875EE3-D96F-422D-80D8-56E450579FD8}
AppName={#DDName}
AppVersion={#DDVersion}
;AppVerName={#DDName} {#DDVersion}
AppPublisher={#DDPublisher}
AppPublisherURL={#DDURL}
AppSupportURL={#DDURL}
AppUpdatesURL={#DDURL}
DefaultDirName={src}\{#DDName}
DisableProgramGroupPage=yes
OutputDir=..\..\build
OutputBaseFilename={#DDInstallerName}
SetupIconFile=..\resources\icons\color_palette.ico
Compression=lzma2/ultra64
SolidCompression=yes
AppCopyright=GNU General Public License version 3
AppMutex=wangwenx190.DynamicDesktop.Controller.1000.AppMutex
SetupMutex=wangwenx190.DynamicDesktop.Extractor.1000.SetupMutex,Global\wangwenx190.DynamicDesktop.Extractor.1000.SetupMutex
PrivilegesRequired=lowest
RestartIfNeededByRun=False
TimeStampsInUTC=True
ShowLanguageDialog=no
Uninstallable=no
VersionInfoVersion={#DDVersion}
VersionInfoCompany=wangwenx190
VersionInfoDescription=Dynamic Desktop Self-Extractor
VersionInfoTextVersion={#DDVersion}
VersionInfoCopyright=GNU General Public License version 3
VersionInfoProductName=Dynamic Desktop
VersionInfoProductVersion={#DDVersion}
VersionInfoProductTextVersion={#DDVersion}
InternalCompressLevel=ultra64
MinVersion=0,6.1
DisableReadyPage=True
DisableReadyMemo=True
DisableDirPage=no
DirExistsWarning=yes
#ifdef WIN64
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64
#else
ArchitecturesAllowed=x86 x64
#endif
WizardImageFile=.\images\wizardimage@250.bmp
WizardSmallImageFile=.\images\wizardsmallimage@250.bmp
AppComments={#DDName}
AppContact={#DDPublisher}

[Languages]
Name: "en"; MessagesFile: ".\languages\en.isl"
Name: "zh_CN"; MessagesFile: ".\languages\zh_CN.isl"
Name: "zh_TW"; MessagesFile: ".\languages\zh_TW.isl"

[Messages]
BeveledLabel={#DDName}

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\..\build\{#DDBinDir}\*"; DestDir: "{app}"; Excludes: "vc_redist.x??.exe,vcredist_x??.exe,*.log,*.ini,*.lib,*.exp,*.ilk"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{userdesktop}\{#DDName}"; Filename: "{app}\{#DDExeName}"; WorkingDir: "{app}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#DDExeName}"; Description: "{cm:LaunchProgram,{#StringChange(DDName, '&', '&&')}}"; WorkingDir: "{app}"; Flags: nowait postinstall skipifdoesntexist skipifsilent
Filename: "{app}\{#DDExeName}"; WorkingDir: "{app}"; Flags: nowait skipifdoesntexist skipifnotsilent
