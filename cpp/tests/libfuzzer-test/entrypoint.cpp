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
#include <tuple> // for std::ignore

#include "smrf/MessageDeserializer.h"
#include "smrf/exceptions.h"

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t* data, std::size_t size)
{
    using namespace smrf;
    ByteArrayView serializedMessageView(data, size);
    try {
        MessageDeserializer deserializer(serializedMessageView);
        std::ignore = deserializer.getSender();
        std::ignore = deserializer.getRecipient();
        std::ignore = deserializer.getTtlMs();
        std::ignore = deserializer.isTtlAbsolute();
        std::ignore = deserializer.getHeaders();
        std::ignore = deserializer.getBody();
        std::ignore = deserializer.decompressBody();
    } catch (const smrf::EncodingException&) {
    }

    return 0;
}