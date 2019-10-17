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

#include "smrf/MessageDeserializer.h"

#include "MessageDeserializerImpl.inl"

namespace smrf
{

MessageDeserializer::MessageDeserializer(const ByteArrayView& serializedMessage, bool verifyInput)
        : _pImpl(std::make_unique<MessageDeserializerImpl>(serializedMessage, verifyInput))
{
}

MessageDeserializer::MessageDeserializer(ByteArrayView&& serializedMessage, bool verifyInput)
        : _pImpl(std::make_unique<MessageDeserializerImpl>(std::move(serializedMessage), verifyInput))
{
}

MessageDeserializer::~MessageDeserializer() = default;

bool MessageDeserializer::isEncrypted() const
{
    return _pImpl->isEncrypted();
}

std::uint64_t MessageDeserializer::getMessageSize() const
{
    return _pImpl->getMessageSize();
}

bool MessageDeserializer::isSigned() const
{
    return _pImpl->isSigned();
}

bool MessageDeserializer::isCompressed() const
{
    return _pImpl->isCompressed();
}

bool MessageDeserializer::isCustomSigned() const
{
    return _pImpl->isCustomSigned();
}

std::string MessageDeserializer::getSender() const
{
    return _pImpl->getSender();
}

ByteArrayView MessageDeserializer::getRawSender() const
{
    return _pImpl->getRawSender();
}

std::string MessageDeserializer::getRecipient() const
{
    return _pImpl->getRecipient();
}

ByteArrayView MessageDeserializer::getRawRecipient() const
{
    return _pImpl->getRawRecipient();
}

std::int64_t MessageDeserializer::getTtlMs() const
{
    return _pImpl->getTtlMs();
}

bool MessageDeserializer::isTtlAbsolute() const
{
    return _pImpl->isTtlAbsolute();
}

std::unordered_map<std::string, std::string> MessageDeserializer::getHeaders() const
{
    return _pImpl->getHeaders();
}

std::vector<std::pair<ByteArrayView, ByteArrayView>> MessageDeserializer::getRawHeaders() const
{
    return _pImpl->getRawHeaders();
}

const ByteArrayView MessageDeserializer::getBody() const
{
    return _pImpl->getBody();
}

ByteVector MessageDeserializer::decompressBody() const
{
    return _pImpl->decompressBody();
}

ByteArrayView MessageDeserializer::getSignature() const
{
    return _pImpl->getSignature();
}

} // namespace smrf
