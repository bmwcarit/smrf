package io.smrf;

import java.util.Arrays;

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

import java.util.Map;

import com.google.common.collect.ImmutableMap;

public class TestMessage {

    private final String sender;
    private final String recipient;
    private final Map<String, String> headers;
    private final long ttlMs;
    private final boolean ttlAbsolute;
    private final byte[] body;

    public TestMessage() {
        sender = "sender";
        recipient = "recipient";
        headers = ImmutableMap.of("key1", "value1", "key2", "value2");
        ttlMs = 12345L;
        ttlAbsolute = false;
        body = new byte[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    }

    public TestMessage(MessageDeserializer deserializer) throws EncodingException {
        sender = deserializer.getSender();
        recipient = deserializer.getRecipient();
        headers = deserializer.getHeaders();
        ttlMs = deserializer.getTtlMs();
        ttlAbsolute = deserializer.isTtlAbsolute();
        if (!deserializer.isEncrypted()) {
            body = deserializer.getUnencryptedBody();
        } else {
            throw new EncodingException("encryption not yet implemented");
        }
    }

    public void initSerializer(MessageSerializer serializer) {
        serializer.setSender(sender);
        serializer.setRecipient(recipient);
        serializer.setHeaders(headers);
        serializer.setTtlMs(ttlMs);
        serializer.setTtlAbsolute(ttlAbsolute);
        serializer.setBody(body);
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + Arrays.hashCode(body);
        result = prime * result + ((headers == null) ? 0 : headers.hashCode());
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
