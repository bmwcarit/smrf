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

import io.joynr.smrf.MessageSerializer.SigningFunction;
import io.joynr.smrf.tests.TestMessage;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertArrayEquals;


import java.nio.ByteBuffer;

import org.junit.Test;

public class RoundtripTest {

    public RoundtripTest() {
        customSignature = "This is a test signature!".getBytes();
    }

    byte[] customSignature;

    void checkCustomSignature(MessageDeserializer deserializer) throws EncodingException
    {
        byte[] signature = deserializer.getSignature();
        assertEquals(signature.length, customSignature.length);
        assertArrayEquals(signature, customSignature);
    }

    @Test
    public void notCompressedNotSignedNotEncrypted() throws SecurityException, EncodingException,
                                                    UnsuppportedVersionException {
        run(false, false, false, false);
    }

    @Test
    public void compressedNotSignedNotEncrypted() throws SecurityException, EncodingException,
                                                 UnsuppportedVersionException {
        run(true, false, false, false);
    }

    @Test
    public void notCompressedNotSignedNotEncryptedCustomSigned() throws SecurityException, EncodingException,
                                                    UnsuppportedVersionException {
        run(false, false, false, true);
    }

    @Test
    public void compressedNotSignedNotEncryptedCustomSigned() throws SecurityException, EncodingException,
                                                 UnsuppportedVersionException {
        run(true, false, false, true);
    }

    private void run(boolean shouldBeCompressed, boolean isSigned, boolean isEncrypted, boolean isCustomSigned) throws SecurityException,
                                                                                       EncodingException,
                                                                                       UnsuppportedVersionException {
        TestMessage expectedMessage = new TestMessage(shouldBeCompressed);
        MessageSerializer serializer = new MessageSerializerImpl();
        expectedMessage.initSerializer(serializer);

        if(isCustomSigned) {
            SigningFunction signingCallback = new SigningFunction() {
                @Override
                public byte[] fn(ByteBuffer msgBuffer) {
                    return customSignature;
                }
            };
            serializer.setCustomSigningCallback(signingCallback);
        }

        byte[] serializedMessage = serializer.serialize();
        MessageDeserializer deserializer = new MessageDeserializerImpl(serializedMessage);
        TestMessage deserializedMessage = TestMessage.getFromDeserializer(deserializer);

        if(isCustomSigned) {
            checkCustomSignature(deserializer);
        }

        assertEquals(expectedMessage, deserializedMessage);
        assertEquals(serializedMessage.length, deserializer.getMessageSize());
    }
}
