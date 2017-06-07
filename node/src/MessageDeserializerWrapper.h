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
#ifndef MESSAGEDESERIALIZERWRAPPER_H
#define MESSAGEDESERIALIZERWRAPPER_H

#include <cassert>

#include <nan.h>
#include <smrf/MessageDeserializer.h>
#include <smrf/exceptions.h>

#include "Strings.h"
#include "util.h"

class MessageDeserializerWrapper : public Nan::ObjectWrap
{
public:
    MessageDeserializerWrapper(Nan::NAN_METHOD_ARGS_TYPE info)
    {
        if (info.Length() != 1) {
            Nan::ThrowError("invalid number of arguments");
            return;
        }

        v8::Local<v8::Value> bufferValue = info[0];

        if (!node::Buffer::HasInstance(bufferValue)) {
            Nan::ThrowTypeError("argument must be a buffer");
            return;
        }

        v8::Local<v8::Object> serializedMessage = bufferValue->ToObject();
        info.This()->Set(Nan::New(Strings::get().serializedMessage), serializedMessage);

        smrf::ByteArrayView serializedMessageView = util::bufferToByteArrayView(serializedMessage);

        try {
            deserializer = std::make_unique<smrf::MessageDeserializer>(serializedMessageView);
        } catch (const smrf::EncodingException& e) {
            Nan::ThrowError(e.what());
            return;
        }

        const bool isCompressed = deserializer->isCompressed();
        info.This()->Set(Nan::New(Strings::get().isCompressed), Nan::New(isCompressed));

        if (!deserializer->isEncrypted()) {
            v8::Local<v8::Object> bodyBuffer;
            if (!isCompressed) {
                bodyBuffer = util::viewToBuffer(deserializer->getBody());
            } else {
                bodyBuffer = util::byteVectorToBuffer(deserializer->decompressBody());
            }
            info.This()->Set(Nan::New(Strings::get().body), bodyBuffer);
        } else {
            Nan::ThrowError("encryption not yet supported");
            return;
        }

        info.This()->Set(Nan::New(Strings::get().sender), util::viewToString(deserializer->getRawSender()));
        info.This()->Set(Nan::New(Strings::get().recipient), util::viewToString(deserializer->getRawRecipient()));

        v8::Local<v8::Object> headers = Nan::New<v8::Object>();
        std::vector<std::pair<smrf::ByteArrayView, smrf::ByteArrayView>> rawHeaders = deserializer->getRawHeaders();
        for (const auto& header : rawHeaders) {
            headers->Set(util::viewToString(header.first), util::viewToString(header.second));
        }
        info.This()->Set(Nan::New(Strings::get().headers), headers);

        // JS Number type is a double internally
        const double ttlMs = static_cast<double>(deserializer->getTtlMs());
        info.This()->Set(Nan::New(Strings::get().ttlMs), Nan::New(ttlMs));

        const bool isTtlAbsolute = deserializer->isTtlAbsolute();
        info.This()->Set(Nan::New(Strings::get().isTtlAbsolute), Nan::New(isTtlAbsolute));
    }

    static NAN_METHOD(New)
    {
        assert(info.IsConstructCall());
        auto* wrapper = new MessageDeserializerWrapper(info);
        wrapper->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    }

    static NAN_MODULE_INIT(Init)
    {
        auto className = util::string("MessageDeserializer");

        v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
        tpl->SetClassName(className);
        auto constructorInstance = tpl->InstanceTemplate();
        constructorInstance->SetInternalFieldCount(1);

        auto tplFunc = Nan::GetFunction(tpl).ToLocalChecked();
        constructorFunction().Reset(tplFunc);
        Nan::Set(target, className, tplFunc);
    }

private:
    static Nan::Persistent<v8::Function>& constructorFunction()
    {
        static Nan::Persistent<v8::Function> constructor;
        return constructor;
    }

    std::unique_ptr<smrf::MessageDeserializer> deserializer;
};

#endif // MESSAGEDESERIALIZERWRAPPER_H
