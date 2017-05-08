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

function runRoundtripTest(expectedMessage, t)
{
    const serializedBuffer = smrf.serialize(expectedMessage);
    const deserializedMessage = smrf.deserialize(serializedBuffer);
    t.is(serializedBuffer, deserializedMessage.serializedMessage);
    roundtripCompare(expectedMessage, deserializedMessage, t);
}

test('not compressed', t => {
    const expectedMessage = clone(message);
    expectedMessage.isCompressed = false;
    runRoundtripTest(expectedMessage, t);
});

test('compressed', t => {
    const expectedMessage = clone(message);
    expectedMessage.isCompressed = true;
    runRoundtripTest(expectedMessage, t);
});

test('large TTL', t => {
    const expectedMessage = clone(message);
    expectedMessage.ttlMs = new Date().getTime();
    expectedMessage.isTtlAbsolute = true;
    runRoundtripTest(expectedMessage, t);
});
