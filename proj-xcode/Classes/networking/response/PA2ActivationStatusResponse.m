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

#import "PA2ActivationStatusResponse.h"

@implementation PA2ActivationStatusResponse

- (instancetype)initWithDictionary:(NSDictionary *)dictionary {
    self = [super init];
    if (self) {
        _activationId			= [dictionary objectForKey:@"activationId"];
        _encryptedStatusBlob	= [dictionary objectForKey:@"encryptedStatusBlob"];
        id customObject			= [dictionary objectForKey:@"customObject"];
        if ([customObject isKindOfClass:[NSDictionary class]]) {
            _customObject = customObject;
        }
    }
    return self;
}

- (NSDictionary *)toDictionary {
    NSMutableDictionary *dictionary = [NSMutableDictionary dictionary];
    if (_activationId) {
        [dictionary setObject:_activationId forKey:@"activationId"];
    }
    if (_encryptedStatusBlob) {
        [dictionary setObject:_encryptedStatusBlob forKey:@"encryptedStatusBlob"];
    }
    if (_customObject && [_customObject isKindOfClass:[NSDictionary class]]) {
        [dictionary setObject:_customObject forKey:@"customObject"];
    }
    return dictionary;
}

@end
