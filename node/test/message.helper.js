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

const bodySize = 100;
const body = Array.from(Array(bodySize).keys());
const headers = {
    key1 : 'value1',
    key2 : 'value2'
};

const message = {
    sender : 'sender',
    recipient : 'recipient',
    ttlMs : 1234567,
    isTtlAbsolute : false,
    headers : headers,
    body : Buffer.from(body),
    isCompressed : false
};

module.exports = message;
