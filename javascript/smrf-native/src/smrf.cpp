/*
 * #%L
 * %%
 * Copyright (C) 2017 BMW Car IT GmbH
 * %%
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * #L%
 */

#include <cstdint>
#include <exception>
#include <functional>
#include <memory>
#include <string>

#include <nan.h>
#include <v8.h>

#include <smrf/ByteArrayView.h>
#include <smrf/ByteVector.h>
#include <smrf/MessageDeserializer.h>
#include <smrf/MessageSerializer.h>

#include "MessageDeserializerWrapper.h"
#include "Strings.h"
#include "marshalling.h"
#include "util.h"

NAN_METHOD(serialize)
{
    using marshalling::convertFromV8;
    using marshalling::getMemberValue;
    using marshalling::getOptionalMemberValue;
    using v8::Local;

    const std::size_t requiredArgCount = 1;

    if (info.Length() != requiredArgCount) {
        return Nan::ThrowError("invalid number of arguments");
    }

    smrf::MessageSerializer serializer;
    // iterate over arguments, pass them on to the serializer
    v8::Local<v8::Object> context = info[0].As<v8::Object>();
    std::string sender;
    std::string recipient;
    std::int64_t ttlMs;
    bool isCompressed;
    bool isTtlAbsolute;

    smrf::ByteVector serializedMessage;

    try {
        convertFromV8(getMemberValue(context, Strings::get().sender), sender);
        serializer.setSender(sender);

        convertFromV8(getMemberValue(context, Strings::get().recipient), recipient);
        serializer.setRecipient(recipient);

        convertFromV8(getMemberValue(context, Strings::get().ttlMs), ttlMs);
        serializer.setTtlMs(ttlMs);

        convertFromV8(getMemberValue(context, Strings::get().isTtlAbsolute), isTtlAbsolute);
        serializer.setTtlAbsolute(isTtlAbsolute);

        convertFromV8(getMemberValue(context, Strings::get().isCompressed), isCompressed);
        serializer.setCompressed(isCompressed);

        using OptionalV8Value = boost::optional<v8::Local<v8::Value>>;
        OptionalV8Value cb = getOptionalMemberValue(context, "signingCallback");

        if (cb) {
            // It MUST NOT copy the argument it is invoked with
            // it MUST be a synchronous function
            std::function<smrf::ByteVector(const smrf::ByteArrayView&)> signingCallback =
                    [&cb, &context](const smrf::ByteArrayView& msg) {
                        v8::Local<v8::Value> msgBuffer = util::bufferWitNoOpFreeCallback(msg);
                        const unsigned argc = 1;
                        v8::Local<v8::Value> argv[] = {msgBuffer};
                        v8::Local<v8::Value> signatureBuffer = cb->As<v8::Function>()->Call(context, argc, argv);
                        smrf::ByteVector signature = util::bufferToByteVector(signatureBuffer);
                        return signature;
                    };

            serializer.setCustomSigningCallback(std::move(signingCallback));
        }

        v8::Local<v8::Value> bufferValue = getMemberValue(context, Strings::get().body);
        if (!node::Buffer::HasInstance(bufferValue)) {
            return Nan::ThrowTypeError("'body' must be a buffer");
        }
        smrf::ByteArrayView bufferView = util::bufferToByteArrayView(bufferValue->ToObject());
        serializer.setBody(bufferView);

        v8::Local<v8::Value> headersValue = getMemberValue(context, Strings::get().headers);
        if (!headersValue->IsObject()) {
            return Nan::ThrowTypeError("'headers' must be an object");
        }
        v8::Local<v8::Object> headers = headersValue->ToObject();
        Nan::MaybeLocal<v8::Array> maybeProperties = Nan::GetOwnPropertyNames(headers);
        if (!maybeProperties.IsEmpty()) {
            v8::Local<v8::Array> properties = maybeProperties.ToLocalChecked();
            const std::size_t headerCount = properties->Length();
            std::unordered_map<std::string, std::string> headerMap(headerCount);
            for (std::size_t i = 0; i < headerCount; ++i) {
                v8::Local<v8::Value> key = properties->Get(i);
                std::string keyStr;
                convertFromV8(key, keyStr);

                v8::Local<v8::Value> value = headers->Get(key);
                std::string valueStr;
                convertFromV8(value, valueStr);

                headerMap.emplace(std::move(keyStr), std::move(valueStr));
            }
            serializer.setHeaders(headerMap);
        }
        // finally, serialize the message
        serializedMessage = serializer.serialize();
        // finally, serialize the message and return the resulting buffer
        info.GetReturnValue().Set(util::byteVectorToBuffer(std::move(serializedMessage)));

    } catch (const std::invalid_argument& e) {
        return Nan::ThrowTypeError(e.what());
    }
}

NAN_MODULE_INIT(Init)
{
    Strings::init();
    MessageDeserializerWrapper::Init(target);
    using namespace Nan;
    Set(target, util::string("serialize"), New<v8::FunctionTemplate>(serialize)->GetFunction());
}

NODE_MODULE(smrf_native, Init)
