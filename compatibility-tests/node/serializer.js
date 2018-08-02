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

const fs = require('fs');
const smrf = require('smrf-native');
const message = require('../../javascript/test/message.helper.js');
const options = require('./options.js');

message.isCompressed = options.compressed;

try {
    const serializedMessage = smrf.serialize(message);
    fs.writeFileSync(options.path, serializedMessage, 'binary');
} catch (e) {
    console.error('error occured: ' + e);
    process.exit(1);
}
