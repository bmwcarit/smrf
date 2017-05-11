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
package io.smrf;

import java.nio.ByteBuffer;

import org.junit.Assert;
import org.junit.Test;

public class ZlibCompressionTest {
    final byte[] data = "HELLO WORLD".getBytes();
    final ByteBuffer repeatedData;
    final byte[] expectedCompressedData = javax.xml.bind.DatatypeConverter
            .parseHexBinary("789cf370f5f1f15708f70ff271f118658e324799a34c729900bee83700");

    public ZlibCompressionTest() {
        final int repetitions = 100;
        repeatedData = ByteBuffer.allocate(data.length * repetitions);
        for (int i = 0; i < repetitions; ++i) {
            repeatedData.put(data);
        }
    }

    @Test
    public void compressGeneratesExpectedOutput() throws EncodingException {
        byte[] compressedData = ZlibCompression.compress(repeatedData.array());
        Assert.assertArrayEquals(expectedCompressedData, compressedData);
    }

    @Test
    public void roundTrip() throws EncodingException {
        byte[] expectedDecompressedData = repeatedData.array();
        byte[] decompressedData = ZlibCompression.decompress(ZlibCompression.compress(expectedDecompressedData));
        Assert.assertArrayEquals(expectedDecompressedData, decompressedData);
    }
}
