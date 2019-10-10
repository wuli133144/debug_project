/******************************************************************************
* Copyright (c) 2008, Shandong Fisherman Information Technology Co., Ltd. 
* All rights reserved.
* �ļ�����: fm_cpc_pub.h
* �ļ�����: Fisherman �����豸(Crypto PCI Card, CPC)���������ӿ�
* ��ǰ�汾: 1.0
* ��    ��: 
* ����ʱ��: 2008-08-21 
* �޸ļ�¼: 
* ----------------------------------------------
*    ʱ��    ����     ����
******************************************************************************/
#ifndef FM_CPC_PUB_H
#define FM_CPC_PUB_H

#ifdef  __cplusplus
    extern "C" {
#endif

// open card dev type��now replace FM_DEV_TYPE_SJK0809��FM_DEV_TYPE_SJK0810
#define FM_DEV_TYPE_PCI_1_2X      0x0120    //pci  V1.2  if use FM_DEV_TYPE_SJK0810, also means use this
#define FM_DEV_TYPE_PCI_2_0X      0x0200    //pci  V2.0
#define FM_DEV_TYPE_PCIE_1_0X     0xE100    //pcie V1.0
#define FM_DEV_TYPE_PCIE_2_0X     0xE200    //pcie V2.0  if use FM_DEV_TYPE_SJK0809, also means use this

/************************ �û����� ************************/
// �û����
#define CPC_USER_ADMIN          1   /* ����Ա */
#define CPC_USER_OPER           2   /* ����Ա */
// �û���Ŀ
#define CPC_USER_MAXADMIN       5   /* ������Ա��Ŀ */
#define CPC_USER_MAXOPER        5   /* ������Ա��Ŀ */
#define CPC_USER_MAXUSER        (CPC_USER_MAXADMIN + CPC_USER_MAXOPER) /* ����û��� */
// �û�����
#define CPC_USER_ADDADMIN       1   /* ��ӹ���Ա */
#define CPC_USER_ADDOPER        2   /* ��Ӳ���Ա */
#define CPC_USER_DELADMIN       3   /* ɾ������Ա */
#define CPC_USER_DELOPER        4   /* ɾ������Ա */
// �û����ݹ���
#define CPC_USER_BAK            1   /* ���� */
#define CPC_USER_RES            2   /* �ָ� */
#define CPC_USER_BAKINIT        3   /* ���ݳ�ʼ�� */
#define CPC_USER_RESINIT        4   /* �ָ���ʼ�� */
// ��֤״̬
#define CPC_USER_AUTHPASS       1   /* ��֤ͨ�� */
#define CPC_USER_AUTHNOPASS     0   /* ��֤δͨ�� */

typedef struct tagUserInfoSt
{
    FM_U8 au8Serial[FM_DEVINFO_SERIAL_LEN];     // ���к�
    FM_U8 u8Flag;                               // ������� CPC_USER_ADMIN / CPC_USER_OPER */
    FM_U8 u8IsAuthed;                           // �Ƿ��¼     1�ѵ�¼
    FM_U8 u8Reserve1;                           // ����
    FM_U8 u8Reserve2;                           // ����
}CPC_USER_INFO, *PCPC_USER_INFO;

/****************************������Ҫ�Ĺ����ṹ********************************/
// ��������
#define CPC_BK_TYPE_ALL     1   // ������������
#define CPC_BK_TYPE_FILEALL 2   // �����ļ�ϵͳ �ݲ�֧��
#define CPC_BK_TYPE_FILE    3   // ���ݵ����ļ� �ݲ�֧��
#define CPC_BK_TYPE_WKALL   4   // �������й�����Կ
#define CPC_BK_TYPE_WK      5   // ���ݵ���������Կ
#define CPC_BK_TYPE_RSAALL  6   // ��������RSA��Կ
#define CPC_BK_TYPE_RSA     7   // ���ݵ���RSA��Կ

// ������Ϣ�洢���豸����
#define CPC_BK_STORE_HOST   1    // ������Ϣ�洢�������ļ�
#define CPC_BK_STORE_DEVKEY 2    // ������Ϣ�洢���豸������IC������USBKey �ݲ�֧��

// ���ݹ����ʼ��ʱ�����ýṹ
typedef struct stBkConifg
{
    FM_U32      u32Type;                // ��������
    FM_U32      u32StoreDev;            // ������Ϣ�洢���豸����
    FM_U8       u8StoreFile[128];       // ������Ϣ�洢���ļ���
    FM_U32      u32ThresholdKeyID;      // �豸������IC������USBKey�����ޱ��ݵ���ԿID [0-99]
    
    // u32TypeΪCPC_BK_TYPE_WK/CPC_BK_TYPE_RSAʱ��Ч
    // ���ݳ�ʼ��(CPC_BK_MNGOP_BAKINIT)ʱ���˲�����ʾ���ݵ���ԿID
    // �ָ���ʼ��(CPC_BK_MNGOP_RESINIT)ʱ���˲�����ʾ�ָ�����ָ����ԿID������ȫFʱ
    // ��ʾʹ�ñ���ʱ��ID��Ϊ�ָ�����ԿID
    FM_U32	    u32HKey;
    
    FM_U8       u8FullPath[128];    // u32TypeΪCPC_BK_TYPE_FILEʱ��Ч, ���ݵĵ����ļ���ȫ·��  
    FM_U32      u32Resvered[16];    // ����Ϊ0
} CPC_BK_CONFIG, *PCPC_BK_CONFIG;



/****************************** API �������� *********************************/
FM_RET FM_CPC_OpenDevice
(
    FM_I FM_U8     *pu8Id,
    FM_I FM_U32     u32Type,
    FM_I FM_U32     u32Flag,
    FM_O FM_HANDLE *phDev
);
FM_RET FM_CPC_CloseDevice
(
    FM_I FM_HANDLE  hDev
);
FM_RET FM_CPC_GetDeviceInfo
(
    FM_I FM_HANDLE  hDev,
    FM_O FM_DEV_INFO  *pDevInfo
); 
FM_RET FM_CPC_GenRandom
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Len,
    FM_O FM_U8     *pu8Random
);
FM_RET FM_CPC_GetInfo
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Flag,
    FM_I FM_U32     u32InLen,
    FM_I FM_U8     *pu8InBuf,
    FM_B FM_U32    *pu32Len,
    FM_O FM_U8     *pu8OutBuf
);
FM_RET FM_CPC_GetErrInfo
(
    FM_I FM_U32     u32LanFlag,
    FM_I FM_U32     u32ErrCode,
    FM_B FM_U32    *pu32Len,
    FM_O FM_S8     *ps8Info
);
FM_RET FM_CPC_GenRSAKeypair
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32KeyBits,
    FM_B FM_HKEY   *phKey,
    FM_O FM_RSA_PublicKey  *pPubkey, 
    FM_O FM_RSA_PrivateKey *pPrikey 
);
FM_RET FM_CPC_DelRSAKeypair
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_HKEY    hKey
);
FM_RET FM_CPC_ImportRSAKeypair
(
    FM_I FM_HANDLE  hDev,
    FM_B FM_HKEY   *phKey,
    FM_I FM_RSA_PublicKey  *pPubkey,
    FM_I FM_RSA_PrivateKey *pPrikey
);
FM_RET FM_CPC_ExportRSAKeypair
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_HKEY    hKey,
    FM_O FM_RSA_PublicKey  *pPubkey,
    FM_O FM_RSA_PrivateKey *pPrikey
);
FM_RET FM_CPC_RSAEncrypt
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_HKEY    hKey,
    FM_I FM_U8     *pu8InBuf,
    FM_I FM_U32     u32InLen,
    FM_O FM_U8     *pu8OutBuf,
    FM_O FM_U32    *pu32OutLen,
    FM_I FM_RSA_PublicKey *pPubkey
);
FM_RET FM_CPC_RSADecrypt
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_HKEY    hKey,
    FM_I FM_U8     *pu8InBuf,
    FM_I FM_U32     u32InLen,
    FM_O FM_U8     *pu8OutBuf,
    FM_O FM_U32    *pu32OutLen,
    FM_I FM_RSA_PrivateKey *pPrikey
);
FM_RET FM_CPC_RSASign
(
	FM_I	FM_HANDLE	hDev,
	FM_I	FM_HKEY		hKey,
	FM_I	FM_U32		u32Alg,
	FM_I	FM_U8		*pu8DataBuf,
	FM_I	FM_U32		u32DataLen,
	FM_O	FM_U8		*pu8SignBuf,
	FM_O 	FM_U32		*pu32SignLen,
	FM_I 	FM_RSA_PrivateKey  *pPrikey
);
FM_RET FM_CPC_RSAVerify
(
	FM_I	FM_HANDLE	hDev,
	FM_I	FM_HKEY		hKey,
	FM_I	FM_U32		u32Alg,
	FM_I	FM_U8		*pu8DataBuf,
	FM_I	FM_U32		u32DataLen,
	FM_I	FM_U8		*pu8SignBuf,
	FM_I	FM_U32		u32SignLen,
	FM_I	FM_RSA_PublicKey   *pPubkey
);
FM_RET FM_CPC_GenECCKeypair
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Alg,
    FM_B FM_HKEY   *phKey,
    FM_O FM_ECC_PublicKey  *pPubkey, 
    FM_O FM_ECC_PrivateKey *pPrikey 
);
FM_RET FM_CPC_DelECCKeypair
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_HKEY    hKey
);
FM_RET FM_CPC_ImportECCKeypair
(
    FM_I FM_HANDLE  hDev,
    FM_B FM_HKEY   *phKey,
    FM_I FM_ECC_PublicKey  *pPubkey,
    FM_I FM_ECC_PrivateKey *pPrikey
);
FM_RET FM_CPC_ExportECCKeypair
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_HKEY    hKey,
    FM_O FM_ECC_PublicKey  *pPubkey,
    FM_O FM_ECC_PrivateKey *pPrikey
);
FM_RET FM_CPC_ECCEncrypt
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Alg,
    FM_I FM_HKEY    hKey,
    FM_I FM_U8     *pu8InBuf,
    FM_I FM_U32     u32InLen,
    FM_I FM_ECC_PublicKey *pPubkey,
    FM_O FM_ECC_Cipher    *pECCCipher
);
FM_RET FM_CPC_ECCDecrypt
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Alg,
    FM_I FM_HKEY    hKey,
    FM_I FM_ECC_Cipher     *pECCCipher,
    FM_I FM_ECC_PrivateKey *pPrikey,
    FM_O FM_U8     *pu8OutBuf,
    FM_O FM_U32    *pu32OutLen
);
FM_RET FM_CPC_ECCSign
(
    FM_I FM_HANDLE             hDev,
    FM_I FM_U32                u32Alg,
    FM_I FM_HKEY               hKey,
    FM_I FM_U8                *pu8InBuf,
    FM_I FM_U32                u32InLen,
    FM_I FM_ECC_PrivateKey    *pPrikey,
    FM_O FM_ECC_Signature     *pSignature
);
FM_RET FM_CPC_ECCVerify
(
    FM_I FM_HANDLE         hDev,
    FM_I FM_U32            u32Alg,
    FM_I FM_HKEY           hKey,
    FM_I FM_ECC_PublicKey *pPubkey,
    FM_I FM_U8            *pu8InBuf,
    FM_I FM_U32            u32InLen,
    FM_I FM_ECC_Signature *pSignature
);
FM_RET FM_CPC_GenerateAgreementDataWithECC
(
    FM_I FM_HANDLE         hDev,
    FM_I FM_U32            u32Alg,
    FM_I FM_HKEY           hKey,
    FM_I FM_U32            u32KeyBits,
    FM_I FM_U8            *pu8SponsorID,
    FM_I FM_U32            u32SponsorIDLen,
    FM_O FM_ECC_PublicKey *pSponsorPubKey,
    FM_O FM_ECC_PublicKey *pSponsorTmpPubKey,
    FM_O FM_HANDLE        *phAgreementHandle
);
FM_RET FM_CPC_GenerateAgreementDataAndKeyWithECC
(
    FM_I FM_HANDLE         hDev,
    FM_I FM_U32            u32Alg,
    FM_I FM_HKEY           hKey,
    FM_I FM_U32            u32KeyBits,
    FM_I FM_U8            *pu8ResponseID,
    FM_I FM_U32            u32ResponseIDLen,
    FM_I FM_U8            *pu8SponsorID,
    FM_I FM_U32            u32SponsorIDLen,
    FM_I FM_ECC_PublicKey *pSponsorPubKey,
    FM_I FM_ECC_PublicKey *pSponsorTmpPubKey,
    FM_O FM_ECC_PublicKey *pResponsePubKey,
    FM_O FM_ECC_PublicKey *pResponseTmpPubKey,
    FM_O FM_HKEY          *phKeyHandle
);
FM_RET FM_CPC_GenerateKeyWithECC
(
    FM_I FM_HANDLE         hDev,
    FM_I FM_U32            u32Alg,
    FM_I FM_U8            *pu8ResponseID,
    FM_I FM_U32            u32ResponseIDLen,
    FM_I FM_ECC_PublicKey *pResponsePubKey,
    FM_I FM_ECC_PublicKey *pResponseTmpPubKey,
    FM_I FM_HANDLE        *phAgreementHandle,
    FM_O FM_HKEY          *phKeyHandle
);

FM_RET FM_CPC_GenKey
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Alg,
    FM_I FM_U32     u32InLen,
    FM_B FM_HKEY   *phKey,
    FM_O FM_U8     *pu8Key
);
FM_RET FM_CPC_DelKey
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_HKEY    hKey
);
FM_RET FM_CPC_ImportKey
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Alg,
    FM_I FM_U8     *pu8Key,
    FM_I FM_U32     u32KeyLen,
    FM_B FM_HKEY   *phKey
);
FM_RET FM_CPC_ExportKey
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_HKEY    hKey,
    FM_O FM_U8     *pu8Key,
    FM_B FM_U32    *pu32Len
);
FM_RET FM_CPC_EncryptInit
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_HKEY    hKey,
    FM_I FM_U32     u32Alg,
    FM_I FM_U32     u32WorkMode,
    FM_I FM_U8     *pu8Key,
    FM_I FM_U32     u32KeyLen,
    FM_I FM_U8     *pu8IV,
    FM_I FM_U32     u32IVLen
);
FM_RET FM_CPC_EncryptUpdate
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Alg,
    FM_I FM_U8     *pu8InBuf,
    FM_I FM_U32     u32InLen,
    FM_O FM_U8     *pu8OutBuf,
    FM_O FM_U32    *pu32OutLen
);
FM_RET FM_CPC_EncryptFinal
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Alg
);
FM_RET FM_CPC_Encrypt
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_HKEY    hKey,
    FM_I FM_U32     u32Alg,
    FM_I FM_U32     u32WorkMode,
    FM_I FM_U8     *pu8InBuf,
    FM_I FM_U32     u32InLen,
    FM_O FM_U8     *pu8OutBuf,
    FM_O FM_U32    *pu32OutLen,
    FM_I FM_U8     *pu8Key,
    FM_I FM_U32     u32KeyLen,
    FM_I FM_U8     *pu8IV,
    FM_I FM_U32     u32IVLen
);
FM_RET FM_CPC_DecryptInit
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_HKEY    hKey,
    FM_I FM_U32     u32Alg,
    FM_I FM_U32     u32WorkMode,
    FM_I FM_U8     *pu8Key,
    FM_I FM_U32     u32KeyLen,
    FM_I FM_U8     *pu8IV,
    FM_I FM_U32     u32IVLen
);
FM_RET FM_CPC_DecryptUpdate
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Alg,
    FM_I FM_U8     *pu8InBuf,
    FM_I FM_U32     u32InLen,
    FM_O FM_U8     *pu8OutBuf,
    FM_O FM_U32    *pu32OutLen
);
FM_RET FM_CPC_DecryptFinal
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Alg
);
FM_RET FM_CPC_Decrypt
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_HKEY    hKey,
    FM_I FM_U32     u32Alg,
    FM_I FM_U32     u32WorkMode,
    FM_I FM_U8     *pu8InBuf,
    FM_I FM_U32     u32InLen,
    FM_O FM_U8     *pu8OutBuf,
    FM_O FM_U32    *pu32OutLen,
    FM_I FM_U8     *pu8Key,
    FM_I FM_U32     u32KeyLen,
    FM_I FM_U8     *pu8IV,
    FM_I FM_U32     u32IVLen
);
#ifdef FMCFG_OS_LINUX
FM_RET FM_CPC_EncryptEx
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_HKEY    hKey,
    FM_I FM_U32     u32Alg,
    FM_I FM_U32     u32WorkMode,
    FM_I FM_U8     *pu8InBuf,
    FM_I FM_U32     u32InLen,
    FM_O FM_U8     *pu8OutBuf,
    FM_O FM_U32    *pu32OutLen,
    FM_I FM_U8     *pu8Key,
    FM_I FM_U32     u32KeyLen,
    FM_I FM_U8     *pu8IV,
    FM_I FM_U32     u32IVLen
);
FM_RET FM_CPC_DecryptEx
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_HKEY    hKey,
    FM_I FM_U32     u32Alg,
    FM_I FM_U32     u32WorkMode,
    FM_I FM_U8     *pu8InBuf,
    FM_I FM_U32     u32InLen,
    FM_O FM_U8     *pu8OutBuf,
    FM_O FM_U32    *pu32OutLen,
    FM_I FM_U8     *pu8Key,
    FM_I FM_U32     u32KeyLen,
    FM_I FM_U8     *pu8IV,
    FM_I FM_U32     u32IVLen
);
#endif
FM_RET FM_CPC_HashInit
(
    FM_I FM_HANDLE         hDev,
    FM_I FM_U32            u32Alg   
);
FM_RET FM_CPC_HashUpdate
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Alg,
    FM_I FM_U8     *pu8InBuf,
    FM_I FM_U32     u32InLen
);
FM_RET FM_CPC_HashFinal
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Alg,
    FM_O FM_U8     *pu8OutMACBuf,
    FM_B FM_U32    *pu32OutMACLen
);
#ifdef FMCFG_OS_LINUX
FM_RET FM_CPC_HashInitEx
(
    FM_I FM_HANDLE              hDev, 
    FM_I FM_U32                 u32Alg,
    FM_O PFM_CPC_SHA1_CTX       pSHACtx
 );
FM_RET FM_CPC_HashUpdateEx
(
    FM_I FM_HANDLE              hDev,
    FM_I FM_U32                 u32Alg,
    FM_I FM_U8                 *pu8InBuf,
    FM_I FM_U32                 u32InLen,
    FM_B PFM_CPC_SHA1_CTX       pSHACtx
);
FM_RET FM_CPC_HashFinalEx
(
    FM_I FM_HANDLE              hDev,
    FM_I FM_U32                 u32Alg,
    FM_O FM_U8                 *pu8HashBuf,
    FM_B FM_U32                *pu32HashLen,
    FM_B PFM_CPC_SHA1_CTX       pSHACtx
);
#endif
FM_U32 FM_CPC_SM3Init
(
    FM_I FM_HANDLE         hDev,
    FM_I FM_ECC_PublicKey *pPubkey,
    FM_I FM_U8            *pu8ID,
    FM_I FM_U32            u32IDLen
);
FM_U32 FM_CPC_SM3Update
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U8     *pu8InBuf,
    FM_I FM_U32     u32InLen
);
FM_U32 FM_CPC_SM3Final
(
    FM_I FM_HANDLE  hDev,
    FM_O FM_U8     *pu8HashBuf,
    FM_B FM_U32    *pu32HashLen
);
#ifdef FMCFG_OS_LINUX
FM_RET FM_CPC_SM3InitEx
(
    FM_I FM_HANDLE               hDev,
    FM_I FM_ECC_PublicKey       *pPubkey,
    FM_I FM_U8                  *pu8ID,
    FM_I FM_U32                  u32IDLen,
    FM_O PFM_CPC_SM3_CTX         pSM3Ctx
);
FM_RET FM_CPC_SM3UpdateEx
(
    FM_I FM_HANDLE              hDev,
    FM_I FM_U8                 *pu8InBuf,
    FM_I FM_U32                 u32InLen,
    FM_B PFM_CPC_SM3_CTX        pSM3Ctx
);
FM_RET FM_CPC_SM3FinalEx
(
    FM_I FM_HANDLE              hDev,
    FM_O FM_U8                 *pu8HashBuf,
    FM_O FM_U32                *pu32HashLen,
    FM_I PFM_CPC_SM3_CTX        pSM3Ctx
);
#endif
FM_RET FM_CPC_USER_Login
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Type,
    FM_I FM_U8     *pu8PinBuf,
    FM_I FM_U32     u32PinLen,
    FM_O FM_HUSER  *phUser,
    FM_O FM_U32    *pu32RetryNum
);
FM_RET FM_CPC_USER_Logout
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_HUSER   hUser
);
FM_RET FM_CPC_USER_ChangePin
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Flag,
    FM_I FM_U8     *pu8OldPinBuf,
    FM_I FM_U32     u32OldPinLen,
    FM_I FM_U8     *pu8NewPinBuf,
    FM_I FM_U32     u32NewPinLen,
    FM_O FM_U32    *pu32RetryNum
);
FM_RET FM_CPC_USER_GetInfo
(
    FM_I FM_HANDLE  hDev,
    FM_O CPC_USER_INFO aUserInfo[CPC_USER_MAXUSER]
);
FM_RET FM_CPC_USER_UserMng
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Flag,
    FM_I FM_U8     *pu8Buf,
    FM_I FM_U32     u32Len
);
FM_RET FM_CPC_USER_BackupMng
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Flag,
    FM_I CPC_BK_CONFIG  *pstBkConfig,
    FM_O FM_U32    *pu32UserAllNum,
    FM_O FM_U32    *pu32UserDealNum
);

FM_RET FM_CPC_FILE_Init
(
    FM_I FM_HANDLE  hDev
);
FM_RET FM_CPC_FILE_CreateDir
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_S8     *ps8FullDir,
    FM_I FM_U32     u32AccCond
);
FM_RET FM_CPC_FILE_DeleteDir
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_S8     *ps8FullDir
);
FM_RET FM_CPC_FILE_CreateFile
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_S8     *ps8DirName,
    FM_I FM_S8     *ps8FileName,
    FM_I FM_U32     u32FileSize,
    FM_I FM_U32     u32AccCond
);
FM_RET FM_CPC_FILE_ReadFile
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_S8     *ps8DirName,
    FM_I FM_S8     *ps8FileName,
    FM_I FM_U32     u32Offset,
    FM_I FM_U32     u32Size,
    FM_O FM_U8     *pu8Buf
);
FM_RET FM_CPC_FILE_WriteFile
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_S8     *ps8DirName,
    FM_I FM_S8     *ps8FileName,
    FM_I FM_U32     u32Offset,
    FM_I FM_U32     u32Size,
    FM_I FM_U8     *pu8Buf
);
FM_RET FM_CPC_FILE_DeleteFile
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_S8     *ps8DirName,
    FM_I FM_S8     *ps8FileName
);
FM_RET FM_CPC_FILE_EnmuDir
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_S8     *ps8DirName,
    FM_B FM_U32    *pu32BufLen,
    FM_O FM_U8     *pu8Buf,
    FM_O FM_U32    *pu32DirNum
);
FM_RET FM_CPC_FILE_EnmuFile
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_S8     *ps8DirName,
    FM_B FM_U32    *pu32BufLen,
    FM_O FM_U8     *pu8Buf,
    FM_O FM_U32    *pu32FileNum
);
FM_RET FM_CPC_Cmd
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U32     u32Cmd, 
    FM_I FM_U32     aPara[],
    FM_I FM_U8     *pu8InData,
    FM_I FM_U32     u32InLen, 
    FM_O FM_U8     *pu8OutData, 
    FM_B FM_U32    *pu32OutLen
);
//�����豸����Կ
FM_RET CPC_Master_GenKey
(
    FM_I FM_HANDLE  hDev
);
//�����豸����Կ
FM_RET CPC_Master_ImportKey
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U8     *pu8Key,
    FM_I FM_U32     u32KeyLen
);
//�����豸����Կ
FM_RET CPC_Master_ExportKey
(
    FM_I FM_HANDLE  hDev,
    FM_O FM_U8     *pu8Key,
    FM_O FM_U32    *pu32Len
);
//����Կ���м���
FM_RET CPC_Master_Encrypt
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U8     *pu8InBuf,
    FM_I FM_U32     u32InLen,
    FM_O FM_U8     *pu8OutBuf,
    FM_O FM_U32    *pu32OutLen
);
//����Կ���н���
FM_RET CPC_Master_Decrypt
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U8     *pu8InBuf,
    FM_I FM_U32     u32InLen,
    FM_O FM_U8     *pu8OutBuf,
    FM_O FM_U32    *pu32OutLen
);
//key����Կ���м���
FM_RET CPC_Usbkey_Encrypt
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U8     *pu8InBuf,
    FM_I FM_U32     u32InLen,
    FM_O FM_U8     *pu8OutBuf,
    FM_O FM_U32    *pu32OutLen
);
//key����Կ���н���
FM_RET CPC_Usbkey_Decrypt
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U8     *pu8InBuf,
    FM_I FM_U32     u32InLen,
    FM_O FM_U8     *pu8OutBuf,
    FM_O FM_U32    *pu32OutLen
);
FM_RET FM_CPC_USER_Right
(
    FM_I FM_HANDLE  hDev,
    FM_I FM_U8  *pu8serial,
    FM_I FM_U32  u32Len
);
FM_RET FM_CPC_ContainerWrite
(
	FM_I FM_HANDLE  hDev,
	FM_I FM_U32  	u32Flag,
	FM_I FM_U8  	*pContainerName,
	FM_I FM_U8  	*pCert,
	FM_I FM_U32  	u32CertLen
);
FM_RET FM_CPC_ContainerRead
(
	FM_I FM_HANDLE  hDev,
	FM_I FM_U32  	u32Flag,	
	FM_I FM_U8  	*pContainerName,
	FM_O FM_U8  	*pValue,
	FM_O FM_U32  	*pu32ValueLen
);
FM_RET FM_CPC_ContainerDelete
(
	FM_I FM_HANDLE  hDev,
	FM_I FM_U8  	*pContainerName
);
FM_RET FM_CPC_ContainerEnum
(
	FM_I FM_HANDLE  hDev,
	FM_O FM_U8  	*pContainerNames,
	FM_O FM_U32  	*pu32ContainerCount
);

#ifdef  __cplusplus
    }
#endif  /* #ifdef  __cplusplus */

#endif  /* #ifndef FM_CPC_PUB_H */

