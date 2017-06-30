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
import java.nio.ByteOrder;
import java.security.PrivateKey;
import java.security.cert.X509Certificate;
import java.util.Map;
import java.util.Map.Entry;
import java.util.HashMap;
import java.lang.UnsupportedOperationException;

import com.google.flatbuffers.FlatBufferBuilder;

import io.joynr.smrf.MessageSerializer.SigningFunction;
import smrf.Header;
import smrf.Message;

public final class MessageSerializerImpl implements MessageSerializer {

    private final FlatBufferBuilder flatBuffersBuilder = new FlatBufferBuilder();

    private X509Certificate signingCertificate;
    private PrivateKey signingKey;
    private X509Certificate encryptionCertificate;
    private boolean isCompressed = false;
    private boolean ttlAbsolute = true;
    private String sender = "";
    private String recipient = "";
    private long ttlMs;
    private Map<String, String> headers = new HashMap<String, String>();
    private byte[] body = new byte[0];
    private boolean isCustomSigned = false;
    private SigningFunction customSigningCallback;

    public MessageSerializer encryptFor(X509Certificate cert) {
        this.encryptionCertificate = cert;
        return this;
    }

    public byte[] serialize() throws SecurityException, EncodingException {

        final boolean isEncrypted = encryptionCertificate != null;
        final boolean isSigned = signingCertificate != null && signingKey != null;

        final int[] headerOffsets = new int[headers.size()];
        int i = 0;
        for (Entry<String, String> entry : headers.entrySet()) {
            final int keyOffset = flatBuffersBuilder.createString(entry.getKey());
            final int valueOffset = flatBuffersBuilder.createString(entry.getValue());

            headerOffsets[i++] = Header.createHeader(flatBuffersBuilder, keyOffset, valueOffset);
        }
        final int headersOffset = flatBuffersBuilder.createSortedVectorOfTables(new Header(), headerOffsets);

        int bodyOffset = 0;
        if (!isEncrypted) {
            if (!isCompressed) {
                bodyOffset = flatBuffersBuilder.createByteVector(body);
            } else {
                bodyOffset = flatBuffersBuilder.createByteVector(ZlibCompression.compress(body));
            }
        } else {
            // encrypt
            if (isCompressed) {
                // generated CMS CompressedData
                // TODO
                throw new UnsupportedOperationException("Encryption with compression is not supported");
            } else {
                // TODO
                throw new UnsupportedOperationException("Encryption is not supported");
            }
        }

        final int recipientOffset = flatBuffersBuilder.createString(recipient);
        final int senderOffset = flatBuffersBuilder.createString(sender);
        final int messageOffset = Message.createMessage(flatBuffersBuilder,
                                                        recipientOffset,
                                                        senderOffset,
                                                        ttlMs,
                                                        ttlAbsolute,
                                                        isSigned,
                                                        isEncrypted,
                                                        isCompressed,
                                                        headersOffset,
                                                        bodyOffset,
                                                        isCustomSigned);

        flatBuffersBuilder.finish(messageOffset);
        final ByteBuffer messageBuffer = flatBuffersBuilder.dataBuffer();

        byte[] signature = null;
        short sigSize = 0;
        if (isCustomSigned) {
            ByteBuffer msgBuffer = flatBuffersBuilder.dataBuffer();
            signature = customSigningCallback.fn(msgBuffer);
        } else if (isSigned) {
            // TODO
            throw new UnsupportedOperationException("Only custom signatures are supported at the moment");
        }

        if (signature != null) {
            if (signature.length > Short.MAX_VALUE) {
                throw new EncodingException("signature size too large");
            }
            sigSize = (short)signature.length;
        }

        final int flatbufferSize = messageBuffer.remaining();
        final int finalSize = MessagePrefix.SIZE + flatbufferSize + sigSize;

        MessagePrefix messagePrefix = new MessagePrefix();
        messagePrefix.sigSize = sigSize;
        messagePrefix.msgSize = flatbufferSize;
        final ByteBuffer finalBuffer = ByteBuffer.allocate(finalSize).order(ByteOrder.LITTLE_ENDIAN);
        messagePrefix.writeToPreallocatedBuffer(finalBuffer);
        finalBuffer.put(messageBuffer);
        if (signature != null) {
            finalBuffer.put(signature);
        }

        return finalBuffer.array();
    }

    public MessageSerializer setBody(byte[] body) {
        this.body = body;
        return this;
    }

    public MessageSerializer setCompressed(boolean isCompressed) {
        this.isCompressed = isCompressed;
        return this;
    }

    public MessageSerializer setHeaders(Map<String, String> headers) {
        this.headers = headers;
        return this;
    }

    public MessageSerializer setRecipient(String recipient) {
        this.recipient = recipient;
        return this;
    }

    public MessageSerializer setSender(String sender) {
        this.sender = sender;
        return this;
    }

    public MessageSerializer setTtlMs(long ttlMs) {
        this.ttlMs = ttlMs;
        return this;
    }

    public MessageSerializer setTtlAbsolute(boolean ttlAbsolute) {
        this.ttlAbsolute = ttlAbsolute;
        return this;
    }

    public MessageSerializer signWith(X509Certificate cert, PrivateKey key) {
        this.signingCertificate = cert;
        this.signingKey = key;
        return this;
    }

    public void setCustomSigningCallback(SigningFunction signingCallback) {
        isCustomSigned = true;
        customSigningCallback = signingCallback;
    }
}
