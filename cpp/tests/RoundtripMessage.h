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
        _sender = "sender";
        _recipient = "recipient";
        _ttlMs = 1234567;
        _ttlAbsolute = false;
        const std::size_t bodySize = 100;
        _body.resize(bodySize);
        for (std::size_t i = 0; i < 100; ++i) {
            _body[i] = i;
        }

        _headers["key1"] = "value1";
        _headers["key2"] = "value2";

        _isCompressed = shouldBeCompressed;
    }

    void init(smrf::MessageSerializer& serializer)
    {
        serializer.setSender(_sender);
        serializer.setRecipient(_recipient);
        serializer.setHeaders(_headers);
        serializer.setTtlMs(static_cast<std::int64_t>(_ttlMs));
        serializer.setTtlAbsolute(_ttlAbsolute);
        serializer.setBody(smrf::ByteArrayView(_body));
        serializer.setCompressed(_isCompressed);
    }

    void check(const smrf::MessageDeserializer& deserializer)
    {
        EXPECT_EQ(_sender, deserializer.getSender());
        EXPECT_EQ(_recipient, deserializer.getRecipient());
        EXPECT_EQ(_headers, deserializer.getHeaders());
        EXPECT_EQ(_ttlMs, deserializer.getTtlMs());
        EXPECT_EQ(_ttlAbsolute, deserializer.isTtlAbsolute());
        EXPECT_EQ(_isCompressed, deserializer.isCompressed());

        if (!_isCompressed) {
            smrf::ByteArrayView deserializedBodyView = deserializer.getBody();
            ASSERT_EQ(_body.size(), deserializedBodyView.size());
            // element-wise comparison
            for (std::size_t i = 0; i < _body.size(); ++i) {
                EXPECT_EQ(_body[i], *(deserializedBodyView.data() + i));
            }
        } else {
            EXPECT_EQ(_body, deserializer.decompressBody());
        }
    }

private:
    std::string _sender;
    std::string _recipient;
    std::uint64_t _ttlMs;
    bool _ttlAbsolute;
    smrf::ByteVector _body;
    bool _isCompressed;
    std::unordered_map<std::string, std::string> _headers;
};

#endif // ROUNDTRIPMESSAGE_H
