// WebpContextMenu.h : CWebpContextMenu ������

#pragma once
#include "resource.h"       // ������



#include "Webp2bmp_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;
#include <vector>

// CWebpContextMenu

class ATL_NO_VTABLE CWebpContextMenu :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWebpContextMenu, &CLSID_WebpContextMenu>,
	public IDispatchImpl<IWebpContextMenu, &IID_IWebpContextMenu, &LIBID_Webp2bmpLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit,//����explorer������Ϣ
	public IContextMenu//ʵ���Ҽ���չ
{
public:
	CWebpContextMenu()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WEBPCONTEXTMENU)


BEGIN_COM_MAP(CWebpContextMenu)
	COM_INTERFACE_ENTRY(IWebpContextMenu)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)//�����ڵ�
	COM_INTERFACE_ENTRY(IContextMenu)//�����ڵ�
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		m_vtFilePaths.clear();
		m_hBitmap = NULL;
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
	}

	//��չ�ӿ�
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
	HBITMAP IconToBitmap(HICON hIcon);

	//���Ҳ��������ļ�
	std::wstring CreateNewName(std::wstring strDir, std::wstring strName);
private:
	std::vector<std::wstring> m_vtFilePaths;
	HBITMAP m_hBitmap;
};

OBJECT_ENTRY_AUTO(__uuidof(WebpContextMenu), CWebpContextMenu)
