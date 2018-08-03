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
'use strict';

import test from 'ava';
const clone = require('clone');
const smrf = require('./testUtils').getSmrf();
const message = require('./message.helper.js');

const errorMessageWrongArgumentCount = 'invalid number of arguments';

test('throws with no arguments', t => {
    const error = t.throws(() => {
        smrf.serialize();
    }, Error);
    t.is(error.message, errorMessageWrongArgumentCount);
});

test('returns buffer', t => {
    const serializedMessage = smrf.serialize(message);
    t.true(Buffer.isBuffer(serializedMessage));
});

test('add custom signature to the message', t => {
    function signingCallback(messageBuffer) {
        t.pass(1);
        const customSignature = Buffer.from('This is a test signature!');
        return customSignature;
    }
    message.signingCallback = signingCallback;
    t.plan(1);
    const serializedMessage = smrf.serialize(message);
});

function runMissingPropertyTest(messageWithMissingProperty, missingProperty) {
    test('message with missing property "' + missingProperty + '" throws', t => {
        const error = t.throws(() => {
            smrf.serialize(messageWithMissingProperty);
        }, TypeError);
        t.is(error.message, 'member is not set: ' + missingProperty);
    });
}

for (var key in message) {
    if (message.hasOwnProperty(key)) {
        const messageWithMissingProperty = clone(message);
        delete messageWithMissingProperty[key];
        runMissingPropertyTest(messageWithMissingProperty, key);
    }
}

test('message with ttl as string throws', t => {
    let corruptedMessage = clone(message);
    corruptedMessage.ttlMs = 'blubber';
    t.throws(() => {
        smrf.serialize(corruptedMessage);
    }, TypeError);
});
