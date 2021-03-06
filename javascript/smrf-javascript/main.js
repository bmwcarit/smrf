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

const MessageSerializer = require('./src/MessageSerializer');

function checkForMembers(obj, memberName) {
    if (obj[memberName] === undefined) throw new TypeError(`member is not set: ${memberName}`);
}

const smrf = {
    serialize(message) {
        if (!message) {
            throw new Error('invalid number of arguments');
        }
        checkForMembers(message, 'sender');
        checkForMembers(message, 'recipient');
        checkForMembers(message, 'ttlMs');
        checkForMembers(message, 'isTtlAbsolute');
        checkForMembers(message, 'headers');
        checkForMembers(message, 'body');
        checkForMembers(message, 'isCompressed');
        if (isNaN(message.ttlMs)) {
            throw new TypeError(`ttlMs must be a Number: ${message.ttlMs}`);
        }

        return MessageSerializer.serialize(message);
    },
    deserialize(serializedMessage) {
        if (!Buffer.isBuffer(serializedMessage)) {
            throw new TypeError('argument must be a buffer');
        }
        return MessageSerializer.deserialize(serializedMessage);
    }
};

module.exports = smrf;
