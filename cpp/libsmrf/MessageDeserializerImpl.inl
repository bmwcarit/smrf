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

#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>

#include <flatbuffers/flatbuffers.h>
#include <smrf_generated.h>

#include "smrf/ByteArrayView.h"
#include "smrf/ByteVector.h"
#include "smrf/Certificate.h"
#include "smrf/PrivateKey.h"
#include "smrf/exceptions.h"

#include "MessagePrefix.h"
#include "compression.h"

#include <iostream>

namespace smrf
{
class MessageDeserializerImpl
{
public:
    explicit MessageDeserializerImpl(const ByteArrayView& serializedMessage, bool verifyInput)
            : serializedMessage(serializedMessage), messagePrefix(this->serializedMessage), message(nullptr)
    {
        init(verifyInput);
    }

    explicit MessageDeserializerImpl(ByteArrayView&& serializedMessage, bool verifyInput)
            : serializedMessage(std::move(serializedMessage)), messagePrefix(this->serializedMessage), message(nullptr)
    {
        init(verifyInput);
    }

    bool isEncrypted() const
    {
        return message->isEncrypted();
    }

    std::uint64_t getMessageSize() const
    {
        return MessagePrefix::SIZE + messagePrefix.msgSize + messagePrefix.sigSize;
    }

    bool isSigned() const
    {
        return message->isSigned();
    }

    bool isCompressed() const
    {
        return message->isCompressed();
    }

    bool isCustomSigned() const
    {
        return message->isCustomSigned();
    }

    std::string getSender() const
    {
        return getString(message->sender());
    }

    ByteArrayView getRawSender() const
    {
        return getRawString(message->sender());
    }

    std::string getRecipient() const
    {
        return getString(message->recipient());
    }

    ByteArrayView getRawRecipient() const
    {
        return getRawString(message->recipient());
    }

    std::int64_t getTtlMs() const
    {
        return message->ttlMs();
    }

    bool isTtlAbsolute() const
    {
        return message->ttlAbsolute();
    }

    std::unordered_map<std::string, std::string> getHeaders() const
    {
        std::unordered_map<std::string, std::string> headers;
        const flatbuffers::Vector<flatbuffers::Offset<Header>>* flatbuffersHeaders = message->headers();
        if (flatbuffersHeaders != nullptr) {
            headers.reserve(flatbuffersHeaders->size());
            for (const Header* entry : *flatbuffersHeaders) {
                // only insert entries with non-empty key and value
                const flatbuffers::String* key = entry->key();
                const flatbuffers::String* value = entry->value();
                if (key && value) {
                    headers.insert({key->str(), value->str()});
                }
            }
        }
        return headers;
    }

    std::vector<std::pair<ByteArrayView, ByteArrayView>> getRawHeaders() const
    {
        std::vector<std::pair<ByteArrayView, ByteArrayView>> rawHeaders;
        const flatbuffers::Vector<flatbuffers::Offset<Header>>* flatbuffersHeaders = message->headers();
        if (flatbuffersHeaders != nullptr) {
            rawHeaders.reserve(flatbuffersHeaders->size());
            for (const Header* entry : *flatbuffersHeaders) {
                // only insert entries with non-empty key and value
                const flatbuffers::String* key = entry->key();
                const flatbuffers::String* value = entry->value();
                if (key && value) {
                    rawHeaders.push_back(
                            {ByteArrayView(key->Data(), key->size()), ByteArrayView(value->Data(), value->size())});
                }
            }
        }
        return rawHeaders;
    }

    const ByteArrayView getBody() const
    {
        if (message->body()) {
            return ByteArrayView(message->body()->Data(), message->body()->size());
        }
        return ByteArrayView();
    }

    ByteVector decompressBody() const
    {
        if (isEncrypted()) {
            throw EncodingException("message is encrypted");
        }
        if (!isCompressed()) {
            throw EncodingException("message is not compressed");
        }
        if (message->body()) {
            return zlib::decompress(ByteArrayView(message->body()->Data(), message->body()->size()));
        }
        return ByteVector();
    }

    ByteArrayView getSignature() const
    {
        if (message->isCustomSigned() || message->isSigned()) {
            const size_t signatureOffset = MessagePrefix::SIZE + messagePrefix.msgSize;
            const Byte* beginningOfSignature = serializedMessage.data() + signatureOffset;
            return ByteArrayView(beginningOfSignature, messagePrefix.sigSize);
        } else {
            throw EncodingException("message has no signature");
        }
    }

private:
    // get the value or a default constructed string if value is not set
    std::string getString(const flatbuffers::String* flatbuffersString) const
    {
        if (flatbuffersString == nullptr) {
            return std::string();
        } else {
            return flatbuffersString->str();
        }
    }

    ByteArrayView getRawString(const flatbuffers::String* flatbuffersString) const
    {
        if (flatbuffersString == nullptr) {
            return ByteArrayView();
        } else {
            return ByteArrayView(flatbuffersString->Data(), flatbuffersString->size());
        }
    }

    void init(bool verifyInput)
    {
        const std::uint8_t* msgStart = serializedMessage.data() + MessagePrefix::SIZE;

        if (verifyInput) {
            if (messagePrefix.version != MessagePrefix::VERSION) {
                throw EncodingException("wrong version");
            }

            const std::size_t expectedMessageSize = MessagePrefix::SIZE + messagePrefix.msgSize + messagePrefix.sigSize;
            if (serializedMessage.size() < expectedMessageSize) {
                throw EncodingException("wrong size");
            }

            flatbuffers::Verifier verifier(msgStart, messagePrefix.msgSize);
            const bool verified = VerifyMessageBuffer(verifier);
            if (!verified) {
                throw EncodingException("wrong encoding");
            }
        }

        message = GetMessage(msgStart);
    }

    const ByteArrayView serializedMessage;
    const MessagePrefix messagePrefix;
    const Message* message;
};
} // namespace smrf
