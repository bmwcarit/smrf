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

#ifndef UTIL_H
#define UTIL_H

#include <nan.h>

#include <smrf/ByteArrayView.h>
#include <smrf/ByteVector.h>

#include "ExternalString.h"

namespace util
{
void noOpFreeCallback(char* data, void* hint)
{
    std::ignore = data;
    std::ignore = hint;
}

char* castToCharPointer(const smrf::Byte* buffer)
{
    return const_cast<char*>(reinterpret_cast<const char*>(buffer));
}

decltype(auto) bufferWitNoOpFreeCallback(const smrf::ByteArrayView& view)
{
    return Nan::NewBuffer(castToCharPointer(view.data()), view.size(), noOpFreeCallback, nullptr).ToLocalChecked();
}

template <typename T>
static void bufferFreeCallback(char* data, void* hint)
{
    std::ignore = data;
    delete reinterpret_cast<T*>(hint);
}

auto bufferToByteArrayView(v8::Local<v8::Object> buffer)
{
    const smrf::Byte* bufferPointer = reinterpret_cast<const smrf::Byte*>(node::Buffer::Data(buffer));
    const std::size_t bufferSize = node::Buffer::Length(buffer);
    return smrf::ByteArrayView(bufferPointer, bufferSize);
}

auto string(const char* str)
{
    return Nan::New<v8::String>(str).ToLocalChecked();
}

auto viewToString(const smrf::ByteArrayView& view)
{
    return Nan::New<v8::String>(reinterpret_cast<const char*>(view.data()), view.size()).ToLocalChecked();
}

auto viewToExternalString(const smrf::ByteArrayView& view)
{
    return Nan::New<v8::String>(new ExternalString(reinterpret_cast<const char*>(view.data()), view.size())).ToLocalChecked();
}

auto byteVectorToBuffer(smrf::ByteVector&& vec)
{
    // store vec in a heap allocated vector
    // node buffer wraps around that vector's data
    // this vector will be deleted when the node Buffer goes out of scope
    auto* byteVector = new smrf::ByteVector(std::move(vec));
    return Nan::NewBuffer(
                   castToCharPointer(byteVector->data()), byteVector->size(), bufferFreeCallback<smrf::ByteVector>, byteVector)
            .ToLocalChecked();
}

} // namespace util

#endif // UTIL_H
