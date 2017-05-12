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

#include <gtest/gtest.h>
#include <smrf/ByteArrayView.h>
#include <smrf/ByteVector.h>
#include <smrf/MessageDeserializer.h>

#include "Options.h"
#include "tests/RoundtripMessage.h"

Options options;

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    options.parse(argc, argv);
    return RUN_ALL_TESTS();
}

smrf::ByteVector readFile(const std::string& path)
{
    std::ifstream input(path, std::ios::binary);
    std::vector<char> buffer((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
    return smrf::ByteVector(buffer.begin(), buffer.end());
}

TEST(CompatibilityTest, deserialize)
{
    RoundtripMessage message(options.isCompressed);
    smrf::ByteVector serializedMessage = readFile(options.path);
    smrf::ByteArrayView serializedMessageView(serializedMessage);
    smrf::MessageDeserializer deserializer(serializedMessageView);
    message.check(deserializer);
}
