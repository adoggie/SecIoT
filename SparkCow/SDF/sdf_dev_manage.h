#ifndef __SDF_DEV_MANAGE_H__
#define __SDF_DEV_MANAGE_H__

#include "base_type.h"
#include "sdf_type.h"

#ifdef __cplusplus
extern "C" {
#endif

int SDF_CreateKeyPair_ECC(void *hSessionHandle, unsigned int uiSignFlag, unsigned int uiKeyIndex, ECCrefPublicKey *pucPublicKey);
int SDF_ImportKeyPair_ECC(void *hSessionHandle, unsigned int uiSignFlag, unsigned int uiKeyIndex,  ECCrefPublicKey *pucPublicKey, ECCrefPrivateKey *pucPrivateKey);
int SDF_CreateKeyPair_RSA(void *hSessionHandle, unsigned int uiSignFlag, unsigned int uiKeyIndex, unsigned int uiBitLen, RSArefPublicKey *pucPublicKey);
int SDF_ImportKeyPair_RSA(void *hSessionHandle, unsigned int uiSignFlag, unsigned int uiKeyIndex, RSArefPrivateKey *pucPrivateKey);
int SDF_CreateKEK(void *hSessionHandle, unsigned int uiKeyIndex, unsigned int uiKeyBits);
int SDF_ImportKEK(void *hSessionHandle, unsigned int uiKeyIndex, unsigned char *pucKey, unsigned int uiKeyBits);
int SDF_GetPINInfo(void *hSessionHandle, unsigned int uiKeyIndex, unsigned int uiPINType, 
						unsigned int *puiMaxRetryCount, unsigned int *puiRemainRetryCount);
int SDF_ChangePIN(void *hSessionHandle, unsigned int uiKeyIndex, unsigned int uiPINType, 
						char *OldPIN, char *NewPIN, unsigned int *puiRetry);
int SDF_UnlockPIN(void *hSessionHandle, unsigned int uiKeyIndex, char *AdminPIN, char *NewUserPIN, unsigned int *puiRetry);
/**
 * Initialize the file system and set root key & admin pin & user pin
 * @arg hSessionHandle		A pointer to the handle of session
 * @arg AdminPin			A pointer to the admin pin and be verified in cos(if no file system initialized in cos, not be verified)
 * @arg pucRootKey			A pointer to the rootkey to be set, if NULL create rootkey using random data in cos
 * @arg uiKeyBits			the bits size of rootkey
 * @arg NewAdminPin		A pointer to the new admin pin
 * @arg NewUserPIN			A pointer to the nw user pin
 *
 * Initialize root key, admin pin, user pin, and file system.
 *
 * @return error code or 0.
 */
int SDF_InitKeyFileSystem(void *hSessionHandle, char *AdminPin, unsigned char *pucRootKey, unsigned int uiKeyBits, char *NewAdminPin, char*NewUserPIN);
int SDF_RestoreFactorySetting(void *hSessionHandle, char *AdminPin);
//int SDF_KeyDistributionPrepare(void *hSessionHandle, char *AdminPin, unsigned char ucStoreKeyNum, unsigned char ucRestoreKeyNum);
//int SDF_KeyDistributionStore(void *hSessionHandle, char *AdminPin, unsigned char ucStoreKeyIndex);
//int SDF_KeyDistributionRestore(void *hSessionHandle, char *AdminPin, unsigned char ucRestoreKeyIndex);

#ifdef __cplusplus
};
#endif

#endif  /*__SDF_DEV_MANAGE_H__*/
