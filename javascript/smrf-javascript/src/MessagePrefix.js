/*
 * #%L
 * %%
 * Copyright (C) 2018 BMW Car IT GmbH
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
'use strict';

/**
 * offset of version field in bytes
 */
const VERSION_OFFSET = 0;

/**
 * size of version field in bytes
 */
const VERSION_SIZE = 2;

/**
 * offset of sigSize field in bytes
 */
const SIG_SIZE_OFFSET = VERSION_OFFSET + VERSION_SIZE;

/**
 * size of msgSize field in bytes
 */
const SIG_SIZE_SIZE = 2;

/**
 * offset of msgSize field in bytes
 */
const MSG_SIZE_OFFSET = SIG_SIZE_OFFSET + SIG_SIZE_SIZE;

/**
 * size of msgSize field in bytes
 */
const MSG_SIZE_SIZE = 4;

/**
 * size of message prefix in bytes
 */
const SIZE = MSG_SIZE_OFFSET + MSG_SIZE_SIZE;

/**
 * current version of SMRF message format
 */
const VERSION = 1;

const MessagePrefix = {
    getPrefix(sigSize, msgSize) {
        const buffer = Buffer.allocUnsafe(SIZE);
        buffer.writeUInt16LE(VERSION, VERSION_OFFSET);
        buffer.writeUInt16LE(sigSize, SIG_SIZE_OFFSET);
        buffer.writeUInt32LE(msgSize, MSG_SIZE_OFFSET);
        return buffer;
    },
    parsePrefix(serializedMessage) {
        if (serializedMessage.size < SIZE) {
            throw new Error(`serialized message is too short: ${serializedMessage.size} bytes`);
        }

        const version = serializedMessage.readUInt16LE(VERSION_OFFSET);
        if (version !== VERSION) {
            throw new Error(`unsupported version: ${version}`);
        }

        const sigSize = serializedMessage.readUInt16LE(SIG_SIZE_OFFSET);
        const msgSize = serializedMessage.readUInt32LE(MSG_SIZE_OFFSET);
        const expectedMessageSize = sigSize + msgSize + SIZE;

        if (expectedMessageSize < serializedMessage.length) {
            throw new Error(
                `corrupted Message: expected length: ${sigSize}${msgSize}${SIZE} but was: ${serializedMessage.length}`
            );
        }

        // slice creates a view on the buffer, no copying
        const msg = serializedMessage.slice(SIZE, SIZE + msgSize);
        const sig = sigSize > 0 ? serializedMessage.slice(-sigSize) : undefined;
        return { msg, sig };
    },
    SIZE
};

module.exports = MessagePrefix;
