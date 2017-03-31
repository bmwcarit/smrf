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

using namespace smrf;

class RoundtripTest : public ::testing::Test
{

public:
    void init(MessageSerializer& serializer)
    {
        serializer.setSender(sender);
        serializer.setRecipient(recipient);
        serializer.setHeaders(headers);
        serializer.setTtlMs(ttlMs);
        serializer.setTtlAbsolute(ttlAbsolute);
        serializer.setBody(ByteArrayView(body));
    }

    void checkResult(MessageDeserializer& deserializer) const
    {
        EXPECT_EQ(sender, deserializer.getSender());
        EXPECT_EQ(recipient, deserializer.getRecipient());
        EXPECT_EQ(headers, deserializer.getHeaders());
        EXPECT_EQ(ttlMs, deserializer.getTtlMs());
        EXPECT_EQ(ttlAbsolute, deserializer.isTtlAbsolute());
    }

protected:
    const std::string sender = "sender";
    const std::string recipient = "recipient";
    const std::unordered_map<std::string, std::string> headers = {{"key1", "value1"}, {"key2", "value2"}};
    const std::int64_t ttlMs = 12345;
    const bool ttlAbsolute = false;
    const ByteVector body = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
};

TEST_F(RoundtripTest, notCompressedNotSignedNotEncrypted)
{
    MessageSerializer serializer;
    init(serializer);
    serializer.setCompressed(false);

    const ByteVector& serializedMessage = serializer.serialize();
    ByteArrayView serializedMessageView(serializedMessage);
    MessageDeserializer deserializer(serializedMessageView);
    ASSERT_FALSE(deserializer.isCompressed());
    ASSERT_FALSE(deserializer.isSigned());
    ASSERT_FALSE(deserializer.isEncrypted());

    checkResult(deserializer);

    ByteArrayView deserializedBodyView = deserializer.getBody();
    ASSERT_EQ(body.size(), deserializedBodyView.size());
    // element-wise comparison
    for (std::size_t i = 0; i < body.size(); ++i) {
        EXPECT_EQ(body[i], *(deserializedBodyView.data() + i));
    }
}

TEST_F(RoundtripTest, compressedNotSignedNotEncrypted)
{
    MessageSerializer serializer;
    init(serializer);
    serializer.setCompressed(true);

    const ByteVector& serializedMessage = serializer.serialize();
    ByteArrayView view(serializedMessage);
    MessageDeserializer deserializer(view);
    ASSERT_TRUE(deserializer.isCompressed());
    ASSERT_FALSE(deserializer.isSigned());
    ASSERT_FALSE(deserializer.isEncrypted());

    checkResult(deserializer);
    EXPECT_EQ(body, deserializer.decompressBody());
}
