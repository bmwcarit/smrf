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

#include <string>
#include <unordered_map>
#include <vector>

#include <gtest/gtest.h>

#include "smrf/ByteArrayView.h"
#include "smrf/ByteVector.h"
#include "smrf/MessageDeserializer.h"
#include "smrf/MessageSerializer.h"

#include "../RoundtripMessage.h"

using namespace smrf;

TEST(RoundtripTest, notCompressedNotSignedNotEncrypted)
{
    const bool isCompressed = false;
    RoundtripMessage message(isCompressed);
    MessageSerializer serializer;
    message.init(serializer);

    const ByteVector& serializedMessage = serializer.serialize();
    ByteArrayView serializedMessageView(serializedMessage);
    MessageDeserializer deserializer(serializedMessageView);
    ASSERT_FALSE(deserializer.isSigned());
    ASSERT_FALSE(deserializer.isEncrypted());
    message.check(deserializer);
}

TEST(RoundtripTest, compressedNotSignedNotEncrypted)
{
    const bool isCompressed = true;
    RoundtripMessage message(isCompressed);
    MessageSerializer serializer;
    message.init(serializer);

    const ByteVector& serializedMessage = serializer.serialize();
    ByteArrayView view(serializedMessage);
    MessageDeserializer deserializer(view);
    ASSERT_FALSE(deserializer.isSigned());
    ASSERT_FALSE(deserializer.isEncrypted());
    message.check(deserializer);
}
