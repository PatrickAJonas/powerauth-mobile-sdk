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

#include <PowerAuth/OtpUtil.h>
#include <cc7/jni/JniHelper.h>

// Package: io.getlime.security.powerauth.util.otp
#define CC7_JNI_CLASS_PATH	    	"io/getlime/security/powerauth/util/otp"
#define CC7_JNI_CLASS_PACKAGE	    io_getlime_security_powerauth_util_otp
#define CC7_JNI_JAVA_CLASS  		OtpUtil
#define CC7_JNI_CPP_CLASS		    OtpUtil
#include <cc7/jni/JniModule.inl>

using namespace io::getlime::powerAuth;

// This class has all its methods declared as static. We don't need to use CC7_JNI_MODULE_CLASS_BEGIN
// macro, because we're OK with simple "exttern C" declaration.

extern "C" {

// ----------------------------------------------------------------------------
// Parser
// ----------------------------------------------------------------------------

//
// public native static Otp parseFromActivationCode(String activationCode)
//
CC7_JNI_METHOD_PARAMS(jobject, parseFromActivationCode, jstring activationCode)
{
	std::string cppActivationCode = cc7::jni::CopyFromJavaString(env, activationCode);
	OtpComponents cppComponents;
	if (false == OtpUtil::parseActivationCode(cppActivationCode, cppComponents)) {
		return NULL;
	}
	// Copy cppResult into java result object
	jclass  resultClazz  = CC7_JNI_MODULE_FIND_CLASS("Otp");
	jobject resultObject = cc7::jni::CreateJavaObject(env, CC7_JNI_MODULE_CLASS_PATH("Otp"), "()V");
	CC7_JNI_SET_FIELD_STRING(resultObject, resultClazz, "activationIdShort",	cc7::jni::CopyToJavaString(env, cppComponents.activationIdShort));
	CC7_JNI_SET_FIELD_STRING(resultObject, resultClazz, "activationOtp",		cc7::jni::CopyToJavaString(env, cppComponents.activationOtp));
	CC7_JNI_SET_FIELD_STRING(resultObject, resultClazz, "activationSignature",	cc7::jni::CopyToNullableJavaString(env, cppComponents.activationSignature));
	return resultObject;
}

// ----------------------------------------------------------------------------
// Validations
// ----------------------------------------------------------------------------

//
// public native static boolean validateTypedCharacter(int utfCodepoint)
//
CC7_JNI_METHOD_PARAMS(jboolean, validateTypedCharacter, jint utfCodepoint)
{
	return (jboolean) OtpUtil::validateTypedCharacter((cc7::U32) utfCodepoint);
}

//
// public native static int validateAndCorrectTypedCharacter(int utfCodepoint)
//
CC7_JNI_METHOD_PARAMS(jint, validateAndCorrectTypedCharacter, jint utfCodepoint)
{
	return (jint) OtpUtil::validateAndCorrectTypedCharacter((cc7::U32) utfCodepoint);
}

//
// public native static boolean validateActivationCode(String activationCode)
//
CC7_JNI_METHOD_PARAMS(jboolean, validateActivationCode, jstring activationCode)
{
	std::string cppActivationCode = cc7::jni::CopyFromJavaString(env, activationCode);
	return (jboolean) OtpUtil::validateActivationCode(cppActivationCode);
}

} // extern "C"
