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

#ifndef STRINGS_H
#define STRINGS_H

#include <nan.h>

#include "util.h"

struct Strings {
    using String = Nan::Persistent<v8::String>;
    String serializedMessage;
    String isCompressed;
    String body;
    String sender;
    String recipient;
    String headers;
    String ttlMs;
    String isTtlAbsolute;
    String signature;

    static auto& get()
    {
        static Strings value;
        return value;
    }

    static void init()
    {
        get().serializedMessage.Reset(util::string("serializedMessage"));
        get().isCompressed.Reset(util::string("isCompressed"));
        get().body.Reset(util::string("body"));
        get().sender.Reset(util::string("sender"));
        get().recipient.Reset(util::string("recipient"));
        get().headers.Reset(util::string("headers"));
        get().ttlMs.Reset(util::string("ttlMs"));
        get().isTtlAbsolute.Reset(util::string("isTtlAbsolute"));
        get().signature.Reset(util::string("signature"));
    }
};

#endif // STRINGS_H
