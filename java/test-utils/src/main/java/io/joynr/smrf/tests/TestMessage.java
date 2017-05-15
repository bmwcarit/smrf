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
package io.joynr.smrf.tests;

import java.util.Arrays;
import java.util.Map;

import com.google.common.collect.ImmutableMap;

import io.joynr.smrf.EncodingException;
import io.joynr.smrf.MessageDeserializer;
import io.joynr.smrf.MessageSerializer;

public class TestMessage {

    private final String sender;
    private final String recipient;
    private final Map<String, String> headers;
    private final long ttlMs;
    private final boolean ttlAbsolute;
    private final byte[] body;
    private final boolean isCompressed;

    public TestMessage(boolean shouldBeCompressed) {
        this.sender = "sender";
        this.recipient = "recipient";
        this.headers = ImmutableMap.of("key1", "value1", "key2", "value2");
        this.ttlMs = 1234567L;
        this.ttlAbsolute = false;

        final int bodySize = 100;
        this.body = new byte[bodySize];
        for (int i = 0; i < bodySize; ++i) {
            this.body[i] = (byte) i;
        }
        this.isCompressed = shouldBeCompressed;
    }

    public TestMessage(String sender,
                       String recipient,
                       Map<String, String> headers,
                       long ttlMs,
                       boolean ttlAbsolute,
                       byte[] body,
                       boolean isCompressed) {
        this.sender = sender;
        this.recipient = recipient;
        this.headers = headers;
        this.ttlMs = ttlMs;
        this.ttlAbsolute = ttlAbsolute;
        this.body = body;
        this.isCompressed = isCompressed;
    }

    public static TestMessage getFromDeserializer(MessageDeserializer deserializer) throws EncodingException {
        String sender = deserializer.getSender();
        String recipient = deserializer.getRecipient();
        Map<String, String> headers = deserializer.getHeaders();
        long ttlMs = deserializer.getTtlMs();
        boolean ttlAbsolute = deserializer.isTtlAbsolute();
        boolean isCompressed = deserializer.isCompressed();
        byte[] body = null;
        if (!deserializer.isEncrypted()) {
            body = deserializer.getUnencryptedBody();
        } else {
            throw new EncodingException("encryption not yet implemented");
        }
        return new TestMessage(sender, recipient, headers, ttlMs, ttlAbsolute, body, isCompressed);
    }

    public void initSerializer(MessageSerializer serializer) {
        serializer.setSender(sender);
        serializer.setRecipient(recipient);
        serializer.setHeaders(headers);
        serializer.setTtlMs(ttlMs);
        serializer.setTtlAbsolute(ttlAbsolute);
        serializer.setBody(body);
        serializer.setCompressed(isCompressed);
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + Arrays.hashCode(body);
        result = prime * result + ((headers == null) ? 0 : headers.hashCode());
        result = prime * result + (isCompressed ? 1231 : 1237);
        result = prime * result + ((recipient == null) ? 0 : recipient.hashCode());
        result = prime * result + ((sender == null) ? 0 : sender.hashCode());
        result = prime * result + (ttlAbsolute ? 1231 : 1237);
        result = prime * result + (int) (ttlMs ^ (ttlMs >>> 32));
        return result;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        TestMessage other = (TestMessage) obj;
        if (!Arrays.equals(body, other.body)) {
            return false;
        }
        if (headers == null) {
            if (other.headers != null) {
                return false;
            }
        } else if (!headers.equals(other.headers)) {
            return false;
        }
        if (isCompressed != other.isCompressed) {
            return false;
        }
        if (recipient == null) {
            if (other.recipient != null) {
                return false;
            }
        } else if (!recipient.equals(other.recipient)) {
            return false;
        }
        if (sender == null) {
            if (other.sender != null) {
                return false;
            }
        } else if (!sender.equals(other.sender)) {
            return false;
        }
        if (ttlAbsolute != other.ttlAbsolute) {
            return false;
        }
        if (ttlMs != other.ttlMs) {
            return false;
        }
        return true;
    }
}
