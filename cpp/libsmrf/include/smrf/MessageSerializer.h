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

#ifndef SMRF_MESSAGESERIALIZER_H
#define SMRF_MESSAGESERIALIZER_H

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "smrf/ByteArrayView.h"
#include "smrf/ByteVector.h"

namespace smrf
{

class Certificate;
class PrivateKey;
class MessageSerializerImpl;

/**
 * @brief Encode a message to SMRF wire format
 *
 * The MessageSerializer class provides all functionality to prepare a message for transmitting
 * on the wire.
 * Creating a new object creates an empty message. This message can be filled by using the
 * given setter methods. Signature and/or encryption are implictly controlled by calling the
 * signWith or the encryptFor method.
 * Both these function calls are optional. If the call was made, the encoded message will be
 * signed or encrypted, respectively. If the message is both, signed and encrypted, it will be
 * encrypted first and then signed.
 *
 * The serialized results of this class can be deserialized with the
 * @see MessageDeserializer class.
 */
class MessageSerializer final
{
public:
    /*
     * @brief Create an (empty) MessageSerializer object
     *
     */
    MessageSerializer();
    ~MessageSerializer();

    /*
     * @brief Set the sender of this message
     *
     * This is required for every SMRF message.
     *
     * @param sender The sender to be set in the SMRF header
     * @return returns *this
     */
    MessageSerializer& setSender(const std::string& sender);

    /*
     * @brief Set the receiver of this message
     *
     * This is required for every SMRF message.
     *
     * @param recipient The recipient to be set in the SMRF header
     * @return returns *this
     */
    MessageSerializer& setRecipient(const std::string& recipient);

    /*
     * @brief Set the time to live of this message
     *
     * This is required for every SMRF message.
     *
     * @param validUntil The absolute time until this message should be valid in
     *                   milliseconds since the epoch (unixtime).
     * @return returns *this
     */
    MessageSerializer& setTtlMs(std::int64_t ttlMs);

    /*
     * @brief Set scope of time to live for this message
     *
     * Indicate whether time to live is an absolute or an relative value.
     * The default is absolute (applies when the method is never called).
     *
     * @return returns *this
     */
    MessageSerializer& setTtlAbsolute(bool absolute);

    /*
     * @brief Set compression for this message
     *
     * Indicate whether the serialized message should be compressed or not.
     * The default is no compression (applies when the method is never called).
     *
     * @return returns *this
     */
    MessageSerializer& setCompressed(bool isCompressed);

    /*
     * @brief Set the additional SMRF headers
     *
     * If this function is not called, the additional headers in the serialized SMRF
     * message will be empty.
     *
     * @param headers A hashmap containing all additional headers.
     * @return returns *this
     */
    MessageSerializer& setHeaders(const std::unordered_map<std::string, std::string>& headerMap);

    /*
     * @brief Set the SMRF message body
     *
     * This is required for every SMRF message.
     *
     * @param body The body of the SMRF message
     * @return returns *this
     */
    MessageSerializer& setBody(const ByteArrayView& body);

    /*
     * @brief Setup the signature that will be used for this SMRF message
     *
     * If this function is called, the given certificate and private key will be stored and
     * used in the encode function to sign the given message. Calling this function produces
     * a signed SMRF message and sets the isSigned header to true.
     *
     * If this function is not called, the serialized SMRF message will be not signed.
     *
     * @param cert A shared pointer to a certificate that should be used for signing.
     * @param key A shared pointer to a private crypto key that belongs to the given certificate.
     * @return returns *this
     */
    MessageSerializer& signWith(std::shared_ptr<const Certificate> cert, std::shared_ptr<const PrivateKey> key);

    /*
     * @brief Setup the encryption that will be used for this SMRF message
     *
     * If this function is called, the public key of the given certificate will be stored
     * and used in the encode function to encrypt the given message. Calling this function
     * produces an encrypted SMRF message and sets the isEncrypted header to true
     *
     * If this function is not called, the serialized SMRF message will not be encrypted.
     *
     * @param key A shared pointer to a certificate that belongs to the message receiver.
     * @return returns *this
     */
    MessageSerializer& encryptFor(std::shared_ptr<const Certificate> cert);

    /*
     * @brief Produce a message in SMRF wire format
     *
     * Serializes the message to SMRF wire format. Encrypts and/or signs the given message.
     * according to the previous configuration. Multple calls to this function are possible.
     * It is also possible to change some fields in between. Every call will trigger a new
     * serialization with all necessary cryptographic operations (sign, encrypt).
     *
     * @see MessageDeserializer
     *
     * @return ByteVector that contains the wrapped and serialized message.
     * @throw SecurityException if encryption or signing of the message fails
     * @throw EncodingException if a non-security related problem occurs when encoding the
     *                          SMRF message.
     */
    ByteVector serialize();

    /*
     * @brief Set a custom signature algorithm for smrf message
     *
     * If this function is called, the supplied callback will be used to get the message signature
     * instead of using SMRF's default signature.
     * If this function is not called, the serialized SMRF message will have smrf default
     * signature
     * @param signingCallback A callback that is passed the serialized message and which returns the signature
     */
    void setCustomSigningCallback(std::function<ByteVector(const ByteArrayView&)> signingCallback);

private:
    std::unique_ptr<MessageSerializerImpl> _pImpl;
};

} // namespace smrf

#endif // SMRF_MESSAGESERIALIZER_H
