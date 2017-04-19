package io.smrf;

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

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.zip.DataFormatException;
import java.util.zip.Deflater;
import java.util.zip.Inflater;

public class ZlibCompression {

    private interface Processor {
        boolean finished();

        void setInput(byte[] input);

        int process(byte[] output) throws DataFormatException;
    }

    private static final class Compressor implements Processor {
        Deflater deflater = new Deflater();

        public boolean finished() {
            return deflater.finished();
        }

        public void setInput(byte[] input) {
            deflater.setInput(input);
            deflater.finish();
        }

        public int process(byte[] output) {
            return deflater.deflate(output);
        }
    }

    private static final class Decompressor implements Processor {
        Inflater inflater = new Inflater();

        public boolean finished() {
            return inflater.finished();
        }

        public void setInput(byte[] input) {
            inflater.setInput(input);
        }

        public int process(byte[] output) throws DataFormatException {
            return inflater.inflate(output);
        }
    }

    private static <T extends Processor> byte[] process(T processor, byte[] input)
            throws DataFormatException, IOException {
        processor.setInput(input);
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream(input.length);
        byte[] buffer = new byte[1024];
        while (!processor.finished()) {
            int compressedBytes = processor.process(buffer);
            outputStream.write(buffer, 0, compressedBytes);
        }
        outputStream.close();
        return outputStream.toByteArray();
    }

    public static byte[] compress(byte[] input) throws EncodingException {
        try {
            return process(new Compressor(), input);
        } catch (DataFormatException | IOException e) {
            throw new EncodingException("could not compress input");
        }
    }

    public static byte[] decompress(byte[] input) throws EncodingException {
        try {
            return process(new Decompressor(), input);
        } catch (DataFormatException | IOException e) {
            throw new EncodingException("could not decompress input");
        }
    }
}
