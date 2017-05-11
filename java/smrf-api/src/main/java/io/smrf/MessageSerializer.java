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

import java.security.PrivateKey;
import java.security.cert.X509Certificate;
import java.util.Map;

public interface MessageSerializer {
    public MessageSerializer encryptFor(X509Certificate cert);
    public byte[] serialize() throws SecurityException, EncodingException;
    public MessageSerializer setBody(byte[] body);
    public MessageSerializer setCompressed(boolean isCompressed);
    public MessageSerializer setHeaders(Map<String, String> headers);
    public MessageSerializer setRecipient(String recipient);
    public MessageSerializer setSender(String sender);
    public MessageSerializer setTtlMs(long ttlMs);
    public MessageSerializer setTtlAbsolute(boolean ttlAbsolute);
    public MessageSerializer signWith(X509Certificate cert, PrivateKey key);
}
