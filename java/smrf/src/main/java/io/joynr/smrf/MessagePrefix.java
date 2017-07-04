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

public class MessagePrefix {

    /**
     * offset of version field in bytes
     */
    private final static int VERSION_OFFSET = 0;

    /**
     * size of version field in bytes
     */
    private final static int VERSION_SIZE = 2;

    /**
     * offset of sigSize field in bytes
     */
    private final static int SIG_SIZE_OFFSET = VERSION_OFFSET + VERSION_SIZE;

    /**
     * size of msgSize field in bytes
     */
    private final static int SIG_SIZE_SIZE = 2;

    /**
     * offset of msgSize field in bytes
     */
    private final static int MSG_SIZE_OFFSET = SIG_SIZE_OFFSET + SIG_SIZE_SIZE;

    /**
     * size of msgSize field in bytes
     */
    private final static int MSG_SIZE_SIZE = 4;

    /**
     * size of message prefix in bytes
     */
    public final static int SIZE = MSG_SIZE_OFFSET + MSG_SIZE_SIZE;

    /**
     * current version of SMRF message format
     */
    public final static short VERSION = 1;

    public short version = VERSION;
    public short sigSize = 0;
    public int msgSize = 0;

    public MessagePrefix(byte[] serializedMessage) throws EncodingException {

        if (serializedMessage.length < SIZE) {
            throw new EncodingException("message size too small");
        }

        final ByteBuffer prefixBuffer = ByteBuffer.wrap(serializedMessage, VERSION_OFFSET, SIZE)
                                                  .order(ByteOrder.LITTLE_ENDIAN);

        this.version = prefixBuffer.getShort(VERSION_OFFSET);
        this.sigSize = prefixBuffer.getShort(SIG_SIZE_OFFSET);
        this.msgSize = prefixBuffer.getInt(MSG_SIZE_OFFSET);
    }

    public MessagePrefix() {
    }

    public void writeToPreallocatedBuffer(ByteBuffer serializedMessage) {
        assert (serializedMessage.capacity() >= SIZE);

        serializedMessage.putShort(version);
        serializedMessage.putShort(sigSize);
        serializedMessage.putInt(msgSize);
    }
}
