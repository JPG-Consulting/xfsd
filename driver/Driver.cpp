/************************************************************************
* �ļ�����:Driver.cpp                                                 
* NT driver�����ṹ������driverʵ�壬��ʼ��������ж�غ�����
*************************************************************************/

#include "Driver.h"
#include "tslib/read_file.h"

/************************************************************************
* ��������:DriverEntry
* ��������:��ʼ���������򣬶�λ������Ӳ����Դ�������ں˶���
* �����б�:
      pDriverObject:��I/O�������д���������������
      pRegistryPath:����������ע�����е�·��
* ���� ֵ:���س�ʼ������״̬
*************************************************************************/
#pragma INITCODE
extern "C" NTSTATUS DriverEntry (
			IN PDRIVER_OBJECT pDriverObject,
			IN PUNICODE_STRING pRegistryPath	) 
{
	NTSTATUS status;
	KdPrint(("Enter DriverEntry\n"));

	//ע�������������ú������
	pDriverObject->DriverUnload = HelloDDKUnload;
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_QUERY_INFORMATION]       =HelloDDKDispatchRoutine;
    pDriverObject->MajorFunction[IRP_MJ_SET_INFORMATION]         =HelloDDKDispatchRoutine;
    pDriverObject->MajorFunction[IRP_MJ_QUERY_VOLUME_INFORMATION]=HelloDDKDispatchRoutine;
    pDriverObject->MajorFunction[IRP_MJ_DIRECTORY_CONTROL]       =HelloDDKDispatchRoutine;
    pDriverObject->MajorFunction[IRP_MJ_FILE_SYSTEM_CONTROL]     =HelloDDKDispatchRoutine;
    pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]          =HelloDDKDispatchRoutine;
    pDriverObject->MajorFunction[IRP_MJ_LOCK_CONTROL]            =HelloDDKDispatchRoutine;
    pDriverObject->MajorFunction[IRP_MJ_CLEANUP]                 =HelloDDKDispatchRoutine;
    pDriverObject->MajorFunction[IRP_MJ_PNP]                     =HelloDDKDispatchRoutine;
	//read������
	pDriverObject->MajorFunction[IRP_MJ_READ] = ReadRoutine;
	//write����
    pDriverObject->MajorFunction[IRP_MJ_WRITE] = WriteRoutine;

	//���������豸����
	status = CreateDevice(pDriverObject);

	KdPrint(("DriverEntry end\n"));
	return status;
}

/************************************************************************
* ��������:CreateDevice
* ��������:��ʼ���豸����
* �����б�:
      pDriverObject:��I/O�������д���������������
* ���� ֵ:���س�ʼ��״̬
*************************************************************************/
#pragma INITCODE
NTSTATUS CreateDevice (
		IN PDRIVER_OBJECT	pDriverObject) 
{
	NTSTATUS status;
	PDEVICE_OBJECT pDevObj;
	PDEVICE_EXTENSION pDevExt;
	
	//�����豸����
	UNICODE_STRING devName;
	RtlInitUnicodeString(&devName,L"\\Device\\XFSDevice");
	
	//�����豸
	status = IoCreateDevice( pDriverObject,
						sizeof(DEVICE_EXTENSION),
						&(UNICODE_STRING)devName,
						FILE_DEVICE_FILE_SYSTEM,//������������ͣ���������
						0, TRUE,
						&pDevObj );
	//�ж��豸�Ƿ�ɹ�
	if (!NT_SUCCESS(status))
		return status;
	//�豸����Ϊ��������д�豸
	pDevObj->Flags |= DO_BUFFERED_IO;
	//�õ��豸��չ
	pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	//�豸��չ���豸����
	pDevExt->pDevice = pDevObj;
	//�豸��չ�е��豸����
	pDevExt->ustrDeviceName = devName;
	//������������
	UNICODE_STRING symLinkName;
	RtlInitUnicodeString(&symLinkName,L"\\??\\XFSD");
	pDevExt->ustrSymLinkName = symLinkName;
	status = IoCreateSymbolicLink( &symLinkName,&devName );
	//�жϷ��������Ƿ�ɹ�
	if (!NT_SUCCESS(status)) 
	{
		IoDeleteDevice( pDevObj );
		return status;
	}
	return STATUS_SUCCESS;
}

/************************************************************************
* ��������:HelloDDKUnload
* ��������:�������������ж�ز���
* �����б�:
      pDriverObject:��������
* ���� ֵ:����״̬
*************************************************************************/
#pragma PAGEDCODE
VOID HelloDDKUnload (IN PDRIVER_OBJECT pDriverObject) 
{
	PDEVICE_OBJECT	pNextObj;
	KdPrint(("Enter DriverUnload\n"));
	//�õ���һ���豸
	pNextObj = pDriverObject->DeviceObject;
	//ö�������豸����
	while (pNextObj != NULL) 
	{
		//�õ��豸��չ
		PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)
			pNextObj->DeviceExtension;

		//ɾ����������
		UNICODE_STRING pLinkName = pDevExt->ustrSymLinkName;
		IoDeleteSymbolicLink(&pLinkName);
		pNextObj = pNextObj->NextDevice;
		//ɾ���豸
		IoDeleteDevice( pDevExt->pDevice );
	}
}

/************************************************************************
* ��������:HelloDDKDispatchRoutine
* ��������:������IRP����ֱ�ӷ��ش���
* �����б�:
      pDevObj:�����豸����
      pIrp:��IO�����
* ���� ֵ:����״̬
*************************************************************************/
#pragma PAGEDCODE
NTSTATUS HelloDDKDispatchRoutine(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp) 
{
	KdPrint(("No such fuction yet\n"));
	NTSTATUS status = STATUS_SUCCESS;
	// ���IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;	// bytes xfered
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
	KdPrint(("Leave HelloDDKDispatchRoutine\n"));
	return status;
}

/************************************************************************
* ��������:WriteRoutine
* ��������:������IRP����ֱ�ӷ��ش���
* �����б�:
      pDevObj:�����豸����
      pIrp:��IO�����
* ���� ֵ:����״̬
*************************************************************************/
#pragma PAGEDCODE
NTSTATUS WriteRoutine(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp) 
{
	KdPrint(("No such fuction yet\n"));
	NTSTATUS status = STATUS_SUCCESS;
	// ���IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;	// bytes xfered
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
	KdPrint(("Leave HelloDDKDispatchRoutine\n"));
	return status;
}

/************************************************************************
* ��������:ReadRoutine
* ��������:�Զ�IRP���д���
* �����б�:
      pDevObj:�����豸����
      pIrp:��IO�����
* ���� ֵ:����״̬
*************************************************************************/
#pragma PAGEDCODE
NTSTATUS ReadRoutine(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp) 
{
	KdPrint(("ReadRoutine on.\n"));
	//��һ��IRP�ļ򵥲������������ܶ�IRP�����ӵĲ���

	PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;

	NTSTATUS status = STATUS_SUCCESS;

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	ULONG ulReadLength = stack->Parameters.Read.Length;
	ULONG ulReadOffset = (ULONG)stack->Parameters.Read.ByteOffset.QuadPart;

char buff2[10240];
char *addr;
	//���ļ���
   init_read_file_from_disk();
   long b=read_file_from_disk( addr,buff2,sizeof(buff2));
	
		//�����ݴ洢��AssociatedIrp.SystemBuffer���Ա�Ӧ�ó���ʹ��
		memcpy(pIrp->AssociatedIrp.SystemBuffer,buff2,ulReadLength);
		status = STATUS_SUCCESS;
	
	
	// ���IRP
	pIrp->IoStatus.Status = status;
	//����IRP�����˶����ֽ�
	pIrp->IoStatus.Information = ulReadLength;	// bytes xfered

	

	//����IRP
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );

	KdPrint(("Leave HelloDDKRead\n"));

	return status;

}



