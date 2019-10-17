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
            : _flatBuffersBuilder(),
              _sender(),
              _recipient(),
              _headers(),
              _body(),
              _signingCertificate(),
              _signingKey(),
              _encryptionCertificate()
    {
    }

    void setSender(const std::string& sender)
    {
        this->_sender = _flatBuffersBuilder.CreateString(sender);
    }

    void setRecipient(const std::string& recipient)
    {
        this->_recipient = _flatBuffersBuilder.CreateString(recipient);
    }

    void setTtlMs(std::int64_t ttlMs)
    {
        this->_ttlMs = ttlMs;
    }

    void setTtlAbsolute(bool ttlAbsolute)
    {
        this->_ttlAbsolute = ttlAbsolute;
    }

    void setCompressed(bool isCompressed)
    {
        this->_isCompressed = isCompressed;
    }

    void setHeaders(const std::unordered_map<std::string, std::string>& headerMap)
    {
        std::vector<flatbuffers::Offset<Header>> headerVec;
        headerVec.reserve(headerMap.size());

        for (const auto& entry : headerMap) {
            auto key = _flatBuffersBuilder.CreateString(entry.first);
            auto value = _flatBuffersBuilder.CreateString(entry.second);
            auto header = CreateHeader(_flatBuffersBuilder, key, value);
            headerVec.push_back(std::move(header));
        }
        this->_headers = _flatBuffersBuilder.CreateVectorOfSortedTables(&headerVec);
    }

    void setBody(const ByteArrayView& body)
    {
        this->_body = body;
    }

    void signWith(std::shared_ptr<const Certificate> cert, std::shared_ptr<const PrivateKey> key)
    {
        assert(cert);
        assert(key);

        this->_signingCertificate = std::move(cert);
        this->_signingKey = std::move(key);
    }

    void encryptFor(std::shared_ptr<const Certificate> cert)
    {
        assert(cert);
        this->_encryptionCertificate = std::move(cert);
    }

    void setCustomSigningCallback(std::function<ByteVector(const ByteArrayView&)> signingCallback)
    {
        assert(signingCallback);
        _isCustomSigned = true;
        _customSigningCallback = signingCallback;
    }

    ByteVector serialize()
    {
        const bool isEncrypted(_encryptionCertificate);
        const bool isSigned(_signingCertificate && _signingKey);
        std::uint16_t sigSize = 0;

        flatbuffers::Offset<flatbuffers::Vector<ByteVector::value_type>> flatbuffersBody;
        if (_body.size() > 0) {
            if (!isEncrypted) {
                if (!_isCompressed) {
                    flatbuffersBody = _flatBuffersBuilder.CreateVector(_body.data(), _body.size());
                } else {
                    // compress plaintext body
                    flatbuffersBody = _flatBuffersBuilder.CreateVector(zlib::compress(_body));
                }
            } else {
                // encrypt using OpenSSL
                if (_isCompressed) {
                    // generated CMS CompressedData
                } else {
                }
            }
        }

        flatbuffers::Offset<Message> message = CreateMessage(_flatBuffersBuilder,
                                                             _recipient,
                                                             _sender,
                                                             _ttlMs,
                                                             _ttlAbsolute,
                                                             isSigned,
                                                             isEncrypted,
                                                             _isCompressed,
                                                             _headers,
                                                             flatbuffersBody,
                                                             _isCustomSigned);

        _flatBuffersBuilder.Finish(message);

        const std::size_t flatbufferSize = _flatBuffersBuilder.GetSize();
        ByteVector signature;
        if (_isCustomSigned) {
            auto beginingBufPtr = _flatBuffersBuilder.GetBufferPointer();
            signature = _customSigningCallback(ByteArrayView(beginingBufPtr, flatbufferSize));
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
        std::memcpy(serializedMessage.data() + flatbuffersOffset, _flatBuffersBuilder.GetBufferPointer(), flatbufferSize);
        std::memcpy(serializedMessage.data() + signatureOffset, signature.data(), signature.size());
        return serializedMessage;
    }

    using CustomSigningCallback = std::function<ByteVector(const ByteArrayView&)>;
    CustomSigningCallback _customSigningCallback;
    bool _isCustomSigned = false;

private:
    flatbuffers::FlatBufferBuilder _flatBuffersBuilder;
    flatbuffers::Offset<flatbuffers::String> _sender;
    flatbuffers::Offset<flatbuffers::String> _recipient;
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Header>>> _headers;
    ByteArrayView _body;
    std::shared_ptr<const Certificate> _signingCertificate;
    std::shared_ptr<const PrivateKey> _signingKey;
    std::shared_ptr<const Certificate> _encryptionCertificate;
    std::int64_t _ttlMs = 0;
    bool _ttlAbsolute = true;
    bool _isCompressed = false;
};
} // namespace smrf
