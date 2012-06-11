//�豸����
//��Ҫ�޸ģ�����������ԭ�ӳ������cdfs��ص����ݽṹ�޸ĳ���ص�xfs���ݽṹ
//������ͣ�������

#include "XProcs.h"

//
//  The Bug check file id for this module
//

#define BugCheckFileId                   (XFS_BUG_CHECK_DEVCTRL)

//
//  Local support routines
//

NTSTATUS
XDevCtrlCompletionRoutine (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp,
    IN PVOID Contxt
    );

#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, XCommonDevControl)
#endif


NTSTATUS
XCommonDevControl (
    IN PIRP_CONTEXT IrpContext,
    IN PIRP Irp
    )



{
    NTSTATUS Status;

    TYPE_OF_OPEN TypeOfOpen;
// ��Ҫ�޸ĳ�xfs��ص����ݽṹ������������
    PFCB Fcb;
    PCCB Ccb;

    PIO_STACK_LOCATION IrpSp;
    PIO_STACK_LOCATION NextIrpSp;

    PVOID TargetBuffer = NULL;

    PAGED_CODE();

    //
    //  Extract and decode the file object.
    //

    IrpSp = IoGetCurrentIrpStackLocation( Irp );

    TypeOfOpen = XDecodeFileObject( IrpContext,
                                     IrpSp->FileObject,
                                     &Fcb,
                                     &Ccb );

    //
    //  The only type of opens we accept are user volume opens.
    // ����ֻ�����û����

    if (TypeOfOpen != UserVolumeOpen) {

        XCompleteRequest( IrpContext, Irp, STATUS_INVALID_PARAMETER );
        return STATUS_INVALID_PARAMETER;
    }

    if (IrpSp->Parameters.DeviceIoControl.IoControlCode == IOCTL_XROM_READ_TOC) {

        //
        // �����������������������޸ģ�������������
        //  ��ʵ������ݽṹ�������Ƿ�ı�

        XVerifyVcb( IrpContext, Fcb->Vcb );

    //
    //  Handle the case of the disk type ourselves.
    //

    } else if (IrpSp->Parameters.DeviceIoControl.IoControlCode == IOCTL_XROM_DISK_TYPE) {

        //
        // �����������������������޸ģ�������������
        //  ��ʵ������ݽṹ�������Ƿ�ı�


        XVerifyVcb( IrpContext, Fcb->Vcb );

        //
        //  Check the size of the output buffer.
        //

        if (IrpSp->Parameters.DeviceIoControl.OutputBufferLength < sizeof( XROM_DISK_DATA )) {

            XCompleteRequest( IrpContext, Irp, STATUS_BUFFER_TOO_SMALL );
            return STATUS_BUFFER_TOO_SMALL;
        }

        //
        //  ��������ݽṹ�ϸ�����Ϣ
        //

        ((PXROM_DISK_DATA) Irp->AssociatedIrp.SystemBuffer)->DiskData = Fcb->Vcb->DiskFlags;

        Irp->IoStatus.Information = sizeof( XROM_DISK_DATA );
        XCompleteRequest( IrpContext, Irp, STATUS_SUCCESS );
        return STATUS_SUCCESS;
    }

    //
    //  �õ�ջλ�ã�����ջԪ����Ϣ
    //

    NextIrpSp = IoGetNextIrpStackLocation( Irp );

    *NextIrpSp = *IrpSp;

    //
    //  Set up the completion routine
    //

    IoSetCompletionRoutine( Irp,
                            XDevCtrlCompletionRoutine,
                            NULL,
                            TRUE,
                            TRUE,
                            TRUE );

    //
    //  Send the request.
    //

    Status = IoCallDriver( IrpContext->Vcb->TargetDeviceObject, Irp );

    //
    //  ����IRP�����������Ѿ����IRP
    //

    XCompleteRequest( IrpContext, NULL, STATUS_SUCCESS );

    return Status;
}


//
//  Local support routine
//

NTSTATUS
XDevCtrlCompletionRoutine (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp,
    IN PVOID Contxt
    )

{
    //
    //  Add the hack-o-ramma to fix formats.
    //

    if (Irp->PendingReturned) {

        IoMarkIrpPending( Irp );
    }

    return STATUS_SUCCESS;

    UNREFERENCED_PARAMETER( DeviceObject );
    UNREFERENCED_PARAMETER( Contxt );
}


