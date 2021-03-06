/**
 * Copyright 2016 Lime - HighTech Solutions s.r.o.
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

#import <Foundation/Foundation.h>

/** Class that represents a PowerAuthSDK instance configuration.
 */
@interface PowerAuthConfiguration : NSObject<NSCopying>

/** Identifier of the PowerAuthSDK instance, used as a 'key' to store session state in the session state keychain.
 */
@property (nonatomic, strong, nonnull) NSString	*instanceId;

/** Base URL to the PowerAuth 2.0 Standard RESTful API (the URL part before "/pa/...").
 */
@property (nonatomic, strong, nonnull) NSString	*baseEndpointUrl;

/** APPLICATION_KEY as defined in PowerAuth 2.0 specification - a key identifying an application version.
 */
@property (nonatomic, strong, nonnull) NSString	*appKey;

/** APPLICATION_SECRET as defined in PowerAuth 2.0 specification - a secret associated with an application version.
 */
@property (nonatomic, strong, nonnull) NSString	*appSecret;

/** KEY_SERVER_MASTER_PUBLIC as defined in PowerAuth 2.0 specification - a master server public key.
 */
@property (nonatomic, strong, nonnull) NSString	*masterServerPublicKey;

/** This value specifies 'key' used to store this PowerAuthSDK instance biometry related key in the biometry key keychain.
 */
@property (nonatomic, strong, nonnull) NSString *keychainKey_Biometry;

/** Encryption key provided by an external context, used to encrypt possession and biometry related factor keys under the hood.
 */
@property (nonatomic, strong, nullable) NSData	*externalEncryptionKey;

/** Validate that the configuration is properly set (all required values were filled in).
 */
- (BOOL) validateConfiguration;

@end
