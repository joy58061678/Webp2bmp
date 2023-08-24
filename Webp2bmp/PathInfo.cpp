/*
Copyright 2014 Google Inc

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include "StdAfx.h"
#include "PathInfo.h"


PathInfo::PathInfo(void)
{
}


PathInfo::~PathInfo(void)
{
}


LONGLONG PathInfo::FileSize(const std::wstring& path)
{
	struct _stat info;
	if (_wstat(path.c_str(), &info) == 0)
	{
		return info.st_size;
	}
	else
	{
		return 0;
	}
}

bool PathInfo::FileExists(const std::wstring& path)
{
	if (path.length() == 0)
		return false;

	if (_waccess(path.c_str(), 0) == 0)
	{
		return true;
	}
	return false;
}

//获取文件名（不包含扩展名）
std::wstring PathInfo::getFileTitle(const std::wstring & absFileName)
{
#if defined(IOS) || defined(ANDROID)
	std::string file = getFileNameWithoutDirectory(absFileName);
	string::size_type pos = file.find_last_of('.');
	if (pos != string::npos)
		return file.substr(0, pos);
	else
		return file;
#else
	std::wstring file = getFileNameWithoutDirectory(absFileName);
	std::wstring wfile = file;
	std::string::size_type pos = wfile.find_last_of(L'.');
	if (pos != std::string::npos)
	{
		std::wstring result = wfile.substr(0, pos);
		return result;
	}
	return file;
#endif
}

//获取文件名（包含扩展名）
std::wstring PathInfo::getFileNameWithoutDirectory(const std::wstring & absFileName)
{
	using namespace std;
#if defined(IOS) || defined(ANDROID)
	string::size_type pos = absFileName.find_last_of("\\/");
	if (pos == string::npos) return absFileName;
	else return absFileName.substr(pos + 1);
#else
	std::wstring wAbsFileName = absFileName;
	string::size_type pos = wAbsFileName.find_last_of(L"\\/");
	if (pos == string::npos)
	{
		return absFileName;
	}
	std::wstring result = wAbsFileName.substr(pos + 1);
	return result;
#endif
}
std::wstring PathInfo::GetDirectory(const std::wstring wstrFilePath)
{
	std::wstring::size_type npos = wstrFilePath.find_last_of(L"\\");
	if (npos == std::wstring::npos)
		return L"";
	std::wstring wstrFolderPath = wstrFilePath.substr(0, npos+1);
	return wstrFolderPath;
}

//return ".docx"
std::wstring PathInfo::GetFileExtension(const std::wstring & fileName)
{
#if defined(IOS) || defined(ANDROID)
	//移动端是utf8编码，所以字符串采用的是string类型
	string::size_type pos = fileName.find_last_of('.');
	if (pos == string::npos) return L"";
	return fileName.substr(pos);
#else
	try
	{
		std::wstring wfileName = fileName;
		std::wstring::size_type pos = wfileName.find_last_of(L'.');
		if (pos == std::wstring::npos)
			return L"";
		std::wstring result = wfileName.substr(pos);
		return result;
	}
	catch (...)
	{
		return L"";
	}
#endif
}

bool PathInfo::IsDefaultOpen(const std::wstring & wstrExtension)
{
	if (wstrExtension.size() <= 0)
	{
		return FALSE;
	}

	HKEY hkeyExtension;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, wstrExtension.c_str(), NULL, KEY_READ, &hkeyExtension) != ERROR_SUCCESS)
	{
		return FALSE;
	}

	TCHAR szRootCache[256] = { 0 };
	DWORD sizeRootCache = sizeof(szRootCache);
	if (RegGetValue(hkeyExtension, NULL, L"", RRF_RT_REG_SZ, NULL, &szRootCache, &sizeRootCache) != ERROR_SUCCESS)
	{
		RegCloseKey(hkeyExtension);
		return FALSE;
	}

	RegCloseKey(hkeyExtension);

	if (wcscmp(szRootCache, L"") == 0)
	{
		HKEY hkeyOpen;
		std::wstring wstrOpen = wstrExtension;
		wstrOpen.append(L"\\OpenWithProgids");
		if (RegOpenKeyEx(HKEY_CLASSES_ROOT, wstrOpen.c_str(), NULL, KEY_READ, &hkeyOpen) != ERROR_SUCCESS)
		{	
			return FALSE;
		}

		TCHAR szOpen[MAX_PATH] = { 0 };
		DWORD sizeOpen = MAX_PATH;
		DWORD dwIndex = 0;
		DWORD dwType = REG_SZ;
		while (RegEnumValue(hkeyOpen, dwIndex, szOpen, &sizeOpen, NULL, &dwType, NULL, NULL) == ERROR_SUCCESS)
		{
			dwIndex++;
			DWORD sizeOpen = MAX_PATH;
			if (wcscmp(szOpen, L"") != 0)
			{
				RegCloseKey(hkeyOpen);
				return TRUE;
			}
		}

		RegCloseKey(hkeyOpen);
		return FALSE;
	}

	return TRUE;
}

std::wstring PathInfo::CombinePath(const std::wstring& path1, const std::wstring& path2)
{
	std::wstring path;
	if (path1.length() > 0 && path2.length() > 0)
	{
		if (path1[path1.length() - 1] == '\\' || path2[0] == '\\')
		{
			if (path1[path1.length() - 1] == '\\' && path2[0] == '\\')
				path = path1 + path2.substr(1);
			else
				path = path1 + path2;
		}
		else
			path = path1 + L"\\" + path2;
	}
	else if (path1.length() > 0)
	{
		path = path1;
	}
	else if (path2.length() > 0)
	{
		path = path2;
	}

	return path;
}

std::wstring PathInfo::GetFileUrl(const std::wstring& path)
{
	std::wstring url = path;
	size_t startIndex = std::wstring::npos;
	while (startIndex < url.length())
	{
		size_t index = url.find(L"\\", startIndex);
		if (index == std::wstring::npos)
			break;
		url.replace(index, 1, L"/");
		startIndex = index + 1;
	}

	if (url.length() > 0)
	{
		url = L"file:///" + url;
	}

	return url;
}

void PathInfo::CreatePath(const std::wstring& path, size_t last)
{
// 	LOGINFORMATION(L"PathInfo::CreatePath(const std::wstring& path, size_t last)");
	const auto parentPos = path.find_last_of(L"\\", last);
	const auto err = ::CreateDirectory(path.substr(0,parentPos).c_str(), NULL);
	if (err == 0 && GetLastError() == ERROR_PATH_NOT_FOUND)
	{
		CreatePath(path, parentPos-1);
		::CreateDirectory(path.substr(0,parentPos).c_str(), NULL);
	}
}
