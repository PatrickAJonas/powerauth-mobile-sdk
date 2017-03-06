/*
 * Copyright 2017 Lime - HighTech Solutions s.r.o.
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

package io.getlime.security.powerauth.core;

/**
 Result from first step of device activation.
 */
public class ActivationStep1Result {

    /**
     Error code returned from the C++ code. The value can be compared
     to constants from ErrorCode class.
     */
    public final int errorCode;
    
    /**
     Activation nonce, in Base64 format
     */
    public final String activationNonce;
    
    /**
     Encrypted device's public key, in Base64 format
     */
    public final String cDevicePublicKey;
    
    /**
     Application signature proving that activation was completed
     with correct application.
     */
    public final String applicationSignature;

    /**
     Ephemeral public key used for ad-hoc encryption used to protect
     cDevicePublicKey.
     */
    public final String ephemeralPublicKey;

    public ActivationStep1Result() {
        this.errorCode = 0;
        this.activationNonce = null;
        this.cDevicePublicKey = null;
        this.applicationSignature = null;
        this.ephemeralPublicKey = null;
    }
}