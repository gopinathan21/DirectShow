#include <tchar.h>
#include <strsafe.h>
#include <dshow.h>
#include <atlbase.h>
//#include <d3d9.h>
//#include <vmr9.h>

#pragma comment(lib,"Strmiids.lib")

#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }

HRESULT EnumerateDevices(REFGUID category, IEnumMoniker** ppEnum)
{
    // Create the System Device Enumerator.
    ICreateDevEnum* pDevEnum;   // Video and Audio interface object
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum)); // Creates the system device enumerator

    if (SUCCEEDED(hr))
    {
        // Create an enumerator for the category.
        hr = pDevEnum->CreateClassEnumerator(category, ppEnum, 0);  // Enumeration of 'category' objects
        if (hr == S_FALSE)
        {
            hr = VFW_E_NOT_FOUND;  // The category is empty. Treat as an error.
        }
        pDevEnum->Release(); // Deletes Enumeration object
    }
    return hr;
}

int main(void)
{
    IGraphBuilder* pGraph = NULL;
    IMediaControl* pControl = NULL; // contains methods for stopping and starting the graph
    IMediaEvent* pEvent = NULL;   // methods for getting events from the Filter Graph Manager

    IPin* m_pCamOutPin = NULL;
    IBaseFilter* m_pDF = NULL;
    IMoniker* pM = NULL;
    IEnumMoniker* pEnum = NULL; // Enumerator object

    // Initialize the COM library.
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        printf("ERROR - Could not initialize COM library");
        return 1;
    }

    // Create the filter graph manager and query for interfaces.
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
        IID_IGraphBuilder, (void**)&pGraph);
    if (FAILED(hr))
    {
        printf("ERROR - Could not create the Filter Graph Manager.");
        CoUninitialize();
        return 1;
    }

    // Bind
    hr = EnumerateDevices(CLSID_VideoInputDeviceCategory, &pEnum);
    if (FAILED(hr))
    {
        printf("ERROR - Could not enumerate video devices.");
        SAFE_RELEASE(pGraph);
        CoUninitialize();
        return 1;
    }

    hr = pEnum->Next(1, &pM, NULL);

    // Check if pM is not NULL before using it
    if (pM != NULL) {
        //BindToObject returns an HRESULT, so check the result
        hr = pM->BindToObject(0, 0, IID_IBaseFilter, (void**)&m_pDF);
        pM->Release(); // Release the moniker after binding
    }

    if (FAILED(hr) || m_pDF == NULL) {
        // Handle the error
        printf("ERROR - Could not bind moniker to filter.");
        SAFE_RELEASE(pEnum);
        SAFE_RELEASE(pGraph);
        CoUninitialize();
        return 1;
    }

    hr = pGraph->AddFilter(m_pDF, L"Video Capture");

    CComPtr<IEnumPins> pinEnum;
    m_pDF->EnumPins(&pinEnum);

    hr = pinEnum->Reset();
    hr = pinEnum->Next(1, &m_pCamOutPin, NULL);

    if (FAILED(hr))
    {
        printf("ERROR - Could not obtain video output pin.");
        SAFE_RELEASE(pEnum);
        SAFE_RELEASE(pGraph);
        CoUninitialize();
        return 1;
    }

    // control
    hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
    hr = pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);

    // Build the graph. IMPORTANT: Change this string to a file on your system.
    hr = pGraph->Render(m_pCamOutPin);
    if (SUCCEEDED(hr))
    {
        // Run the graph.
        hr = pControl->Run();
        if (SUCCEEDED(hr))
        {
            // Wait for completion.
            long evCode;
            pEvent->WaitForCompletion(INFINITE, &evCode);
        }
    }

    // Release resources
    SAFE_RELEASE(pEnum);
    SAFE_RELEASE(pGraph);
    CoUninitialize();

    return 0;
}
