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

class RoundtripTest : public ::testing::Test
{
public:
    RoundtripTest()
    {
        customSignature = {'T', 'h', 'i', 's', ' ', 'i', 's', ' ', 'a', ' ', 't', 'e', 's',
                           't', ' ', 's', 'i', 'g', 'n', 'a', 't', 'u', 'r', 'e', '!'};
    }

protected:
    void checkCustomSignature(const smrf::MessageDeserializer& deserializer)
    {
        ByteArrayView signature = deserializer.getSignature();
        ASSERT_EQ(customSignature.size(), signature.size());

        for (std::size_t i = 0; i < customSignature.size(); ++i) {
            EXPECT_EQ(customSignature[i], *(signature.data() + i));
        }
    }
    ByteVector customSignature;
};

TEST_F(RoundtripTest, notCompressedNotSignedNotEncryptedNotCustomSigned)
{
    const bool isCompressed = false;
    RoundtripMessage message(isCompressed);
    MessageSerializer serializer;
    message.init(serializer);

    const ByteVector& serializedMessage = serializer.serialize();
    ByteArrayView serializedMessageView(serializedMessage);
    MessageDeserializer deserializer(serializedMessageView);
    EXPECT_EQ(serializedMessageView.size(), deserializer.getMessageSize());
    ASSERT_FALSE(deserializer.isSigned());
    ASSERT_FALSE(deserializer.isEncrypted());
    ASSERT_FALSE(deserializer.isCustomSigned());
    message.check(deserializer);
}

TEST_F(RoundtripTest, compressedNotSignedNotEncryptedNotCustomSigned)
{
    const bool isCompressed = true;
    RoundtripMessage message(isCompressed);
    MessageSerializer serializer;
    message.init(serializer);

    const ByteVector& serializedMessage = serializer.serialize();
    ByteArrayView serializedMessageView(serializedMessage);
    MessageDeserializer deserializer(serializedMessageView);
    EXPECT_EQ(serializedMessageView.size(), deserializer.getMessageSize());
    ASSERT_FALSE(deserializer.isSigned());
    ASSERT_FALSE(deserializer.isEncrypted());
    ASSERT_FALSE(deserializer.isCustomSigned());
    message.check(deserializer);
}

TEST_F(RoundtripTest, notCompressedNotEncryptedCustomSigned)
{
    const bool isCompressed = false;
    RoundtripMessage message(isCompressed);
    MessageSerializer serializer;
    message.init(serializer);
    auto signingCallback = [this](const ByteArrayView&) { return customSignature; };
    serializer.setCustomSigningCallback(signingCallback);

    const ByteVector& serializedMessage = serializer.serialize();
    ByteArrayView serializedMessageView(serializedMessage);
    MessageDeserializer deserializer(serializedMessageView);
    EXPECT_EQ(serializedMessageView.size(), deserializer.getMessageSize());
    ASSERT_FALSE(deserializer.isSigned());
    ASSERT_FALSE(deserializer.isEncrypted());
    ASSERT_TRUE(deserializer.isCustomSigned());
    checkCustomSignature(deserializer);
    message.check(deserializer);
}

TEST_F(RoundtripTest, compressedNotEncryptedCustomSigned)
{
    const bool isCompressed = true;
    RoundtripMessage message(isCompressed);
    MessageSerializer serializer;
    message.init(serializer);
    auto signingCallback = [this](const ByteArrayView&) { return customSignature; };
    serializer.setCustomSigningCallback(signingCallback);

    const ByteVector& serializedMessage = serializer.serialize();
    ByteArrayView serializedMessageView(serializedMessage);
    MessageDeserializer deserializer(serializedMessageView);
    EXPECT_EQ(serializedMessageView.size(), deserializer.getMessageSize());
    ASSERT_FALSE(deserializer.isSigned());
    ASSERT_FALSE(deserializer.isEncrypted());
    ASSERT_TRUE(deserializer.isCustomSigned());
    checkCustomSignature(deserializer);
    message.check(deserializer);
}
