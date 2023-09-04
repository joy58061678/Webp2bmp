// WebpContextMenu.cpp : CWebpContextMenu ��ʵ��

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

	//�����ݶ����в���CF_HDROP��������
	hr = pdtobj->GetData(&fmt, &stg);
	if (FAILED(hr))
	{
		return E_INVALIDARG;
	}

	//��ȡָ��ʵ�����ݵ�ָ��
	hDrop = (HDROP)GlobalLock(stg.hGlobal);
	if (hDrop == NULL)
	{
		return E_INVALIDARG;
	}

// ��ѯѡ����ļ�����
	UINT uFilesCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	if (0 == uFilesCount)
	{
		GlobalUnlock(stg.hGlobal);
		ReleaseStgMedium(&stg);
		return E_INVALIDARG;
	}

	// ���ļ����Ʊ����ڱ�����
	hr = E_INVALIDARG;
	TCHAR szFile[MAX_PATH] = { 0 };
	for (UINT i = 0; i < uFilesCount; i++)
	{
		// ȡ����һ���ļ���.
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

	//�ͷ���Դ
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
	if (uFlags & CMF_DEFAULTONLY)//˫��ʱ����Ĭ�ϣ���������
	{
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, (ULONG)0);
	}
	HRESULT hr = S_OK;
	UINT nCmdId = idCmdFirst;
	UINT uAddCount = 0;

	BOOL bRet = FALSE;
	bRet = InsertMenu(hmenu, indexMenu + uAddCount, MF_SEPARATOR | MF_BYPOSITION, nCmdId++, NULL);//�ָ���
	uAddCount++;
	//��Ӱ�ť
// 	bRet = InsertMenu(hmenu, indexMenu + uAddCount, MF_STRING | MF_BYPOSITION, nCmdId++, TEXT("webpתbmp"));
	//���ð�ť��ͼ��
// 	HICON hIcon = (HICON)LoadImage(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
// 	m_hBitmap = IconToBitmap(hIcon);

	HMENU hSubMenu = CreatePopupMenu();
// 	AppendMenu(hSubMenu, MF_STRING, idCmdFirst++, L"ת��PNG");
// 	AppendMenu(hSubMenu, MF_STRING, idCmdFirst++, L"ת��BMP");

	InsertMenu(hSubMenu, 0, MF_BYPOSITION | MF_STRING, nCmdId++, TEXT("ת��PNG"));
	bRet = SetMenuItemBitmaps(hSubMenu, 0, MF_BYPOSITION, m_hBitmapPng, m_hBitmapPng);
	InsertMenu(hSubMenu, 1, MF_BYPOSITION | MF_STRING, nCmdId++, TEXT("ת��BMP"));
	bRet = SetMenuItemBitmaps(hSubMenu, 1, MF_BYPOSITION, m_hBitmapBmp, m_hBitmapBmp);

	InsertMenu(hmenu, indexMenu + uAddCount, MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, TEXT("Webpת������"));
	bRet = SetMenuItemBitmaps(hmenu, indexMenu + uAddCount, MF_BYPOSITION, m_hBitmap, m_hBitmap);
	uAddCount++;

	bRet = InsertMenu(hmenu, indexMenu + uAddCount, MF_SEPARATOR | MF_BYPOSITION, nCmdId++, NULL);//�ָ���
	uAddCount++;

	//���һ������Ϊ�����Ĳ˵��ĸ���
	return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, (ULONG)(uAddCount));
}

HRESULT STDMETHODCALLTYPE CWebpContextMenu::InvokeCommand(
	/* [annotation][in] */
	_In_  CMINVOKECOMMANDINFO *pici)
{
	HRESULT hr = S_OK;

	//���lpVerbָ��һ���ַ��������Դ˴ε���
	if (0 != HIWORD(pici->lpVerb))
	{
		return E_INVALIDARG;
	}

	//���lpVerb�ǲ�����ӵ�����,��������Ӧ
	switch (LOWORD(pici->lpVerb))//Ϊ�����˵�ʱ��indexMenu��ֵ
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
	//�Ƿ��ǻ�ȡ��ʾ�����ð����ַ���
	if (uType & GCS_HELPTEXT)
	{
		//ѡ������
		LPCTSTR szPrompt = NULL;
		switch (idCmd)
		{
		case 1:
		{

			szPrompt = TEXT("Help��webp�ļ�ת��Ϊpng�ļ���");
			break;
		}
		case 2:
		{
			szPrompt = TEXT("Help��webp�ļ�ת��Ϊbmp�ļ���");
			break;
		}
		default:
			return E_INVALIDARG;
		}

		//�ַ�����
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

	//�ҵ�dwebp.exeĿ¼
	GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szModulePath, MAX_PATH);

	TCHAR* pszFileName = PathFindFileName(szModulePath);
	//�Ƴ��ļ�������\\Ҳ�Ƴ���
	if (PathRemoveFileSpec(szModulePath))
	{
		if (wcscmp(pszFileName, TEXT("Webp2bmp64.dll")) == 0)
			PathRemoveFileSpec(szModulePath);//���Ƴ�һ��Ŀ¼

		//ƴ·��
		if (PathAppend(szModulePath, TEXT("dwebp.exe")))
		{
			strExe = szModulePath;
		}
	}
	if (strExe == L"" || PathInfo::FileExists(strExe) == false)
	{
		::MessageBox(NULL, TEXT("û���ҵ�dwebp.exe�ļ���"), TEXT("����"), MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING);
		return E_FAIL;
	}

	for each (std::wstring var in m_vtFilePaths)
	{
		//��ȡ�ļ���
		std::wstring strFileName = PathInfo::getFileTitle(var);
		std::wstring strDirectory = PathInfo::GetDirectory(var);

		std::wstring strBmpName = strFileName + TEXT(".png");
		//�������ļ�
		std::wstring strNewFileName = CreateNewName(strDirectory, strBmpName);
		if (strNewFileName == L"")
		{
			return E_FAIL;
		}
		std::wstring strParams = L"\"" + var + L"\"" + L" -o " + L"\"" + strNewFileName + L"\"";//·���ӡ�����ֹ�ո�
		//����ת��
		ShellExecute(NULL, NULL, strExe.c_str(), strParams.c_str(), strDirectory.c_str(), SW_HIDE);
	}
	return S_OK;
}

HRESULT CWebpContextMenu::OnConvertWebpToBmp()
{
	std::wstring strExe = L"";
	TCHAR szModulePath[MAX_PATH] = { 0 };

	//�ҵ�dwebp.exeĿ¼
	GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szModulePath, MAX_PATH);
	//lstrcpyn(pszIconFile, szModulePath, cchMax);
	TCHAR* pszFileName = PathFindFileName(szModulePath);
	//�Ƴ��ļ�������\\Ҳ�Ƴ���
	if (PathRemoveFileSpec(szModulePath))
	{
		if (wcscmp(pszFileName, TEXT("Webp2bmp64.dll")) == 0)
			PathRemoveFileSpec(szModulePath);//���Ƴ�һ��Ŀ¼

		//ƴ·���Զ����ӷ�б��
		if (PathAppend(szModulePath, TEXT("dwebp.exe")))
		{
			strExe = szModulePath;
		}
	}
	if (strExe == L"" || PathInfo::FileExists(strExe) == false)
	{
		::MessageBox(NULL, TEXT("û���ҵ�dwebp.exe�ļ���"), TEXT("����"), MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING);
		return E_FAIL;
	}

	for each (std::wstring var in m_vtFilePaths)
	{
		//��ȡ�ļ���
		std::wstring strFileName = PathInfo::getFileTitle(var);
		std::wstring strDirectory = PathInfo::GetDirectory(var);
		
		std::wstring strBmpName = strFileName + TEXT(".bmp");
		//�������ļ�
		std::wstring strNewFileName = CreateNewName(strDirectory, strBmpName);
		if (strNewFileName == L"")
		{
			return E_FAIL;
		}
		std::wstring strParams = L"\"" + var + L"\"" + L" -bmp -o " + L"\"" + strNewFileName + L"\"";//·���ӡ�����ֹ�ո�
		//����ת��
		ShellExecute(NULL, NULL, strExe.c_str(), strParams.c_str(), strDirectory.c_str(), SW_HIDE);
	}
	return S_OK;
}

//���Ҳ��������ļ�
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
			strFile = strDir + strFileName + L" ������" + std::to_wstring(uCopyIndex) + L"��" + strExt;
			uCopyIndex++;
		}
		else
		{
			strFile = strDir + strFileName + L" ����" + strExt;
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

	// ��ȡͼ����Ϣ
	GetIconInfo(hIcon, &iconInfo);

	// ����DC
	hDC = GetDC(NULL);
	hMemDC = CreateCompatibleDC(hDC);

	// ��ȡλͼ
	bmp = { 0 };
	GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bmp);

	// ����λͼ
	hBitmap = CreateCompatibleBitmap(hDC, bmp.bmWidth, bmp.bmHeight);
	SelectObject(hMemDC, hBitmap);

	// ����ͼ��
	DrawIcon(hMemDC, 0, 0, hIcon);

	// �ͷ���Դ
	DeleteObject(iconInfo.hbmColor);
	DeleteObject(iconInfo.hbmMask);
	DeleteDC(hMemDC);
	ReleaseDC(NULL, hDC);

	return hBitmap;
}
