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

#ifndef SMRF_MESSAGEPREFIX_H
#define SMRF_MESSAGEPREFIX_H

#include <cassert>
#include <cstdint>

#include <boost/endian/conversion.hpp>

#include "smrf/ByteArrayView.h"
#include "smrf/ByteVector.h"
#include "smrf/exceptions.h"

namespace smrf
{

class MessagePrefix
{
public:
    explicit MessagePrefix(const ByteArrayView& serializedMessage)
    {
        if (serializedMessage.size() < SIZE) {
            throw EncodingException("message size too small");
        }
        read(serializedMessage, VERSION_OFFSET, version);
        read(serializedMessage, SIG_SIZE_OFFSET, sigSize);
        read(serializedMessage, MSG_SIZE_OFFSET, msgSize);
    }

    MessagePrefix() = default;

    void writeToPreallocatedBuffer(ByteVector& buffer)
    {
        assert(buffer.size() >= SIZE);
        write(buffer, VERSION_OFFSET, version);
        write(buffer, SIG_SIZE_OFFSET, sigSize);
        write(buffer, MSG_SIZE_OFFSET, msgSize);
    }

    static constexpr std::uint16_t VERSION = 1;

    std::uint16_t version = VERSION;
    std::uint16_t sigSize = 0;
    std::uint32_t msgSize = 0;

    static constexpr std::size_t VERSION_OFFSET = 0;
    static constexpr std::size_t SIG_SIZE_OFFSET = VERSION_OFFSET + sizeof(version);
    static constexpr std::size_t MSG_SIZE_OFFSET = SIG_SIZE_OFFSET + sizeof(sigSize);

    /**
     * Size in bytes of the message prefix
     */
    static constexpr std::size_t SIZE = MSG_SIZE_OFFSET + sizeof(msgSize);

private:
    template <typename T>
    void read(const ByteArrayView& serializedMessage, std::size_t offset, T& value)
    {
        value = boost::endian::little_to_native(*reinterpret_cast<const T*>(serializedMessage.data() + offset));
    }

    template <typename T>
    void write(ByteVector& buffer, std::size_t offset, T value)
    {
        *reinterpret_cast<T*>(buffer.data() + offset) = boost::endian::native_to_little(value);
    }
};

} // namespace smrf

#endif // SMRF_MESSAGEPREFIX_H
