/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Feb 23 19:25:36 2001
 */
/* Compiler settings for C:\Sources\Program sources\CVS home\windev\devicemanager\devicemanager.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __devicemanager_h__
#define __devicemanager_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IInformation_FWD_DEFINED__
#define __IInformation_FWD_DEFINED__
typedef interface IInformation IInformation;
#endif 	/* __IInformation_FWD_DEFINED__ */


#ifndef __IDevice_FWD_DEFINED__
#define __IDevice_FWD_DEFINED__
typedef interface IDevice IDevice;
#endif 	/* __IDevice_FWD_DEFINED__ */


#ifndef ___IDeviceEvents_FWD_DEFINED__
#define ___IDeviceEvents_FWD_DEFINED__
typedef interface _IDeviceEvents _IDeviceEvents;
#endif 	/* ___IDeviceEvents_FWD_DEFINED__ */


#ifndef __IKernel_FWD_DEFINED__
#define __IKernel_FWD_DEFINED__
typedef interface IKernel IKernel;
#endif 	/* __IKernel_FWD_DEFINED__ */


#ifndef ___IKernelEvents_FWD_DEFINED__
#define ___IKernelEvents_FWD_DEFINED__
typedef interface _IKernelEvents _IKernelEvents;
#endif 	/* ___IKernelEvents_FWD_DEFINED__ */


#ifndef ___IInformationEvents_FWD_DEFINED__
#define ___IInformationEvents_FWD_DEFINED__
typedef interface _IInformationEvents _IInformationEvents;
#endif 	/* ___IInformationEvents_FWD_DEFINED__ */


#ifndef __Device_FWD_DEFINED__
#define __Device_FWD_DEFINED__

#ifdef __cplusplus
typedef class Device Device;
#else
typedef struct Device Device;
#endif /* __cplusplus */

#endif 	/* __Device_FWD_DEFINED__ */


#ifndef __Information_FWD_DEFINED__
#define __Information_FWD_DEFINED__

#ifdef __cplusplus
typedef class Information Information;
#else
typedef struct Information Information;
#endif /* __cplusplus */

#endif 	/* __Information_FWD_DEFINED__ */


#ifndef __Kernel_FWD_DEFINED__
#define __Kernel_FWD_DEFINED__

#ifdef __cplusplus
typedef class Kernel Kernel;
#else
typedef struct Kernel Kernel;
#endif /* __cplusplus */

#endif 	/* __Kernel_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IInformation_INTERFACE_DEFINED__
#define __IInformation_INTERFACE_DEFINED__

/* interface IInformation */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IInformation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0FDFFA99-68D5-4BE0-8984-FB6A3A2C0AAC")
    IInformation : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IInformationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IInformation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IInformation __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IInformation __RPC_FAR * This);
        
        END_INTERFACE
    } IInformationVtbl;

    interface IInformation
    {
        CONST_VTBL struct IInformationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IInformation_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IInformation_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IInformation_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IInformation_INTERFACE_DEFINED__ */


#ifndef __IDevice_INTERFACE_DEFINED__
#define __IDevice_INTERFACE_DEFINED__

/* interface IDevice */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDevice;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6575FAAB-8750-42D2-ADDD-4BE764AA2767")
    IDevice : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsValid( 
            /* [out] */ BOOL __RPC_FAR *valid) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetDeviceName( 
            /* [in] */ BSTR devname) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Map( 
            /* [in] */ unsigned long address,
            /* [in] */ unsigned long size) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Unmap( 
            /* [in] */ unsigned long address) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RaiseInterrupt( 
            /* [in] */ unsigned long inhno) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDeviceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDevice __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDevice __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDevice __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsValid )( 
            IDevice __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *valid);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDeviceName )( 
            IDevice __RPC_FAR * This,
            /* [in] */ BSTR devname);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Map )( 
            IDevice __RPC_FAR * This,
            /* [in] */ unsigned long address,
            /* [in] */ unsigned long size);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unmap )( 
            IDevice __RPC_FAR * This,
            /* [in] */ unsigned long address);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RaiseInterrupt )( 
            IDevice __RPC_FAR * This,
            /* [in] */ unsigned long inhno);
        
        END_INTERFACE
    } IDeviceVtbl;

    interface IDevice
    {
        CONST_VTBL struct IDeviceVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDevice_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDevice_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDevice_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDevice_IsValid(This,valid)	\
    (This)->lpVtbl -> IsValid(This,valid)

#define IDevice_SetDeviceName(This,devname)	\
    (This)->lpVtbl -> SetDeviceName(This,devname)

#define IDevice_Map(This,address,size)	\
    (This)->lpVtbl -> Map(This,address,size)

#define IDevice_Unmap(This,address)	\
    (This)->lpVtbl -> Unmap(This,address)

#define IDevice_RaiseInterrupt(This,inhno)	\
    (This)->lpVtbl -> RaiseInterrupt(This,inhno)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDevice_IsValid_Proxy( 
    IDevice __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *valid);


void __RPC_STUB IDevice_IsValid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDevice_SetDeviceName_Proxy( 
    IDevice __RPC_FAR * This,
    /* [in] */ BSTR devname);


void __RPC_STUB IDevice_SetDeviceName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDevice_Map_Proxy( 
    IDevice __RPC_FAR * This,
    /* [in] */ unsigned long address,
    /* [in] */ unsigned long size);


void __RPC_STUB IDevice_Map_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDevice_Unmap_Proxy( 
    IDevice __RPC_FAR * This,
    /* [in] */ unsigned long address);


void __RPC_STUB IDevice_Unmap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDevice_RaiseInterrupt_Proxy( 
    IDevice __RPC_FAR * This,
    /* [in] */ unsigned long inhno);


void __RPC_STUB IDevice_RaiseInterrupt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDevice_INTERFACE_DEFINED__ */


#ifndef ___IDeviceEvents_INTERFACE_DEFINED__
#define ___IDeviceEvents_INTERFACE_DEFINED__

/* interface _IDeviceEvents */
/* [object][helpstring][uuid] */ 


EXTERN_C const IID IID__IDeviceEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DA93137D-FB52-4421-B95D-9077340AD03B")
    _IDeviceEvents : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnRead( 
            /* [in] */ long address,
            /* [in] */ long sz,
            /* [length_is][size_is][out] */ byte __RPC_FAR data[  ]) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnWrite( 
            /* [in] */ long address,
            /* [in] */ long sz,
            /* [length_is][size_is][in] */ byte __RPC_FAR data[  ]) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnKernelStart( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnKernelExit( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct _IDeviceEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _IDeviceEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _IDeviceEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _IDeviceEvents __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnRead )( 
            _IDeviceEvents __RPC_FAR * This,
            /* [in] */ long address,
            /* [in] */ long sz,
            /* [length_is][size_is][out] */ byte __RPC_FAR data[  ]);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnWrite )( 
            _IDeviceEvents __RPC_FAR * This,
            /* [in] */ long address,
            /* [in] */ long sz,
            /* [length_is][size_is][in] */ byte __RPC_FAR data[  ]);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnKernelStart )( 
            _IDeviceEvents __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnKernelExit )( 
            _IDeviceEvents __RPC_FAR * This);
        
        END_INTERFACE
    } _IDeviceEventsVtbl;

    interface _IDeviceEvents
    {
        CONST_VTBL struct _IDeviceEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IDeviceEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IDeviceEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IDeviceEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IDeviceEvents_OnRead(This,address,sz,data)	\
    (This)->lpVtbl -> OnRead(This,address,sz,data)

#define _IDeviceEvents_OnWrite(This,address,sz,data)	\
    (This)->lpVtbl -> OnWrite(This,address,sz,data)

#define _IDeviceEvents_OnKernelStart(This)	\
    (This)->lpVtbl -> OnKernelStart(This)

#define _IDeviceEvents_OnKernelExit(This)	\
    (This)->lpVtbl -> OnKernelExit(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE _IDeviceEvents_OnRead_Proxy( 
    _IDeviceEvents __RPC_FAR * This,
    /* [in] */ long address,
    /* [in] */ long sz,
    /* [length_is][size_is][out] */ byte __RPC_FAR data[  ]);


void __RPC_STUB _IDeviceEvents_OnRead_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE _IDeviceEvents_OnWrite_Proxy( 
    _IDeviceEvents __RPC_FAR * This,
    /* [in] */ long address,
    /* [in] */ long sz,
    /* [length_is][size_is][in] */ byte __RPC_FAR data[  ]);


void __RPC_STUB _IDeviceEvents_OnWrite_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE _IDeviceEvents_OnKernelStart_Proxy( 
    _IDeviceEvents __RPC_FAR * This);


void __RPC_STUB _IDeviceEvents_OnKernelStart_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE _IDeviceEvents_OnKernelExit_Proxy( 
    _IDeviceEvents __RPC_FAR * This);


void __RPC_STUB _IDeviceEvents_OnKernelExit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* ___IDeviceEvents_INTERFACE_DEFINED__ */


#ifndef __IKernel_INTERFACE_DEFINED__
#define __IKernel_INTERFACE_DEFINED__

/* interface IKernel */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IKernel;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D3E42099-3FDD-4A78-BDBD-4E57D362F5ED")
    IKernel : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Read( 
            /* [in] */ unsigned long address,
            /* [in] */ unsigned long sz,
            /* [length_is][size_is][out] */ byte __RPC_FAR data[  ]) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsValid( 
            /* [out] */ BOOL __RPC_FAR *valid) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Write( 
            /* [in] */ unsigned long address,
            /* [in] */ unsigned long sz,
            /* [length_is][size_is][in] */ byte __RPC_FAR data[  ]) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IKernelVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IKernel __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IKernel __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IKernel __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Read )( 
            IKernel __RPC_FAR * This,
            /* [in] */ unsigned long address,
            /* [in] */ unsigned long sz,
            /* [length_is][size_is][out] */ byte __RPC_FAR data[  ]);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsValid )( 
            IKernel __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *valid);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Write )( 
            IKernel __RPC_FAR * This,
            /* [in] */ unsigned long address,
            /* [in] */ unsigned long sz,
            /* [length_is][size_is][in] */ byte __RPC_FAR data[  ]);
        
        END_INTERFACE
    } IKernelVtbl;

    interface IKernel
    {
        CONST_VTBL struct IKernelVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKernel_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IKernel_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IKernel_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IKernel_Read(This,address,sz,data)	\
    (This)->lpVtbl -> Read(This,address,sz,data)

#define IKernel_IsValid(This,valid)	\
    (This)->lpVtbl -> IsValid(This,valid)

#define IKernel_Write(This,address,sz,data)	\
    (This)->lpVtbl -> Write(This,address,sz,data)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IKernel_Read_Proxy( 
    IKernel __RPC_FAR * This,
    /* [in] */ unsigned long address,
    /* [in] */ unsigned long sz,
    /* [length_is][size_is][out] */ byte __RPC_FAR data[  ]);


void __RPC_STUB IKernel_Read_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IKernel_IsValid_Proxy( 
    IKernel __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *valid);


void __RPC_STUB IKernel_IsValid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IKernel_Write_Proxy( 
    IKernel __RPC_FAR * This,
    /* [in] */ unsigned long address,
    /* [in] */ unsigned long sz,
    /* [length_is][size_is][in] */ byte __RPC_FAR data[  ]);


void __RPC_STUB IKernel_Write_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IKernel_INTERFACE_DEFINED__ */


#ifndef ___IKernelEvents_INTERFACE_DEFINED__
#define ___IKernelEvents_INTERFACE_DEFINED__

/* interface _IKernelEvents */
/* [object][helpstring][uuid] */ 


EXTERN_C const IID IID__IKernelEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1353969D-E84F-463F-B211-337E9BCFB99E")
    _IKernelEvents : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnInterruptRequest( 
            /* [in] */ unsigned long inhno) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct _IKernelEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _IKernelEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _IKernelEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _IKernelEvents __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnInterruptRequest )( 
            _IKernelEvents __RPC_FAR * This,
            /* [in] */ unsigned long inhno);
        
        END_INTERFACE
    } _IKernelEventsVtbl;

    interface _IKernelEvents
    {
        CONST_VTBL struct _IKernelEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IKernelEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IKernelEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IKernelEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IKernelEvents_OnInterruptRequest(This,inhno)	\
    (This)->lpVtbl -> OnInterruptRequest(This,inhno)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE _IKernelEvents_OnInterruptRequest_Proxy( 
    _IKernelEvents __RPC_FAR * This,
    /* [in] */ unsigned long inhno);


void __RPC_STUB _IKernelEvents_OnInterruptRequest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* ___IKernelEvents_INTERFACE_DEFINED__ */


#ifndef ___IInformationEvents_INTERFACE_DEFINED__
#define ___IInformationEvents_INTERFACE_DEFINED__

/* interface _IInformationEvents */
/* [object][helpstring][uuid] */ 


EXTERN_C const IID IID__IInformationEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("11E125BE-FC60-4DC9-8393-DC393B556D06")
    _IInformationEvents : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnDeviceChanged( 
            short reason,
            long devid,
            long extra) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnKernelChanged( 
            short reason) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct _IInformationEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _IInformationEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _IInformationEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _IInformationEvents __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnDeviceChanged )( 
            _IInformationEvents __RPC_FAR * This,
            short reason,
            long devid,
            long extra);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnKernelChanged )( 
            _IInformationEvents __RPC_FAR * This,
            short reason);
        
        END_INTERFACE
    } _IInformationEventsVtbl;

    interface _IInformationEvents
    {
        CONST_VTBL struct _IInformationEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IInformationEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IInformationEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IInformationEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IInformationEvents_OnDeviceChanged(This,reason,devid,extra)	\
    (This)->lpVtbl -> OnDeviceChanged(This,reason,devid,extra)

#define _IInformationEvents_OnKernelChanged(This,reason)	\
    (This)->lpVtbl -> OnKernelChanged(This,reason)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE _IInformationEvents_OnDeviceChanged_Proxy( 
    _IInformationEvents __RPC_FAR * This,
    short reason,
    long devid,
    long extra);


void __RPC_STUB _IInformationEvents_OnDeviceChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE _IInformationEvents_OnKernelChanged_Proxy( 
    _IInformationEvents __RPC_FAR * This,
    short reason);


void __RPC_STUB _IInformationEvents_OnKernelChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* ___IInformationEvents_INTERFACE_DEFINED__ */



#ifndef __ATLDEVICEMANAGERLib_LIBRARY_DEFINED__
#define __ATLDEVICEMANAGERLib_LIBRARY_DEFINED__

/* library ATLDEVICEMANAGERLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ATLDEVICEMANAGERLib;

EXTERN_C const CLSID CLSID_Device;

#ifdef __cplusplus

class DECLSPEC_UUID("FEE2A68B-7F98-40E0-9853-183EE68BC7F8")
Device;
#endif

EXTERN_C const CLSID CLSID_Information;

#ifdef __cplusplus

class DECLSPEC_UUID("D7AAF617-008A-4961-BB51-041CFD490ED3")
Information;
#endif

EXTERN_C const CLSID CLSID_Kernel;

#ifdef __cplusplus

class DECLSPEC_UUID("51789667-9F20-40AF-AF7F-9856325DFB0B")
Kernel;
#endif
#endif /* __ATLDEVICEMANAGERLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
