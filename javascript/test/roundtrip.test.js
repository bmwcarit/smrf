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
const smrf = require('..');
const message = require('./message.helper.js');
const roundtripCompare = require('./roundtrip.helper.js');
const customSignature = Buffer.from('This is a test signature!');

function signingCallback(messageBuffer) {
    return customSignature;
}

function checkCustomSignature(deserializedMessage, t) {
    const signature = deserializedMessage.signature;
    t.is(signature.length, customSignature.length);
    t.deepEqual(signature, customSignature);
}

function runRoundtripTest(expectedMessage, t, isCustomSigned) {
    let message = clone(expectedMessage);
    if (isCustomSigned) {
        expectedMessage.signature = customSignature;
        message.signingCallback = signingCallback;
    }

    const serializedBuffer = smrf.serialize(message);

    const deserializedMessage = smrf.deserialize(serializedBuffer);

    if (isCustomSigned) {
        checkCustomSignature(deserializedMessage, t);
    }
    t.is(serializedBuffer, deserializedMessage.serializedMessage);
    roundtripCompare(expectedMessage, deserializedMessage, t);

}

test('not compressed', t => {
    const expectedMessage = clone(message);
    expectedMessage.isCompressed = false;
    runRoundtripTest(expectedMessage, t, false);
});

test('compressed', t => {
    const expectedMessage = clone(message);
    expectedMessage.isCompressed = true;
    runRoundtripTest(expectedMessage, t, false);
});

test('not compressed customSigned', t => {
    let expectedMessage = clone(message);
    expectedMessage.isCompressed = false;
    runRoundtripTest(expectedMessage, t, true);
});

test('compressed customSigned', t => {
    const expectedMessage = clone(message);
    expectedMessage.isCompressed = true;
    runRoundtripTest(expectedMessage, t, true);
});

test('large TTL', t => {
    const expectedMessage = clone(message);
    expectedMessage.ttlMs = new Date().getTime();
    expectedMessage.isTtlAbsolute = true;
    runRoundtripTest(expectedMessage, t);
});
