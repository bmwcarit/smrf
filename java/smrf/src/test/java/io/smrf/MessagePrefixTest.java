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

import static org.junit.Assert.assertEquals;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import org.junit.Test;
import org.junit.runner.RunWith;

import junitparams.Parameters;
import junitparams.JUnitParamsRunner;

@RunWith(JUnitParamsRunner.class)
public class MessagePrefixTest {

    @Test(expected = EncodingException.class)
    @Parameters
    public void tooShortInputCausesException(int size) throws EncodingException {
        byte[] bytes = new byte[size];
        @SuppressWarnings("unused")
        MessagePrefix messagePrefix = new MessagePrefix(bytes);
    }

    public Integer[] parametersForTooShortInputCausesException() {
        Integer[] array = new Integer[MessagePrefix.SIZE];
        for (int i = 0; i < array.length; i++) {
            array[i] = i;
        }
        return array;
    }

    @Test
    public void roundtrip() throws EncodingException {
        MessagePrefix expectedMessagePrefix = new MessagePrefix();
        expectedMessagePrefix.version = Byte.MAX_VALUE;
        expectedMessagePrefix.msgSize = Integer.MAX_VALUE;
        expectedMessagePrefix.sigSize = Integer.MAX_VALUE;

        ByteBuffer buffer = ByteBuffer.allocate(MessagePrefix.SIZE).order(ByteOrder.LITTLE_ENDIAN);
        expectedMessagePrefix.writeToPreallocatedBuffer(buffer);

        MessagePrefix messagePrefix = new MessagePrefix(buffer.array());
        assertEquals(expectedMessagePrefix.version, messagePrefix.version);
        assertEquals(expectedMessagePrefix.msgSize, messagePrefix.msgSize);
        assertEquals(expectedMessagePrefix.sigSize, messagePrefix.sigSize);
    }
}
