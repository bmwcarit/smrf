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

#include "smrf/MessageSerializer.h"

#include "MessageSerializerImpl.inl"

namespace smrf
{

MessageSerializer::MessageSerializer() : _pImpl(std::make_unique<MessageSerializerImpl>())
{
}

MessageSerializer::~MessageSerializer() = default;

MessageSerializer& MessageSerializer::setSender(const std::string& sender)
{
    _pImpl->setSender(sender);
    return *this;
}

MessageSerializer& MessageSerializer::setRecipient(const std::string& recipient)
{
    _pImpl->setRecipient(recipient);
    return *this;
}

MessageSerializer& MessageSerializer::setTtlMs(std::int64_t ttlMs)
{
    _pImpl->setTtlMs(ttlMs);
    return *this;
}

MessageSerializer& MessageSerializer::setTtlAbsolute(bool ttlAbsolute)
{
    _pImpl->setTtlAbsolute(ttlAbsolute);
    return *this;
}

MessageSerializer& MessageSerializer::setCompressed(bool isCompressed)
{
    _pImpl->setCompressed(isCompressed);
    return *this;
}

MessageSerializer& MessageSerializer::setHeaders(const std::unordered_map<std::string, std::string>& headerMap)
{
    _pImpl->setHeaders(headerMap);
    return *this;
}

MessageSerializer& MessageSerializer::setBody(const ByteArrayView& body)
{
    _pImpl->setBody(body);
    return *this;
}

MessageSerializer& MessageSerializer::signWith(std::shared_ptr<const Certificate> cert, std::shared_ptr<const PrivateKey> key)
{
    _pImpl->signWith(std::move(cert), std::move(key));
    return *this;
}

MessageSerializer& MessageSerializer::encryptFor(std::shared_ptr<const Certificate> cert)
{
    _pImpl->encryptFor(std::move(cert));
    return *this;
}

ByteVector MessageSerializer::serialize()
{
    return _pImpl->serialize();
}

void MessageSerializer::setCustomSigningCallback(std::function<ByteVector(const ByteArrayView&)> signingCallback)
{
    _pImpl->setCustomSigningCallback(std::move(signingCallback));
}

} // namespace smrf
