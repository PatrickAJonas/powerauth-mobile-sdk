/*
 * Copyright 2016-2017 Lime - HighTech Solutions s.r.o.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cc7tests/CC7Tests.h>
#include "protocol/ProtocolUtils.h"
#include "protocol/Constants.h"
#include "crypto/CryptoUtils.h"

using namespace cc7;
using namespace cc7::tests;
using namespace io::getlime::powerAuth;

namespace io
{
namespace getlime
{
namespace powerAuthTests
{
	class pa2ProtocolUtilsTests : public UnitTest
	{
	public:
		
		pa2ProtocolUtilsTests()
		{
			CC7_REGISTER_TEST_METHOD(testValidateUnlockKeys)
			CC7_REGISTER_TEST_METHOD(testValidateUnlockKeysNegative)
			CC7_REGISTER_TEST_METHOD(testLockUnlockSignatureKeys)
		}
		
		// unit tests
		
		void testValidateUnlockKeys()
		{
			// Positive scenarios
			{
				SignatureUnlockKeys unlock;
				unlock.possessionUnlockKey = crypto::GetRandomData(16);
				ccstAssertTrue(protocol::ValidateUnlockKeys(unlock, nullptr, SF_Possession));
			}
			{
				SignatureUnlockKeys unlock;
				unlock.userPassword        = cc7::MakeRange("Hello world");
				ccstAssertTrue(protocol::ValidateUnlockKeys(unlock, nullptr, SF_Knowledge));
			}
			{
				SignatureUnlockKeys unlock;
				unlock.biometryUnlockKey   = crypto::GetRandomData(16);
				ccstAssertTrue(protocol::ValidateUnlockKeys(unlock, nullptr, SF_Biometry));
			}
			{
				SignatureUnlockKeys unlock;
				unlock.possessionUnlockKey = crypto::GetRandomData(16);
				unlock.biometryUnlockKey   = crypto::GetRandomData(16);
				ccstAssertTrue(protocol::ValidateUnlockKeys(unlock, nullptr, SF_Possession_Biometry));
			}
			{
				SignatureUnlockKeys unlock;
				unlock.possessionUnlockKey = crypto::GetRandomData(16);
				unlock.userPassword        = cc7::MakeRange("Hello world");
				ccstAssertTrue(protocol::ValidateUnlockKeys(unlock, nullptr, SF_Possession_Knowledge));
			}
			{
				SignatureUnlockKeys unlock;
				unlock.possessionUnlockKey = crypto::GetRandomData(16);
				unlock.userPassword        = cc7::MakeRange("Hello world");
				unlock.biometryUnlockKey   = crypto::GetRandomData(16);
				ccstAssertTrue(protocol::ValidateUnlockKeys(unlock, nullptr, SF_Possession_Knowledge_Biometry));
			}
			{
				SignatureUnlockKeys unlock;
				unlock.possessionUnlockKey = crypto::GetRandomData(16);
				ccstAssertTrue(protocol::ValidateUnlockKeys(unlock, nullptr, protocol::SF_Transport));
			}
			{
				SignatureUnlockKeys unlock;
				unlock.possessionUnlockKey = crypto::GetRandomData(16);
				unlock.userPassword        = cc7::MakeRange("Hello world");
				ccstAssertTrue(protocol::ValidateUnlockKeys(unlock, nullptr, protocol::SF_FirstLock));
			}
			{
				SignatureUnlockKeys unlock;
				unlock.possessionUnlockKey = crypto::GetRandomData(16);
				unlock.userPassword        = cc7::MakeRange("Hello world");
				unlock.biometryUnlockKey   = crypto::GetRandomData(16);
				ccstAssertTrue(protocol::ValidateUnlockKeys(unlock, nullptr, protocol::SF_FirstLock));
			}
		}
		
		void testValidateUnlockKeysNegative()
		{
			{
				SignatureUnlockKeys unlock;
				ccstAssertFalse(protocol::ValidateUnlockKeys(unlock, nullptr, SF_Possession));
			}
			{
				SignatureUnlockKeys unlock;
				ccstAssertFalse(protocol::ValidateUnlockKeys(unlock, nullptr, SF_Knowledge));
				unlock.userPassword = cc7::MakeRange("1");
				ccstAssertFalse(protocol::ValidateUnlockKeys(unlock, nullptr, SF_Knowledge));
				unlock.userPassword = cc7::MakeRange("12");
				ccstAssertFalse(protocol::ValidateUnlockKeys(unlock, nullptr, SF_Knowledge));
				unlock.userPassword = cc7::MakeRange("123");
				ccstAssertFalse(protocol::ValidateUnlockKeys(unlock, nullptr, SF_Knowledge));
				unlock.userPassword = cc7::MakeRange("1234");
				ccstAssertTrue(protocol::ValidateUnlockKeys(unlock, nullptr, SF_Knowledge));
			}
			{
				SignatureUnlockKeys unlock;
				ccstAssertFalse(protocol::ValidateUnlockKeys(unlock, nullptr, SF_Biometry));
			}
			{
				SignatureUnlockKeys unlock;
				unlock.possessionUnlockKey = cc7::ByteArray(16, 0);	// zero
				ccstAssertFalse(protocol::ValidateUnlockKeys(unlock, nullptr, SF_Knowledge));
				ccstAssertFalse(protocol::ValidateUnlockKeys(unlock, nullptr, protocol::SF_Transport));
			}
			{
				SignatureUnlockKeys unlock;
				unlock.biometryUnlockKey = cc7::ByteArray(16, 0);	// zero
				ccstAssertFalse(protocol::ValidateUnlockKeys(unlock, nullptr, SF_Biometry));
			}
		}
		
		void testLockUnlockSignatureKeys()
		{
			const cc7::ByteArray biometryKey   = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
			const cc7::ByteArray possessionKey = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,  1,  1,  1,  1,  1,  1  };
			const cc7::ByteArray knowledgeKey  = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5,  6,  6,  7,  7,  8,  8  };
			const cc7::ByteArray transportKey  = { 9, 9, 9, 9, 9, 9, 9, 9, 9, 8,  8,  8,  8,  8,  8,  8  };
			
			const cc7::ByteArray EEK           = crypto::GetRandomData(16);
			const cc7::ByteArray biometryPK    = crypto::GetRandomData(16);
			const cc7::ByteArray possessionPK  = crypto::GetRandomData(16);
			
			const cc7::ByteArray knowledgePass = cc7::MakeRange("SuperSecret");
			const cc7::ByteArray knowledgeSalt = crypto::GetRandomData(16);
			const cc7::U32 knowledgeIterations = protocol::PBKDF2_PASS_ITERATIONS;
			
			protocol::SignatureKeys secret_no_eek, secret_with_eek;
			// Lock without EEK
			{
				SignatureFactor factor = protocol::SF_FirstLock;
				protocol::SignatureKeys plain;
				plain.biometryKey = biometryKey;
				plain.possessionKey = possessionKey;
				plain.transportKey = transportKey;
				plain.knowledgeKey = knowledgeKey;
				SignatureUnlockKeys lockKeys;
				lockKeys.biometryUnlockKey = biometryPK;
				lockKeys.possessionUnlockKey = possessionKey;
				lockKeys.userPassword = knowledgePass;
				protocol::SignatureUnlockKeysReq lockRequest(factor, &lockKeys, nullptr, &knowledgeSalt, knowledgeIterations);
				ccstAssertTrue(protocol::LockSignatureKeys(secret_no_eek, plain, lockRequest));
				
				// Try to lock possession & transport with using EEK, even if plain has flag that we're not using EEK.
				// Possession
				protocol::SignatureKeys secret;
				protocol::SignatureUnlockKeysReq lockRequest2(SF_Possession, &lockKeys, &EEK, nullptr, 0);
				ccstAssertTrue(protocol::LockSignatureKeys(secret, plain, lockRequest2));
				ccstAssertEqual(secret_no_eek.possessionKey, secret.possessionKey);
				ccstAssertFalse(secret.usesExternalKey);
				// Transport
				clearSignatureKeysStruct(secret);
				protocol::SignatureUnlockKeysReq lockRequest3(protocol::SF_Transport, &lockKeys, &EEK, nullptr, 0);
				ccstAssertTrue(protocol::LockSignatureKeys(secret, plain, lockRequest3));
				ccstAssertEqual(secret_no_eek.transportKey, secret.transportKey);
				ccstAssertFalse(secret.usesExternalKey);
				// Transport + Possession
				clearSignatureKeysStruct(secret);
				protocol::SignatureUnlockKeysReq lockRequest4(protocol::SF_Transport|SF_Possession, &lockKeys, &EEK, nullptr, 0);
				ccstAssertTrue(protocol::LockSignatureKeys(secret, plain, lockRequest4));
				ccstAssertEqual(secret_no_eek.transportKey, secret.transportKey);
				ccstAssertEqual(secret_no_eek.possessionKey, secret.possessionKey);
				ccstAssertFalse(secret.usesExternalKey);
				
				// EEK misuse.
				// Biometry + Knowledge should not be locked with EEK, in case that plain struct has EEK flag equal to false.
				protocol::SignatureUnlockKeysReq lockRequest5(SF_Biometry, &lockKeys, &EEK, &knowledgeSalt, knowledgeIterations);
				ccstAssertFalse(protocol::LockSignatureKeys(secret, plain, lockRequest5));
				protocol::SignatureUnlockKeysReq lockRequest6(SF_Knowledge, &lockKeys, &EEK, &knowledgeSalt, knowledgeIterations);
				ccstAssertFalse(protocol::LockSignatureKeys(secret, plain, lockRequest6));
				protocol::SignatureUnlockKeysReq lockRequest7(SF_Knowledge|SF_Biometry, &lockKeys, &EEK, &knowledgeSalt, knowledgeIterations);
				ccstAssertFalse(protocol::LockSignatureKeys(secret, plain, lockRequest7));
			}
			// Lock with EEK
			{
				SignatureFactor factor = protocol::SF_FirstLock;
				protocol::SignatureKeys plain;
				plain.biometryKey = biometryKey;
				plain.possessionKey = possessionKey;
				plain.transportKey = transportKey;
				plain.knowledgeKey = knowledgeKey;
				SignatureUnlockKeys lockKeys;
				lockKeys.biometryUnlockKey = biometryPK;
				lockKeys.possessionUnlockKey = possessionKey;
				lockKeys.userPassword = knowledgePass;
				protocol::SignatureUnlockKeysReq lockRequest(factor, &lockKeys, &EEK, &knowledgeSalt, knowledgeIterations);
				ccstAssertTrue(protocol::LockSignatureKeys(secret_with_eek, plain, lockRequest));
				// Possession & Transport should be the same for EEK & non-EEK protected keys
				ccstAssertEqual(secret_no_eek.transportKey, secret_with_eek.transportKey);
				ccstAssertEqual(secret_no_eek.possessionKey, secret_with_eek.possessionKey);
				ccstAssertNotEqual(secret_no_eek.biometryKey, secret_with_eek.biometryKey);
				ccstAssertNotEqual(secret_no_eek.knowledgeKey, secret_with_eek.knowledgeKey);
				ccstAssertTrue(secret_with_eek.usesExternalKey);
				
				// Try to lock possession & transport without EEK.
				// Normally, EEK usage flag is set during the unlock operation, but in this situation,
				// we have to set it manually.
				plain.usesExternalKey = true;
				// Possession
				protocol::SignatureKeys secret;
				protocol::SignatureUnlockKeysReq lockRequest2(SF_Possession, &lockKeys, nullptr, nullptr, 0);
				ccstAssertTrue(protocol::LockSignatureKeys(secret, plain, lockRequest2));
				ccstAssertEqual(secret_no_eek.possessionKey, secret.possessionKey);
				ccstAssertTrue(secret.usesExternalKey);
				// Transport
				clearSignatureKeysStruct(secret);
				protocol::SignatureUnlockKeysReq lockRequest3(protocol::SF_Transport, &lockKeys, nullptr, nullptr, 0);
				ccstAssertTrue(protocol::LockSignatureKeys(secret, plain, lockRequest3));
				ccstAssertEqual(secret_no_eek.transportKey, secret.transportKey);
				ccstAssertTrue(secret.usesExternalKey);
				// Possession + Transport
				clearSignatureKeysStruct(secret);
				protocol::SignatureUnlockKeysReq lockRequest4(protocol::SF_Transport|SF_Possession, &lockKeys, nullptr, nullptr, 0);
				ccstAssertTrue(protocol::LockSignatureKeys(secret, plain, lockRequest4));
				ccstAssertEqual(secret_no_eek.possessionKey, secret.possessionKey);
				ccstAssertEqual(secret_no_eek.transportKey, secret.transportKey);
				ccstAssertTrue(secret.usesExternalKey);
				
				// EEK misuse.
				// For Biometry & Knowledge, EEK must be consistent between request and plain structure.
				clearSignatureKeysStruct(secret);
				protocol::SignatureUnlockKeysReq lockRequest5(SF_Biometry, &lockKeys, nullptr, &knowledgeSalt, knowledgeIterations);
				ccstAssertFalse(protocol::LockSignatureKeys(secret, plain, lockRequest5));
				protocol::SignatureUnlockKeysReq lockRequest6(SF_Knowledge, &lockKeys, nullptr, &knowledgeSalt, knowledgeIterations);
				ccstAssertFalse(protocol::LockSignatureKeys(secret, plain, lockRequest6));
				protocol::SignatureUnlockKeysReq lockRequest7(SF_Biometry|SF_Knowledge, &lockKeys, nullptr, &knowledgeSalt, knowledgeIterations);
				ccstAssertFalse(protocol::LockSignatureKeys(secret, plain, lockRequest7));
			}
			// Unlock without EEK
			{
				SignatureFactor factor = SF_Possession_Knowledge_Biometry | protocol::SF_Transport;
				SignatureUnlockKeys unlockKeys;
				unlockKeys.biometryUnlockKey = biometryPK;
				unlockKeys.possessionUnlockKey = possessionKey;
				unlockKeys.userPassword = knowledgePass;
				protocol::SignatureUnlockKeysReq unlockRequest(factor, &unlockKeys, nullptr, &knowledgeSalt, knowledgeIterations);
				protocol::SignatureKeys plain;
				ccstAssertTrue(protocol::UnlockSignatureKeys(plain, secret_no_eek, unlockRequest));
				ccstAssertEqual(biometryKey, plain.biometryKey);
				ccstAssertEqual(knowledgeKey, plain.knowledgeKey);
				ccstAssertEqual(possessionKey, plain.possessionKey);
				ccstAssertEqual(transportKey, plain.transportKey);
				
				// Try to unlock possession & transport with EEK, even if EEK flag is false in secret structure.
				// Possession
				clearSignatureKeysStruct(plain);
				protocol::SignatureUnlockKeysReq unlockRequest2(SF_Possession, &unlockKeys, &EEK, nullptr, 0);
				ccstAssertTrue(protocol::UnlockSignatureKeys(plain, secret_no_eek, unlockRequest2));
				ccstAssertEqual(possessionKey, plain.possessionKey);
				ccstAssertFalse(plain.usesExternalKey);
				// Transport
				clearSignatureKeysStruct(plain);
				protocol::SignatureUnlockKeysReq unlockRequest3(protocol::SF_Transport, &unlockKeys, &EEK, nullptr, 0);
				ccstAssertTrue(protocol::UnlockSignatureKeys(plain, secret_no_eek, unlockRequest3));
				ccstAssertEqual(transportKey, plain.transportKey);
				ccstAssertFalse(plain.usesExternalKey);
				// Possession + Transport
				clearSignatureKeysStruct(plain);
				protocol::SignatureUnlockKeysReq unlockRequest4(protocol::SF_Transport|SF_Possession, &unlockKeys, &EEK, nullptr, 0);
				ccstAssertTrue(protocol::UnlockSignatureKeys(plain, secret_no_eek, unlockRequest4));
				ccstAssertEqual(possessionKey, plain.possessionKey);
				ccstAssertEqual(transportKey, plain.transportKey);
				ccstAssertFalse(plain.usesExternalKey);
				
				// Misuse.
				// Knowledge & Biometry should not be unlocked when EEK is present but secret structure says it was created without EEK.
				clearSignatureKeysStruct(plain);
				protocol::SignatureUnlockKeysReq unlockRequest5(SF_Knowledge, &unlockKeys, &EEK, &knowledgeSalt, knowledgeIterations);
				ccstAssertFalse(protocol::LockSignatureKeys(plain, secret_no_eek, unlockRequest5));
				protocol::SignatureUnlockKeysReq unlockRequest6(SF_Biometry, &unlockKeys, &EEK, &knowledgeSalt, knowledgeIterations);
				ccstAssertFalse(protocol::LockSignatureKeys(plain, secret_no_eek, unlockRequest6));
				protocol::SignatureUnlockKeysReq unlockRequest7(SF_Knowledge|SF_Biometry, &unlockKeys, &EEK, &knowledgeSalt, knowledgeIterations);
				ccstAssertFalse(protocol::LockSignatureKeys(plain, secret_no_eek, unlockRequest7));
			}
			// Unlock with EEK
			{
				SignatureFactor factor = SF_Possession_Knowledge_Biometry | protocol::SF_Transport;
				SignatureUnlockKeys unlockKeys;
				unlockKeys.biometryUnlockKey = biometryPK;
				unlockKeys.possessionUnlockKey = possessionKey;
				unlockKeys.userPassword = knowledgePass;
				protocol::SignatureUnlockKeysReq unlockRequest(factor, &unlockKeys, &EEK, &knowledgeSalt, knowledgeIterations);
				protocol::SignatureKeys plain;
				ccstAssertTrue(protocol::UnlockSignatureKeys(plain, secret_with_eek, unlockRequest));
				ccstAssertEqual(biometryKey, plain.biometryKey);
				ccstAssertEqual(knowledgeKey, plain.knowledgeKey);
				ccstAssertEqual(possessionKey, plain.possessionKey);
				ccstAssertEqual(transportKey, plain.transportKey);
				// Try to unlock possession & transport, without EEK. Attempt to unlock possession & transport keys
				// must pass also when EEK is not available.
				// Possession
				clearSignatureKeysStruct(plain);
				protocol::SignatureUnlockKeysReq unlockRequest2(SF_Possession, &unlockKeys, nullptr, nullptr, 0);
				ccstAssertTrue(protocol::UnlockSignatureKeys(plain, secret_with_eek, unlockRequest2));
				ccstAssertEqual(possessionKey, plain.possessionKey);
				// Transport
				clearSignatureKeysStruct(plain);
				protocol::SignatureUnlockKeysReq unlockRequest3(protocol::SF_Transport, &unlockKeys, nullptr, nullptr, 0);
				ccstAssertTrue(protocol::UnlockSignatureKeys(plain, secret_with_eek, unlockRequest3));
				ccstAssertEqual(transportKey, plain.transportKey);
				// Possession + Transport
				clearSignatureKeysStruct(plain);
				protocol::SignatureUnlockKeysReq unlockRequest4(protocol::SF_Transport|SF_Possession, &unlockKeys, nullptr, nullptr, 0);
				ccstAssertTrue(protocol::UnlockSignatureKeys(plain, secret_with_eek, unlockRequest4));
				ccstAssertEqual(transportKey, plain.transportKey);
				ccstAssertEqual(possessionKey, plain.possessionKey);
				
				// EEK misuse.
				clearSignatureKeysStruct(plain);
				protocol::SignatureUnlockKeysReq unlockRequest5(SF_Knowledge, &unlockKeys, nullptr, &knowledgeSalt, knowledgeIterations);
				ccstAssertFalse(protocol::LockSignatureKeys(plain, secret_with_eek, unlockRequest5));
				protocol::SignatureUnlockKeysReq unlockRequest6(SF_Biometry, &unlockKeys, nullptr, &knowledgeSalt, knowledgeIterations);
				ccstAssertFalse(protocol::LockSignatureKeys(plain, secret_with_eek, unlockRequest6));
				protocol::SignatureUnlockKeysReq unlockRequest7(SF_Knowledge|SF_Biometry, &unlockKeys, nullptr, &knowledgeSalt, knowledgeIterations);
				ccstAssertFalse(protocol::LockSignatureKeys(plain, secret_with_eek, unlockRequest7));
			}
		}
		
		// helper methods
		void clearSignatureKeysStruct(protocol::SignatureKeys & keys)
		{
			keys.usesExternalKey = false;
			keys.biometryKey.clear();
			keys.possessionKey.clear();
			keys.knowledgeKey.clear();
			keys.transportKey.clear();
		}

		
	};
	
	CC7_CREATE_UNIT_TEST(pa2ProtocolUtilsTests, "pa2")
	
} // io::getlime::powerAuthTests
} // io::getlime
} // io
