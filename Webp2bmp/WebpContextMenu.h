// WebpContextMenu.h : CWebpContextMenu 的声明

#pragma once
#include "resource.h"       // 主符号



#include "Webp2bmp_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;
#include <vector>

// CWebpContextMenu

class ATL_NO_VTABLE CWebpContextMenu :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWebpContextMenu, &CLSID_WebpContextMenu>,
	public IDispatchImpl<IWebpContextMenu, &IID_IWebpContextMenu, &LIBID_Webp2bmpLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit,//接受explorer传递信息
	public IContextMenu//实现右键扩展
{
public:
	CWebpContextMenu()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WEBPCONTEXTMENU)


BEGIN_COM_MAP(CWebpContextMenu)
	COM_INTERFACE_ENTRY(IWebpContextMenu)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)//添加入口点
	COM_INTERFACE_ENTRY(IContextMenu)//添加入口点
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		m_vtFilePaths.clear();
		m_hBitmap = (HBITMAP)LoadImage(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 16, 16, LR_DEFAULTCOLOR);
		m_hBitmapBmp = (HBITMAP)LoadImage(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 16, 16, LR_DEFAULTCOLOR);
		m_hBitmapPng = (HBITMAP)LoadImage(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IDB_BITMAP3), IMAGE_BITMAP, 16, 16, LR_DEFAULTCOLOR);

		return S_OK;
	}

	void FinalRelease()
	{
		m_vtFilePaths.clear();
		if (m_hBitmap != NULL)
		{
			DeleteObject(m_hBitmap);
			m_hBitmap = NULL;
		}
		if (m_hBitmapBmp!= NULL)
		{
			DeleteObject(m_hBitmapBmp);
			m_hBitmapBmp = NULL;
		}
		if (m_hBitmapPng != NULL)
		{
			DeleteObject(m_hBitmapPng);
			m_hBitmapPng = NULL;
		}
	}

	//扩展接口
public:
	//IShellExtInit
	virtual HRESULT STDMETHODCALLTYPE Initialize(
		/* [annotation][unique][in] */
		_In_opt_  PCIDLIST_ABSOLUTE pidlFolder,
		/* [annotation][unique][in] */
		_In_opt_  IDataObject *pdtobj,
		/* [annotation][unique][in] */
		_In_opt_  HKEY hkeyProgID);



	//IContextMenu
	virtual HRESULT STDMETHODCALLTYPE QueryContextMenu(
		/* [annotation][in] */
		_In_  HMENU hmenu,
		/* [annotation][in] */
		_In_  UINT indexMenu,
		/* [annotation][in] */
		_In_  UINT idCmdFirst,
		/* [annotation][in] */
		_In_  UINT idCmdLast,
		/* [annotation][in] */
		_In_  UINT uFlags);

	virtual HRESULT STDMETHODCALLTYPE InvokeCommand(
		/* [annotation][in] */
		_In_  CMINVOKECOMMANDINFO *pici);

	virtual HRESULT STDMETHODCALLTYPE GetCommandString(
		/* [annotation][in] */
		_In_  UINT_PTR idCmd,
		/* [annotation][in] */
		_In_  UINT uType,
		/* [annotation][in] */
		_Reserved_  UINT *pReserved,
		/* [annotation][out] */
		_Out_writes_bytes_((uType & GCS_UNICODE) ? (cchMax * sizeof(wchar_t)) : cchMax) _When_(!(uType & (GCS_VALIDATEA | GCS_VALIDATEW)), _Null_terminated_)  CHAR *pszName,
		/* [annotation][in] */
		_In_  UINT cchMax);

public:
	HRESULT OnConvertWebpToBmp();
	HRESULT OnConvertWebpToPng();

	HBITMAP IconToBitmap(HICON hIcon);

	//查找并重命名文件
	std::wstring CreateNewName(std::wstring strDir, std::wstring strName);
private:
	std::vector<std::wstring> m_vtFilePaths;

	HBITMAP m_hBitmap;
	HBITMAP m_hBitmapBmp;
	HBITMAP m_hBitmapPng;
};

OBJECT_ENTRY_AUTO(__uuidof(WebpContextMenu), CWebpContextMenu)
