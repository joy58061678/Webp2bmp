

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Aug 24 09:18:03 2023
 */
/* Compiler settings for Webp2bmp.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Webp2bmp_i_h__
#define __Webp2bmp_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWebpContextMenu_FWD_DEFINED__
#define __IWebpContextMenu_FWD_DEFINED__
typedef interface IWebpContextMenu IWebpContextMenu;

#endif 	/* __IWebpContextMenu_FWD_DEFINED__ */


#ifndef __WebpContextMenu_FWD_DEFINED__
#define __WebpContextMenu_FWD_DEFINED__

#ifdef __cplusplus
typedef class WebpContextMenu WebpContextMenu;
#else
typedef struct WebpContextMenu WebpContextMenu;
#endif /* __cplusplus */

#endif 	/* __WebpContextMenu_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IWebpContextMenu_INTERFACE_DEFINED__
#define __IWebpContextMenu_INTERFACE_DEFINED__

/* interface IWebpContextMenu */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IWebpContextMenu;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6856FD1D-1690-4342-9EF0-83B6E73D6AB1")
    IWebpContextMenu : public IDispatch
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct IWebpContextMenuVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWebpContextMenu * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWebpContextMenu * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWebpContextMenu * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWebpContextMenu * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWebpContextMenu * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWebpContextMenu * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWebpContextMenu * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IWebpContextMenuVtbl;

    interface IWebpContextMenu
    {
        CONST_VTBL struct IWebpContextMenuVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWebpContextMenu_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWebpContextMenu_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWebpContextMenu_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWebpContextMenu_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IWebpContextMenu_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IWebpContextMenu_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IWebpContextMenu_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWebpContextMenu_INTERFACE_DEFINED__ */



#ifndef __Webp2bmpLib_LIBRARY_DEFINED__
#define __Webp2bmpLib_LIBRARY_DEFINED__

/* library Webp2bmpLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_Webp2bmpLib;

EXTERN_C const CLSID CLSID_WebpContextMenu;

#ifdef __cplusplus

class DECLSPEC_UUID("EB08C4B9-7BC7-4263-908E-C10CD45BE69C")
WebpContextMenu;
#endif
#endif /* __Webp2bmpLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


