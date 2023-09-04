// WebpContextMenu.cpp : CWebpContextMenu 的实现

#include "stdafx.h"
#include "WebpContextMenu.h"
#include "PathInfo.h"
#include <string>

//////////////////////////////////////////////////////////////////////////
//IShellExtInit
HRESULT STDMETHODCALLTYPE CWebpContextMenu::Initialize(
	/* [annotation][unique][in] */
	_In_opt_  PCIDLIST_ABSOLUTE pidlFolder,
	/* [annotation][unique][in] */
	_In_opt_  IDataObject *pdtobj,
	/* [annotation][unique][in] */
	_In_opt_  HKEY hkeyProgID)
{
	HRESULT hr = E_INVALIDARG;

	FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HDROP hDrop = NULL;

	//在数据对象中查找CF_HDROP类型数据
	hr = pdtobj->GetData(&fmt, &stg);
	if (FAILED(hr))
	{
		return E_INVALIDARG;
	}

	//获取指向实际数据的指针
	hDrop = (HDROP)GlobalLock(stg.hGlobal);
	if (hDrop == NULL)
	{
		return E_INVALIDARG;
	}

// 查询选择的文件数量
	UINT uFilesCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	if (0 == uFilesCount)
	{
		GlobalUnlock(stg.hGlobal);
		ReleaseStgMedium(&stg);
		return E_INVALIDARG;
	}

	// 将文件名称保存在变量中
	hr = E_INVALIDARG;
	TCHAR szFile[MAX_PATH] = { 0 };
	for (UINT i = 0; i < uFilesCount; i++)
	{
		// 取得下一个文件名.
		if (0 != DragQueryFile(hDrop, i, szFile, MAX_PATH))
		{
			std::wstring strExt = PathInfo::GetFileExtension(szFile);
			if (strExt == TEXT(".webp"))
			{
				m_vtFilePaths.push_back(szFile);
				hr = S_OK;
			}
		}
		else
		{
			hr = E_INVALIDARG;
			break;
		}
	}

	//释放资源
	GlobalUnlock(stg.hGlobal);
	ReleaseStgMedium(&stg);
	return hr;
}

//////////////////////////////////////////////////////////////////////////
//IContextMenu
HRESULT STDMETHODCALLTYPE CWebpContextMenu::QueryContextMenu(
	/* [annotation][in] */
	_In_  HMENU hmenu,
	/* [annotation][in] */
	_In_  UINT indexMenu,
	/* [annotation][in] */
	_In_  UINT idCmdFirst,
	/* [annotation][in] */
	_In_  UINT idCmdLast,
	/* [annotation][in] */
	_In_  UINT uFlags)
{
	if (uFlags & CMF_DEFAULTONLY)//双击时触发默认，不做处理
	{
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, (ULONG)0);
	}
	HRESULT hr = S_OK;
	UINT nCmdId = idCmdFirst;
	UINT uAddCount = 0;

	BOOL bRet = FALSE;
	bRet = InsertMenu(hmenu, indexMenu + uAddCount, MF_SEPARATOR | MF_BYPOSITION, nCmdId++, NULL);//分隔线
	uAddCount++;
	//添加按钮
// 	bRet = InsertMenu(hmenu, indexMenu + uAddCount, MF_STRING | MF_BYPOSITION, nCmdId++, TEXT("webp转bmp"));
	//设置按钮的图标
// 	HICON hIcon = (HICON)LoadImage(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
// 	m_hBitmap = IconToBitmap(hIcon);

	HMENU hSubMenu = CreatePopupMenu();
// 	AppendMenu(hSubMenu, MF_STRING, idCmdFirst++, L"转成PNG");
// 	AppendMenu(hSubMenu, MF_STRING, idCmdFirst++, L"转成BMP");

	InsertMenu(hSubMenu, 0, MF_BYPOSITION | MF_STRING, nCmdId++, TEXT("转成PNG"));
	bRet = SetMenuItemBitmaps(hSubMenu, 0, MF_BYPOSITION, m_hBitmapPng, m_hBitmapPng);
	InsertMenu(hSubMenu, 1, MF_BYPOSITION | MF_STRING, nCmdId++, TEXT("转成BMP"));
	bRet = SetMenuItemBitmaps(hSubMenu, 1, MF_BYPOSITION, m_hBitmapBmp, m_hBitmapBmp);

	InsertMenu(hmenu, indexMenu + uAddCount, MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, TEXT("Webp转换工具"));
	bRet = SetMenuItemBitmaps(hmenu, indexMenu + uAddCount, MF_BYPOSITION, m_hBitmap, m_hBitmap);
	uAddCount++;

	bRet = InsertMenu(hmenu, indexMenu + uAddCount, MF_SEPARATOR | MF_BYPOSITION, nCmdId++, NULL);//分隔线
	uAddCount++;

	//最后一个参数为创建的菜单的个数
	return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, (ULONG)(uAddCount));
}

HRESULT STDMETHODCALLTYPE CWebpContextMenu::InvokeCommand(
	/* [annotation][in] */
	_In_  CMINVOKECOMMANDINFO *pici)
{
	HRESULT hr = S_OK;

	//如果lpVerb指向一个字符串，忽略此次调用
	if (0 != HIWORD(pici->lpVerb))
	{
		return E_INVALIDARG;
	}

	//检查lpVerb是不是添加的命令,并进行响应
	switch (LOWORD(pici->lpVerb))//为创建菜单时的indexMenu数值
	{
	case 1://PNG
	{
		hr = OnConvertWebpToPng();
		break;
	}
	case 2://BMP
	{
 		hr = OnConvertWebpToBmp();
		break;
	}
	default:
		return E_INVALIDARG;
	}

	return hr;
}

HRESULT STDMETHODCALLTYPE CWebpContextMenu::GetCommandString(
	/* [annotation][in] */
	_In_  UINT_PTR idCmd,
	/* [annotation][in] */
	_In_  UINT uType,
	/* [annotation][in] */
	_Reserved_  UINT *pReserved,
	/* [annotation][out] */
	_Out_writes_bytes_((uType & GCS_UNICODE) ? (cchMax * sizeof(wchar_t)) : cchMax) _When_(!(uType & (GCS_VALIDATEA | GCS_VALIDATEW)), _Null_terminated_)  CHAR *pszName,
	/* [annotation][in] */
	_In_  UINT cchMax)
{
	HRESULT hr = S_OK;
	//是否是获取提示，设置帮助字符串
	if (uType & GCS_HELPTEXT)
	{
		//选择内容
		LPCTSTR szPrompt = NULL;
		switch (idCmd)
		{
		case 1:
		{

			szPrompt = TEXT("Help：webp文件转化为png文件。");
			break;
		}
		case 2:
		{
			szPrompt = TEXT("Help：webp文件转化为bmp文件。");
			break;
		}
		default:
			return E_INVALIDARG;
		}

		//字符编码
		USES_CONVERSION;
		if (uType & GCS_UNICODE)
		{
			lstrcpynW((LPWSTR)pszName, T2CW(szPrompt), cchMax-1);
		}
		else
		{
			lstrcpynA(pszName, T2CA(szPrompt), cchMax-1);
		}
	}
	return hr;
}

//////////////////////////////////////////////////////////////////////////
//
HRESULT CWebpContextMenu::OnConvertWebpToPng()
{
	std::wstring strExe = L"";
	TCHAR szModulePath[MAX_PATH] = { 0 };

	//找到dwebp.exe目录
	GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szModulePath, MAX_PATH);

	TCHAR* pszFileName = PathFindFileName(szModulePath);
	//移除文件名，连\\也移除了
	if (PathRemoveFileSpec(szModulePath))
	{
		if (wcscmp(pszFileName, TEXT("Webp2bmp64.dll")) == 0)
			PathRemoveFileSpec(szModulePath);//再移除一层目录

		//拼路径
		if (PathAppend(szModulePath, TEXT("dwebp.exe")))
		{
			strExe = szModulePath;
		}
	}
	if (strExe == L"" || PathInfo::FileExists(strExe) == false)
	{
		::MessageBox(NULL, TEXT("没有找到dwebp.exe文件！"), TEXT("警告"), MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING);
		return E_FAIL;
	}

	for each (std::wstring var in m_vtFilePaths)
	{
		//获取文件名
		std::wstring strFileName = PathInfo::getFileTitle(var);
		std::wstring strDirectory = PathInfo::GetDirectory(var);

		std::wstring strBmpName = strFileName + TEXT(".png");
		//重命名文件
		std::wstring strNewFileName = CreateNewName(strDirectory, strBmpName);
		if (strNewFileName == L"")
		{
			return E_FAIL;
		}
		std::wstring strParams = L"\"" + var + L"\"" + L" -o " + L"\"" + strNewFileName + L"\"";//路径加‘’防止空格
		//调用转换
		ShellExecute(NULL, NULL, strExe.c_str(), strParams.c_str(), strDirectory.c_str(), SW_HIDE);
	}
	return S_OK;
}

HRESULT CWebpContextMenu::OnConvertWebpToBmp()
{
	std::wstring strExe = L"";
	TCHAR szModulePath[MAX_PATH] = { 0 };

	//找到dwebp.exe目录
	GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szModulePath, MAX_PATH);
	//lstrcpyn(pszIconFile, szModulePath, cchMax);
	TCHAR* pszFileName = PathFindFileName(szModulePath);
	//移除文件名，连\\也移除了
	if (PathRemoveFileSpec(szModulePath))
	{
		if (wcscmp(pszFileName, TEXT("Webp2bmp64.dll")) == 0)
			PathRemoveFileSpec(szModulePath);//再移除一层目录

		//拼路径自动增加反斜杠
		if (PathAppend(szModulePath, TEXT("dwebp.exe")))
		{
			strExe = szModulePath;
		}
	}
	if (strExe == L"" || PathInfo::FileExists(strExe) == false)
	{
		::MessageBox(NULL, TEXT("没有找到dwebp.exe文件！"), TEXT("警告"), MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING);
		return E_FAIL;
	}

	for each (std::wstring var in m_vtFilePaths)
	{
		//获取文件名
		std::wstring strFileName = PathInfo::getFileTitle(var);
		std::wstring strDirectory = PathInfo::GetDirectory(var);
		
		std::wstring strBmpName = strFileName + TEXT(".bmp");
		//重命名文件
		std::wstring strNewFileName = CreateNewName(strDirectory, strBmpName);
		if (strNewFileName == L"")
		{
			return E_FAIL;
		}
		std::wstring strParams = L"\"" + var + L"\"" + L" -bmp -o " + L"\"" + strNewFileName + L"\"";//路径加‘’防止空格
		//调用转换
		ShellExecute(NULL, NULL, strExe.c_str(), strParams.c_str(), strDirectory.c_str(), SW_HIDE);
	}
	return S_OK;
}

//查找并重命名文件
std::wstring CWebpContextMenu::CreateNewName(std::wstring strDir, std::wstring strName)
{
	if (strDir == L"" || strName == L"")
		return L"";

	std::wstring strFile = strDir + strName;

	bool bCopy = false;
	UINT uCopyIndex = 1;
	while (PathInfo::FileExists(strFile))
	{
		std::wstring strFileName = PathInfo::getFileTitle(strName);
		std::wstring strExt = PathInfo::GetFileExtension(strName);

		if (bCopy)
		{
			strFile = strDir + strFileName + L" 副本（" + std::to_wstring(uCopyIndex) + L"）" + strExt;
			uCopyIndex++;
		}
		else
		{
			strFile = strDir + strFileName + L" 副本" + strExt;
			bCopy = true;
		}
	}

	return strFile;
}

HBITMAP CWebpContextMenu::IconToBitmap(HICON hIcon)
{
	HDC hDC;
	HDC hMemDC;
	HBITMAP hBitmap;
	BITMAP bmp;
	ICONINFO iconInfo;

	// 获取图标信息
	GetIconInfo(hIcon, &iconInfo);

	// 创建DC
	hDC = GetDC(NULL);
	hMemDC = CreateCompatibleDC(hDC);

	// 获取位图
	bmp = { 0 };
	GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bmp);

	// 创建位图
	hBitmap = CreateCompatibleBitmap(hDC, bmp.bmWidth, bmp.bmHeight);
	SelectObject(hMemDC, hBitmap);

	// 绘制图标
	DrawIcon(hMemDC, 0, 0, hIcon);

	// 释放资源
	DeleteObject(iconInfo.hbmColor);
	DeleteObject(iconInfo.hbmMask);
	DeleteDC(hMemDC);
	ReleaseDC(NULL, hDC);

	return hBitmap;
}
