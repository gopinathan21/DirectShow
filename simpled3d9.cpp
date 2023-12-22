//#include <tchar.h>
//#include <strsafe.h>
//#include <dshow.h>
//#include <atlbase.h>
//#include <d3d9.h>
//#include <vmr9.h>
//
//#pragma comment(lib, "Strmiids.lib")
//#pragma comment(lib, "d3d9.lib")
//
//#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }
//
//HRESULT EnumerateDevices(REFGUID category, IEnumMoniker** ppEnum)
//{
//    ICreateDevEnum* pDevEnum;
//    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));
//
//    if (SUCCEEDED(hr))
//    {
//        hr = pDevEnum->CreateClassEnumerator(category, ppEnum, 0);
//        if (hr == S_FALSE)
//        {
//            hr = VFW_E_NOT_FOUND;
//        }
//        pDevEnum->Release();
//    }
//    return hr;
//}
//
//void RenderWithDirect3D(IDirect3DDevice9* pD3DDevice)
//{
//    // Clear the screen to blue
//    if (pD3DDevice)
//    {
//        pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
//        pD3DDevice->Present(NULL, NULL, NULL, NULL);
//    }
//}
//
//int main(void)
//{
//    IGraphBuilder* pGraph = NULL;
//    IMediaControl* pControl = NULL;
//    IMediaEvent* pEvent = NULL;
//    IPin* m_pCamOutPin = NULL;
//    IBaseFilter* m_pDF = NULL;
//    IMoniker* pM = NULL;
//    IEnumMoniker* pEnum = NULL;
//
//    IDirect3D9* pD3D = NULL;
//    IDirect3DDevice9* pD3DDevice = NULL;
//
//    HRESULT hr = CoInitialize(NULL);
//    if (FAILED(hr))
//    {
//        printf("ERROR - Could not initialize COM library");
//        return 1;
//    }
//
//    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);
//    if (FAILED(hr))
//    {
//        printf("ERROR - Could not create the Filter Graph Manager.");
//        CoUninitialize();
//        return 1;
//    }
//
//    hr = EnumerateDevices(CLSID_VideoInputDeviceCategory, &pEnum);
//    if (FAILED(hr))
//    {
//        printf("ERROR - Could not enumerate video devices.");
//        SAFE_RELEASE(pGraph);
//        CoUninitialize();
//        return 1;
//    }
//
//    hr = pEnum->Next(1, &pM, NULL);
//
//    if (pM != NULL) {
//        hr = pM->BindToObject(0, 0, IID_IBaseFilter, (void**)&m_pDF);
//        pM->Release();
//    }
//
//    if (FAILED(hr) || m_pDF == NULL) {
//        printf("ERROR - Could not bind moniker to filter.");
//        SAFE_RELEASE(pEnum);
//        SAFE_RELEASE(pGraph);
//        CoUninitialize();
//        return 1;
//    }
//
//    hr = pGraph->AddFilter(m_pDF, L"Video Capture");
//
//    CComPtr<IEnumPins> pinEnum;
//    m_pDF->EnumPins(&pinEnum);
//
//    hr = pinEnum->Reset();
//    hr = pinEnum->Next(1, &m_pCamOutPin, NULL);
//
//    if (FAILED(hr))
//    {
//        printf("ERROR - Could not obtain video output pin.");
//        SAFE_RELEASE(pEnum);
//        SAFE_RELEASE(pGraph);
//        CoUninitialize();
//        return 1;
//    }
//
//    hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
//    hr = pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);
//
//    // Initialize Direct3D 
//    pD3D = Direct3DCreate9(D3D_SDK_VERSION); // Creating a device https://learn.microsoft.com/en-us/windows/win32/direct3d9/creating-a-device
//    if (pD3D)
//    {
//        D3DPRESENT_PARAMETERS d3dpp = { 0 }; 
//        d3dpp.Windowed = TRUE;
//        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
//
//        if (SUCCEEDED(pD3D->CreateDevice(
//            D3DADAPTER_DEFAULT,
//            D3DDEVTYPE_HAL,
//            GetConsoleWindow(),
//            D3DCREATE_HARDWARE_VERTEXPROCESSING,
//            &d3dpp,
//            &pD3DDevice)))
//        {
//            // Render with Direct3D before or after video rendering
//            RenderWithDirect3D(pD3DDevice);
//        }
//
//        SAFE_RELEASE(pD3D);
//    }
//
//    hr = pGraph->Render(m_pCamOutPin);
//    if (SUCCEEDED(hr))
//    {
//        hr = pControl->Run();
//        if (SUCCEEDED(hr))
//        {
//            long evCode;
//            pEvent->WaitForCompletion(INFINITE, &evCode);
//        }
//    }
//
//    SAFE_RELEASE(pEnum);
//    SAFE_RELEASE(pGraph);
//    SAFE_RELEASE(pD3DDevice);
//    CoUninitialize();
//
//    return 0;
//}
