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

#include <algorithm>
#include <fstream>
#include <iterator>

#include <smrf/ByteVector.h>
#include <smrf/MessageSerializer.h>

#include "Options.h"
#include "tests/RoundtripMessage.h"

void writeFile(const smrf::ByteVector& byteVector, const std::string& path)
{
    std::ofstream output(path, std::ios::out | std::ofstream::binary);
    std::copy(byteVector.begin(), byteVector.end(), std::ostreambuf_iterator<char>(output));
}

int main(int argc, char* argv[])
{
    Options options;
    options.parse(argc, argv);
    RoundtripMessage message(options.isCompressed);
    smrf::MessageSerializer serializer;
    message.init(serializer);
    smrf::ByteVector serializedMessage = serializer.serialize();
    writeFile(serializedMessage, options.path);
}
