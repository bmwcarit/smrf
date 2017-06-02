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
package io.joynr.smrf;

import java.nio.ByteBuffer;
import java.security.PrivateKey;
import java.security.cert.X509Certificate;
import java.util.HashMap;
import java.util.Map;
import java.lang.UnsupportedOperationException;

import smrf.Header;
import smrf.Message;

public final class MessageDeserializerImpl implements MessageDeserializer {

    private MessagePrefix messagePrefix;
    private Message message;
    private String sender;
    private String recipient;
    private Map<String, String> headers;

    public MessageDeserializerImpl(byte[] serializedMessage) throws EncodingException, UnsuppportedVersionException {
        messagePrefix = new MessagePrefix(serializedMessage);
        if (messagePrefix.version != MessagePrefix.VERSION) {
            throw new UnsuppportedVersionException(messagePrefix.version);
        }

        final int expectedMessageSize = MessagePrefix.SIZE + messagePrefix.msgSize + messagePrefix.sigSize;
        if (serializedMessage.length < expectedMessageSize) {
            throw new EncodingException("message size is wrong");
        }

        ByteBuffer byteBuffer = ByteBuffer.wrap(serializedMessage, MessagePrefix.SIZE, messagePrefix.msgSize);
        message = Message.getRootAsMessage(byteBuffer);
    }

    public byte[] decryptBody(PrivateKey key) throws SecurityException {
        throw new UnsupportedOperationException("Not implemented");
    }

    public CertificateIdentifier getEncryptionCertificateIdentifier() {
        throw new UnsupportedOperationException("Not implemented");
    }

    public String getHeader(String key) {
        Map<String, String> headers = getHeaders();
        return headers.get(key);
    }

    public Map<String, String> getHeaders() {
        if (headers == null) {
            headers = new HashMap<>();
            for (int i = 0; i < message.headersLength(); i++) {
                Header header = message.headers(i);
                headers.put(header.key(), header.value());
            }
        }
        return headers;
    }

    public String getRecipient() {
        if (recipient == null) {
            recipient = message.recipient();
        }
        return recipient;
    }

    public String getSender() {
        if (sender == null) {
            sender = message.sender();
        }
        return sender;
    }

    public CertificateIdentifier getSignerCertificateIdentifier() {
        throw new UnsupportedOperationException("Not implemented");
    }

    public long getTtlMs() {
        return message.ttlMs();
    }

    public byte[] getUnencryptedBody() throws EncodingException {
        if (message.isEncrypted()) {
            throw new EncodingException("message is encrypted");
        }
        ByteBuffer bodyBuffer = message.bodyAsByteBuffer();
        // TODO we could avoid a copy if we return a ByteBuffer instead
        byte[] body = new byte[bodyBuffer.remaining()];
        bodyBuffer.get(body);
        if (message.isCompressed()) {
            return ZlibCompression.decompress(body);
        }
        return body;
    }

    public boolean isCompressed() {
        return message.isCompressed();
    }

    public boolean isCustomSigned() {
        return message.isCustomSigned();
    }

    public boolean isEncrypted() {
        return message.isEncrypted();
    }

    public boolean isSigned() {
        return message.isSigned();
    }

    public boolean isTtlAbsolute() {
        return message.ttlAbsolute();
    }

    public int getMessageSize() {
        return MessagePrefix.SIZE + messagePrefix.msgSize + messagePrefix.sigSize;
    }

    public boolean verifySignature(X509Certificate cert) {
        throw new UnsupportedOperationException("Not implemented");
    }
}
