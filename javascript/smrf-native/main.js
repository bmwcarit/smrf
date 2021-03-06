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

var smrfNative;
try {
    smrfNative = require('smrf-native-cpp.node');
} catch (err) {
    smrfNative = require('bindings')('smrf-native-cpp.node');
}

function deserialize(buffer)
{
    return new smrfNative.MessageDeserializer(buffer);
}

module.exports = {
    serialize : smrfNative.serialize,
    deserialize : deserialize
};
