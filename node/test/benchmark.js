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

const smrf = require('..');
const Benchmark = require('benchmark');
const message = require('./message.helper.js');

const suite = new Benchmark.Suite();

const serializedMessage = smrf.serialize(message);

suite.add('serialize',
          function() {
              return smrf.serialize(message);
          })
        .add('deserialize',
             function() {
                 return smrf.deserialize(serializedMessage);
             })
        .on('cycle',
            function(event) {
                console.log(String(event.target));
            })
        .run({'async' : false});
