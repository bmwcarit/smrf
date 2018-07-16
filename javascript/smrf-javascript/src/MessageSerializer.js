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

const flatbuffers = require('flatbuffers').flatbuffers;
const smrf = require('../flatbuffers_generated/smrf_generated').smrf;
const MessagePrefix = require('./MessagePrefix');
const zlib = require('zlib');

const MessageSerializer = {
    serialize(message) {
        const builder = new flatbuffers.Builder();

        const recipient = builder.createString(message.recipient);
        const sender = builder.createString(message.sender);

        const high = message.ttlMs / 0x100000000;
        const low = message.ttlMs % 0x100000000;

        const ttlMs = builder.createLong(low, high);

        const serializedHeaders = [];

        for (const key in message.headers) {
            if (message.headers.hasOwnProperty(key)) {
                const value = message.headers[key];
                const serializedKey = builder.createString(key);

                const serializedValue = builder.createString(value);
                smrf.Header.startHeader(builder);
                smrf.Header.addKey(builder, serializedKey);
                smrf.Header.addValue(builder, serializedValue);
                const headerOffset = smrf.Header.endHeader(builder);
                serializedHeaders.push(headerOffset);
            }
        }

        const headersVector = smrf.Message.createHeadersVector(builder, serializedHeaders);
        const body = message.isCompressed ? zlib.deflateSync(message.body) : message.body;
        const bodyVector = smrf.Message.createBodyVector(builder, body);

        smrf.Message.startMessage(builder);
        smrf.Message.addRecipient(builder, recipient);
        smrf.Message.addSender(builder, sender);
        smrf.Message.addTtlMs(builder, ttlMs);
        smrf.Message.addHeaders(builder, headersVector);
        smrf.Message.addBody(builder, bodyVector);

        // only set values when it's not the default
        if (message.isTtlAbsolute === false) {
            smrf.Message.addTtlAbsolute(builder, false);
        }

        smrf.Message.addIsEncrypted(builder, false);

        if (message.isCompressed) {
            smrf.Message.addIsCompressed(builder, true);
        }
        if (message.signingCallback) {
            smrf.Message.addIsSigned(builder, false);
            smrf.Message.addIsCustomSigned(builder, true);
        }

        const messageOffset = smrf.Message.endMessage(builder);
        builder.finish(messageOffset);
        const serializedMessage = builder.asUint8Array(); // Of type `Uint8Array`.

        let signatureLength = 0;
        let signature;
        if (message.signingCallback) {
            // this is of Type Buffer which inherits from Uint8Array
            signature = message.signingCallback(serializedMessage);
            signatureLength = signature.length;
        }

        const serializedMessageLength = serializedMessage.length;
        const totalSize = serializedMessageLength + signatureLength + MessagePrefix.SIZE;
        const prefixBuffer = MessagePrefix.getPrefix(signatureLength, serializedMessageLength);

        const bufferList = signature ? [prefixBuffer, serializedMessage, signature] : [prefixBuffer, serializedMessage];

        // Buffer.concat can concat Uint8Arrays and Buffers
        return Buffer.concat(bufferList, totalSize);
    },
    deserialize(serializedMessage) {
        const msgObj = MessagePrefix.parsePrefix(serializedMessage);

        const buf = new flatbuffers.ByteBuffer(msgObj.msg);
        const deserializedMessage = smrf.Message.getRootAsMessage(buf);

        if (deserializedMessage.isEncrypted()) {
            throw new Error('encryption is not yet supported');
        }

        const isCompressed = deserializedMessage.isCompressed();

        const bodyBuffer = Buffer.from(deserializedMessage.bodyArray());
        const body = isCompressed ? zlib.inflateSync(bodyBuffer) : bodyBuffer;

        const headerLength = deserializedMessage.headersLength();
        const headers = {};
        for (let i = 0; i < headerLength; i++) {
            const headerObject = deserializedMessage.headers(i);
            headers[headerObject.key()] = headerObject.value();
        }

        const result = {
            recipient: deserializedMessage.recipient(),
            sender: deserializedMessage.sender(),
            ttlMs: deserializedMessage.ttlMs().toFloat64(),
            isTtlAbsolute: deserializedMessage.ttlAbsolute(),
            isCompressed,
            body,
            headers
        };
        if (msgObj.sig) {
            result.signature = msgObj.sig;
        }

        return result;
    }
};

module.exports = MessageSerializer;
