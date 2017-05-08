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

#ifndef SMRF_MESSAGEDESERIALIZER_H
#define SMRF_MESSAGEDESERIALIZER_H

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include "smrf/ByteArrayView.h"
#include "smrf/ByteVector.h"

namespace smrf
{

class CertificateIdentifier;
class Certificate;
class PrivateKey;
class MessageDeserializerImpl;

/* @brief Deserialize SMRF messages from wire-format
 *
 * A new MessageDeserializer is initialized with the SMRF message in wire-format.
 * The message can then be tested if it was signed and/or encrypted. If the message
 * is signed or encrypted, the CertificateIdentifier of the certificate that was used
 * for this action can be retrieved with the getSignerCertId or the getEncryptionCertId
 * method, respectively.
 *
 * If the message is signed, the signature should be validated before any other operation
 * on the message is executed. This is done by the verifySignature method that takes the
 * certificate to be used for this operation.
 *
 * The MessageDeserializer also provides methods to access the other SMRF header fields.
 * If the message is encrypted, the message body cannot be retrieved directly but must be
 * decrypted beforehand. This is implemented in the decryptBody method.
 */
class MessageDeserializer final
{
public:
    /**
     * @brief Instantiate the MessageDeserializer from a vector of bytes
     *
     * @parameter serializedMessage A ByteArrayView containing a message in SMRF wire format
     * @parameter verifyInput if true, input will be verified prior to deserialization
     * @throw EncodingException if the format of the message is invalid.
     */
    explicit MessageDeserializer(const ByteArrayView& serializedMessage, bool verifyInput = true);
    explicit MessageDeserializer(ByteArrayView&& serializedMessage, bool verifyInput = true);

    ~MessageDeserializer();

    /**
     * @brief Check if the given message is encrypted
     *
     * @return true if the message is encrypted. False otherwise.
     */
    bool isEncrypted() const;

    /**
     * @brief Returns the size of the serialized message
     *
     * @return The size of the serialized message in byte
     */
    std::uint64_t getMessageSize() const;

    /**
     * @brief Check if the given message is signed
     *
     * @return true if the message is signed. False otherwise.
     */
    bool isSigned() const;

    /**
     * @brief Check if the given message is compressed
     *
     * @return true if the message is compressed. False otherwise.
     */
    bool isCompressed() const;

    /**
     * @brief Get the CertificateIdentifier of the certificate used to sign this message
     *
     *
     * @return The CertificateIdentifier of the certificate used to sign this message
     * @throws SecurityException if the information cannot be retrieved from the signature.
     */
    CertificateIdentifier getSignerCertId() const;

    /**
     * @brief Get the CertificateIdentifier of the certificate used to encrypt this message
     *
     *
     * @return The CertificateIdentifier of the certificate used to encrypt this message
     * @throws SecurityException if the information cannot be retrieved from the encryption
                                 container.
               EncodingException if this message is not encrypted.
     */
    CertificateIdentifier getEncryptionCertId() const;

    /* @brief Get sender field from SMRF message
     *
     * @return The sender field of the SMRF header
     */
    std::string getSender() const;
    ByteArrayView getRawSender() const;

    /* @brief Get receiver field from SMRF message
     *
     * @return The recipient field of the SMRF header
     */
    std::string getRecipient() const;
    ByteArrayView getRawRecipient() const;

    /* @brief Get TTL from SMRF message
     *
     * @return The TTL field of the SMRF header
     */
    std::int64_t getTtlMs() const;

    /* @brief Get ttlAbsolute from SMRF message
     *
     * @return true if TTL is absolute, false if TTL is relative
     */
    bool isTtlAbsolute() const;

    /* @brief Get additional headers map
     *
     * @return The additional header information of the SMRF message
     */
    std::unordered_map<std::string, std::string> getHeaders() const;
    std::vector<std::pair<ByteArrayView, ByteArrayView>> getRawHeaders() const;

    /**
     * @brief Verify the signature of the message.
     *
     * This function can only be called on signed messages (test with isSigned).
     * The signature certificate is passed in as a parameter.
     *
     * If the signature verification can be carried out, but detects an invalid
     * signature, this method returns false.
     * However, if signature validation fails this is reported via an exception.
     * Examples include: Trying to verify the signature of an unsigned message,
     * providing an invalid certificate (as indicated by the Certificate::isValid()
     * method).
     *
     * @return true if the signature is valid. False otherwise.
     * @throw SecurityException if the message signature cannot be verified.
     * @throw EncodingException if the message was not signed.
     */
    bool verifySignature(std::shared_ptr<const Certificate> cert) const;

    /* @brief Get SMRF message body
     *
     * @return The message body
     */
    const ByteArrayView getBody() const;

    /* @brief Get SMRF message body
     *
     * @return The message body
     * @throw EncodingException if the message is encrypted or not compressed
     */
    ByteVector decompressBody() const;

    /**
     * @brief Decrypt the body of an encrypted SMRF message
     *
     * Decrypts (and possibly decompresses) the encrypted body of a SMRF message.
     * Moreover, the MAC on the encrypted message part is validated.
     *
     * @param key A std::shared_ptr to the key to use for decryption.
     * @returns The decrypted message body.
     * @throw SecurityException if an encrypted message cannot be decrypted,
                                or if the MAC validation on the encrypted payload fails.
     *        EncodingException if some other error occurs during deserialization
     *                          (e.g., while uncompressing of compressed messages)
     */
    ByteVector decryptBody(std::shared_ptr<const PrivateKey> key) const;

private:
    std::unique_ptr<MessageDeserializerImpl> pImpl;
};

} // namespace smrf

#endif // SMRF_MESSAGEDESERIALIZER_H
