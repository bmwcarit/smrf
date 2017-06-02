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
#include <cstring>
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

namespace smrf
{
class MessageSerializerImpl
{
public:
    MessageSerializerImpl()
            : flatBuffersBuilder(),
              sender(),
              recipient(),
              headers(),
              body(),
              signingCertificate(),
              signingKey(),
              encryptionCertificate()
    {
    }

    void setSender(const std::string& sender)
    {
        this->sender = flatBuffersBuilder.CreateString(sender);
    }

    void setRecipient(const std::string& recipient)
    {
        this->recipient = flatBuffersBuilder.CreateString(recipient);
    }

    void setTtlMs(std::int64_t ttlMs)
    {
        this->ttlMs = ttlMs;
    }

    void setTtlAbsolute(bool ttlAbsolute)
    {
        this->ttlAbsolute = ttlAbsolute;
    }

    void setCompressed(bool isCompressed)
    {
        this->isCompressed = isCompressed;
    }

    void setHeaders(const std::unordered_map<std::string, std::string>& headerMap)
    {
        std::vector<flatbuffers::Offset<Header>> headerVec;

        for (const auto& entry : headerMap) {
            auto key = flatBuffersBuilder.CreateString(entry.first);
            auto value = flatBuffersBuilder.CreateString(entry.second);
            auto header = CreateHeader(flatBuffersBuilder, key, value);
            headerVec.push_back(std::move(header));
        }
        this->headers = flatBuffersBuilder.CreateVectorOfSortedTables(&headerVec);
    }

    void setBody(const ByteArrayView& body)
    {
        this->body = body;
    }

    void signWith(std::shared_ptr<const Certificate> cert, std::shared_ptr<const PrivateKey> key)
    {
        assert(cert);
        assert(key);

        this->signingCertificate = std::move(cert);
        this->signingKey = std::move(key);
    }

    void encryptFor(std::shared_ptr<const Certificate> cert)
    {
        assert(cert);
        this->encryptionCertificate = std::move(cert);
    }

    void setCustomSigningCallback(std::function<ByteVector(const ByteArrayView&)> signingCallback)
    {
        assert(signingCallback);
        isCustomSigned = true;
        customSigningCallback = signingCallback;
    }

    ByteVector serialize()
    {
        const bool isEncrypted(encryptionCertificate);
        const bool isSigned(signingCertificate && signingKey);
        std::uint32_t sigSize = 0;

        flatbuffers::Offset<flatbuffers::Vector<ByteVector::value_type>> flatbuffersBody;
        if (body.size() > 0) {
            if (!isEncrypted) {
                if (!isCompressed) {
                    flatbuffersBody = flatBuffersBuilder.CreateVector(body.data(), body.size());
                } else {
                    // compress plaintext body
                    flatbuffersBody = flatBuffersBuilder.CreateVector(zlib::compress(body));
                }
            } else {
                // encrypt using OpenSSL
                if (isCompressed) {
                    // generated CMS CompressedData
                } else {
                }
            }
        }

        flatbuffers::Offset<Message> message =
                CreateMessage(flatBuffersBuilder, recipient, sender, ttlMs, ttlAbsolute, isSigned, isEncrypted, isCompressed,
                              headers, flatbuffersBody, isCustomSigned);

        flatBuffersBuilder.Finish(message);

        const std::size_t flatbufferSize = flatBuffersBuilder.GetSize();
        ByteVector signature;
        if (isCustomSigned) {
            auto beginingBufPtr = flatBuffersBuilder.GetBufferPointer();
            signature = customSigningCallback(ByteArrayView(beginingBufPtr, flatbufferSize));
        } else if (isSigned) {
        }

        sigSize = signature.size();
        std::size_t finalSize = MessagePrefix::SIZE + flatbufferSize + sigSize;
        ByteVector serializedMessage(finalSize);
        MessagePrefix messagePrefix;
        messagePrefix.sigSize = sigSize;
        messagePrefix.msgSize = flatbufferSize;
        messagePrefix.writeToPreallocatedBuffer(serializedMessage);
        const std::size_t flatbuffersOffset = MessagePrefix::SIZE;
        const std::size_t signatureOffset = flatbuffersOffset + flatbufferSize;
        std::memcpy(serializedMessage.data() + flatbuffersOffset, flatBuffersBuilder.GetBufferPointer(), flatbufferSize);
        std::memcpy(serializedMessage.data() + signatureOffset, signature.data(), signature.size());
        return serializedMessage;
    }

    using CustomSigningCallback = std::function<ByteVector(const ByteArrayView&)>;
    CustomSigningCallback customSigningCallback;
    bool isCustomSigned = false;

private:
    flatbuffers::FlatBufferBuilder flatBuffersBuilder;
    flatbuffers::Offset<flatbuffers::String> sender;
    flatbuffers::Offset<flatbuffers::String> recipient;
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Header>>> headers;
    ByteArrayView body;
    std::shared_ptr<const Certificate> signingCertificate;
    std::shared_ptr<const PrivateKey> signingKey;
    std::shared_ptr<const Certificate> encryptionCertificate;
    std::int64_t ttlMs = 0;
    bool ttlAbsolute = true;
    bool isCompressed = false;
};
} // namespace smrf
