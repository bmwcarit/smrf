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

#ifndef SMRF_BYTEARRAYVIEW_H
#define SMRF_BYTEARRAYVIEW_H

#include <cstdint>

#include "smrf/ByteVector.h"

namespace smrf
{

/**
 * @brief This class provides a read-only view for a contiguous chunk of memory.
 */
class ByteArrayView
{
public:
    ByteArrayView() : arrayData(nullptr), arraySize(0)
    {
    }

    explicit ByteArrayView(const Byte* arrayData, std::size_t arraySize) : arrayData(arrayData), arraySize(arraySize)
    {
    }

    explicit ByteArrayView(const ByteVector& byteVector) : arrayData(byteVector.data()), arraySize(byteVector.size())
    {
    }

    ByteArrayView(const ByteArrayView&) = default;
    ByteArrayView& operator=(const ByteArrayView&) = default;

    const Byte* data() const
    {
        return arrayData;
    }

    std::size_t size() const
    {
        return arraySize;
    }

private:
    const Byte* arrayData;
    std::size_t arraySize;
};
} // namespace smrf

#endif // SMRF_BYTEARRAYVIEW_H
