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

#ifndef MARSHALLING_H
#define MARSHALLING_H

#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#include <boost/optional.hpp>
#include <nan.h>

#include "util.h"

namespace marshalling
{

using OptionalV8Value = boost::optional<v8::Local<v8::Value>>;

OptionalV8Value maybeToOptionalValue(Nan::MaybeLocal<v8::Value> maybeValue)
{
    OptionalV8Value optionalV8Value;
    if (!maybeValue.IsEmpty()) {
        v8::Local<v8::Value> value = maybeValue.ToLocalChecked();
        if (!value->IsUndefined()) {
            optionalV8Value = value;
        }
    }
    return optionalV8Value;
}

OptionalV8Value getOptionalMemberValue(v8::Local<v8::Object> context, const char* key)
{
    Nan::MaybeLocal<v8::Value> maybeValue = Nan::Get(context, Nan::New(key).ToLocalChecked());
    return maybeToOptionalValue(maybeValue);
}

void convertFromV8(const v8::Local<v8::Value>& v8Value, bool& value)
{
    value = v8Value->BooleanValue();
}

void convertFromV8(const v8::Local<v8::Value>& v8Value, std::string& value)
{
    Nan::Utf8String stringValue(v8Value->ToString());
    value = std::string(*stringValue, stringValue.length());
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value> convertFromV8(const v8::Local<v8::Value>& v8Value, T& value)
{
    value = v8Value->NumberValue();
}

v8::Local<v8::Value> getMemberValue(const v8::Local<v8::Object>& context, const v8::Local<v8::String>& key)
{
    Nan::MaybeLocal<v8::Value> maybeValue = Nan::Get(context, key);
    if (!maybeValue.IsEmpty()) {
        v8::Local<v8::Value> value = maybeValue.ToLocalChecked();
        if (!value->IsUndefined()) {
            return value;
        }
    }
    std::string keyStr;
    convertFromV8(key, keyStr);
    throw std::invalid_argument(std::string("member is not set: " + keyStr));
}

v8::Local<v8::Value> getMemberValue(const v8::Local<v8::Object>& context, const Nan::Persistent<v8::String>& key)
{
    return getMemberValue(context, Nan::New(key));
}

template <std::size_t N>
v8::Local<v8::Value> getMemberValue(const v8::Local<v8::Object>& context, const char (&key)[N])
{
    return getMemberValue(context, util::string(key));
}

} // namespace marshalling

#endif // MARSHALLING_H
