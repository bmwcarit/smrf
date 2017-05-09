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

import java.io.FileOutputStream;
import java.io.IOException;

import io.smrf.EncodingException;
import io.smrf.MessageSerializer;
import io.smrf.MessageSerializerImpl;

public class CompatibilitySerializer {

    public static void main(String[] args) {
        CliSerializerOptions options = new CliSerializerOptions(args);
        MessageSerializer serializer = new MessageSerializerImpl();
        TestMessage message = new TestMessage(options.shouldBeCompressed);
        message.initSerializer(serializer);
        byte[] serializedMessage;
        try {
            serializedMessage = serializer.serialize();
        } catch (SecurityException | EncodingException e) {
            System.err.println(e);
            System.exit(1);
            return;
        }
        try {
            FileOutputStream outputStream = new FileOutputStream(options.path);

            outputStream.write(serializedMessage, 0, serializedMessage.length);
            outputStream.flush();
            outputStream.close();
        } catch (IOException e) {
            System.err.println(e);
            System.exit(1);
        }
    }
}
