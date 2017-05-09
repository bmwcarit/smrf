package io.smrf.tests;

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

import static org.junit.Assert.assertEquals;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import org.junit.Test;

import io.smrf.EncodingException;
import io.smrf.MessageDeserializer;
import io.smrf.MessageDeserializerImpl;
import io.smrf.UnsuppportedVersionException;

public class CompatibilityDeserializer {

    private static byte[] serializedMessage;
    private static TestMessage expectedMessage;

    public static void main(String[] args) {
        CliSerializerOptions options = new CliSerializerOptions(args);
        expectedMessage = new TestMessage(options.shouldBeCompressed);
        File inputFile = new File(options.path);
        serializedMessage = new byte[(int) inputFile.length()];
        try {
            FileInputStream inputStream = new FileInputStream(inputFile);
            inputStream.read(serializedMessage, 0, serializedMessage.length);
            inputStream.close();
        } catch (IOException e) {
            System.err.println(e);
            System.exit(1);
            return;
        }
        org.junit.runner.JUnitCore.main("io.smrf.tests.CompatibilityDeserializer");
    }

    @Test
    public void checkExpectedMessage() throws EncodingException, UnsuppportedVersionException {
        MessageDeserializer deserializer = new MessageDeserializerImpl(serializedMessage);
        TestMessage deserializedMessage = TestMessage.getFromDeserializer(deserializer);
        assertEquals(expectedMessage, deserializedMessage);
    }
}
