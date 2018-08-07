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
const smrf = require('./testUtils').getSmrf();

const expectedErrorMessage = 'argument must be a buffer';

test('throws with no arguments', t => {
    const error = t.throws(() => {
        smrf.deserialize();
    }, TypeError);
    t.is(error.message, expectedErrorMessage);
});

test('throws with malformed input data', t => {
    const malformedInput = Buffer.from([0, 1, 2, 3, 4, 5, 6, 7, 8, 9]);
    const error = t.throws(() => {
        smrf.deserialize(malformedInput);
    }, Error);
});

function runWrongArgumentTest(arg) {
    test('throws with wrong argument type: ' + typeof arg, t => {
        const error = t.throws(() => {
            smrf.deserialize(arg);
        }, TypeError);
        t.is(error.message, expectedErrorMessage);
    });
}
const wrongArguments = [true, 1, 'string', { key: 'value' }];
wrongArguments.forEach(arg => runWrongArgumentTest(arg));
