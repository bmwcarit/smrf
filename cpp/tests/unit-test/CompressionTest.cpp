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

#include <gtest/gtest.h>

#include "smrf/ByteVector.h"

#include "../compression.h"

using namespace smrf;

TEST(CompressionTest, zlibCompressGeneratesExpectedOutput)
{
    const ByteVector data = {'H', 'E', 'L', 'L', 'O', ' ', 'W', 'O', 'R', 'L', 'D'};
    ByteVector repeatedData;
    for (std::size_t i = 0; i < 100; ++i) {
        repeatedData.insert(repeatedData.end(), data.begin(), data.end());
    }

    // generated through `openssl zlib < input.txt | xxd -i`
    // clang-format off
    const ByteVector expectedCompressedData = {0x78,
                                               0x9c,
                                               0xf3,
                                               0x70,
                                               0xf5,
                                               0xf1,
                                               0xf1,
                                               0x57,
                                               0x08,
                                               0xf7,
                                               0x0f,
                                               0xf2,
                                               0x71,
                                               0xf1,
                                               0x18,
                                               0x65,
                                               0x8e,
                                               0x32,
                                               0x47,
                                               0x99,
                                               0xa3,
                                               0x4c,
                                               0x72,
                                               0x99,
                                               0x00,
                                               0xbe,
                                               0xe8,
                                               0x37,
                                               0x00};
    // clang-format on

    ByteVector compressedData = zlib::compress(repeatedData);

    EXPECT_EQ(compressedData, expectedCompressedData);
}

TEST(CompressionTest, roundtrip)
{
    ByteVector expectedData = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    ByteVector compressedData = zlib::compress(expectedData);
    ByteVector decompressedData = zlib::decompress(compressedData);
    EXPECT_EQ(expectedData, decompressedData);
}
