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

package io.getlime.security.powerauth.sdk.impl;

import io.getlime.security.powerauth.networking.base.PA2AuthorizationHttpHeader;

/**
 * Created by miroslavmichalec on 25/10/2016.
 */

public class PowerAuthAuthorizationHttpHeader extends PA2AuthorizationHttpHeader {

    private int powerAuthErrorCode;

    public PowerAuthAuthorizationHttpHeader(String value, int powerAuthErrorCode) {
        super(value);
        this.powerAuthErrorCode = powerAuthErrorCode;
    }

    public int getPowerAuthErrorCode() {
        return powerAuthErrorCode;
    }
}
