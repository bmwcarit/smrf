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

#ifndef ROUNDTRIPMESSAGE_H
#define ROUNDTRIPMESSAGE_H

#include <cstdint>
#include <string>
#include <unordered_map>

#include <gtest/gtest.h>

#include "smrf/ByteArrayView.h"
#include "smrf/ByteVector.h"
#include "smrf/MessageDeserializer.h"
#include "smrf/MessageSerializer.h"

class RoundtripMessage
{
public:
    RoundtripMessage(bool shouldBeCompressed)
    {
        sender = "sender";
        recipient = "recipient";
        ttlMs = 1234567;
        ttlAbsolute = false;
        const std::size_t bodySize = 100;
        body.resize(bodySize);
        for (std::size_t i = 0; i < 100; ++i) {
            body[i] = i;
        }

        headers["key1"] = "value1";
        headers["key2"] = "value2";

        isCompressed = shouldBeCompressed;
    }

    void init(smrf::MessageSerializer& serializer)
    {
        serializer.setSender(sender);
        serializer.setRecipient(recipient);
        serializer.setHeaders(headers);
        serializer.setTtlMs(ttlMs);
        serializer.setTtlAbsolute(ttlAbsolute);
        serializer.setBody(smrf::ByteArrayView(body));
        serializer.setCompressed(isCompressed);
    }

    void check(const smrf::MessageDeserializer& deserializer)
    {
        EXPECT_EQ(sender, deserializer.getSender());
        EXPECT_EQ(recipient, deserializer.getRecipient());
        EXPECT_EQ(headers, deserializer.getHeaders());
        EXPECT_EQ(ttlMs, deserializer.getTtlMs());
        EXPECT_EQ(ttlAbsolute, deserializer.isTtlAbsolute());
        EXPECT_EQ(isCompressed, deserializer.isCompressed());

        if (!isCompressed) {
            smrf::ByteArrayView deserializedBodyView = deserializer.getBody();
            ASSERT_EQ(body.size(), deserializedBodyView.size());
            // element-wise comparison
            for (std::size_t i = 0; i < body.size(); ++i) {
                EXPECT_EQ(body[i], *(deserializedBodyView.data() + i));
            }
        } else {
            EXPECT_EQ(body, deserializer.decompressBody());
        }
    }

private:
    std::string sender;
    std::string recipient;
    std::uint64_t ttlMs;
    bool ttlAbsolute;
    smrf::ByteVector body;
    bool isCompressed;
    std::unordered_map<std::string, std::string> headers;
};

#endif // ROUNDTRIPMESSAGE_H
