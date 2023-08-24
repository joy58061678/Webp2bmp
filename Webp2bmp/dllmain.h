// dllmain.h : 模块类的声明。

class CWebp2bmpModule : public ATL::CAtlDllModuleT< CWebp2bmpModule >
{
public :
	DECLARE_LIBID(LIBID_Webp2bmpLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_WEBP2BMP, "{54AC9916-5147-4C0B-83B1-84BF569FD092}")
};

extern class CWebp2bmpModule _AtlModule;
