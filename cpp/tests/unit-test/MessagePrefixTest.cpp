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
#include <limits>
#include <type_traits>

#include <gtest/gtest.h>

#include "smrf/ByteArrayView.h"
#include "smrf/ByteVector.h"

#include "../MessagePrefix.h"

using namespace smrf;

TEST(MessagePrefixTest, tooShortInputCausesException)
{
    for (std::size_t i = 0; i < MessagePrefix::SIZE; ++i) {
        ByteVector byteVector(i);
        ByteArrayView view(byteVector);
        EXPECT_THROW(MessagePrefix{view}, EncodingException);
    }
}

TEST(MessagePrefixTest, roundTrip)
{
    auto setMax = [](auto& value) { value = std::numeric_limits<std::decay_t<decltype(value)>>::max(); };
    MessagePrefix expectedMessagePrefix;

    setMax(expectedMessagePrefix.version);
    setMax(expectedMessagePrefix.msgSize);
    setMax(expectedMessagePrefix.sigSize);

    ByteVector byteVector(MessagePrefix::SIZE);
    expectedMessagePrefix.writeToPreallocatedBuffer(byteVector);

    ByteArrayView view(byteVector);
    MessagePrefix messagePrefix(view);
    EXPECT_EQ(expectedMessagePrefix.version, messagePrefix.version);
    EXPECT_EQ(expectedMessagePrefix.msgSize, messagePrefix.msgSize);
    EXPECT_EQ(expectedMessagePrefix.sigSize, messagePrefix.sigSize);
}
