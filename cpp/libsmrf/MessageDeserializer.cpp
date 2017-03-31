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
        : pImpl(std::make_unique<MessageDeserializerImpl>(serializedMessage, verifyInput))
{
}

MessageDeserializer::MessageDeserializer(ByteArrayView&& serializedMessage, bool verifyInput)
        : pImpl(std::make_unique<MessageDeserializerImpl>(std::move(serializedMessage), verifyInput))
{
}

MessageDeserializer::~MessageDeserializer() = default;

bool MessageDeserializer::isEncrypted() const
{
    return pImpl->isEncrypted();
}

bool MessageDeserializer::isSigned() const
{
    return pImpl->isSigned();
}

bool MessageDeserializer::isCompressed() const
{
    return pImpl->isCompressed();
}

std::string MessageDeserializer::getSender() const
{
    return pImpl->getSender();
}

ByteArrayView MessageDeserializer::getRawSender() const
{
    return pImpl->getRawSender();
}

std::string MessageDeserializer::getRecipient() const
{
    return pImpl->getRecipient();
}

ByteArrayView MessageDeserializer::getRawRecipient() const
{
    return pImpl->getRawRecipient();
}

std::int64_t MessageDeserializer::getTtlMs() const
{
    return pImpl->getTtlMs();
}

bool MessageDeserializer::isTtlAbsolute() const
{
    return pImpl->isTtlAbsolute();
}

std::unordered_map<std::string, std::string> MessageDeserializer::getHeaders() const
{
    return pImpl->getHeaders();
}

std::vector<std::pair<ByteArrayView, ByteArrayView>> MessageDeserializer::getRawHeaders() const
{
    return pImpl->getRawHeaders();
}

const ByteArrayView MessageDeserializer::getBody() const
{
    return pImpl->getBody();
}

ByteVector MessageDeserializer::decompressBody() const
{
    return pImpl->decompressBody();
}

} // namespace smrf
