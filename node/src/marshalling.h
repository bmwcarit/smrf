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

#include <nan.h>

namespace marshalling
{

v8::Local<v8::Value> getMemberValue(v8::Local<v8::Object> context, const char* key)
{
    Nan::MaybeLocal<v8::Value> maybeValue = Nan::Get(context, Nan::New(key).ToLocalChecked());
    if (!maybeValue.IsEmpty()) {
        v8::Local<v8::Value> value = maybeValue.ToLocalChecked();
        if (!value->IsUndefined()) {
            return value;
        }
    }
    throw std::invalid_argument(std::string("member is not set: ") + key);
}

void convertFromV8(v8::Local<v8::Value> v8Value, bool& value)
{
    value = v8Value->BooleanValue();
}

void convertFromV8(v8::Local<v8::Value> v8Value, std::string& value)
{
    v8::String::Utf8Value stringValue(v8Value->ToString());
    value = std::string(*stringValue, stringValue.length());
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value> convertFromV8(v8::Local<v8::Value> v8Value, T& value)
{
    value = v8Value->NumberValue();
}

} // namespace marshalling

#endif // MARSHALLING_H
