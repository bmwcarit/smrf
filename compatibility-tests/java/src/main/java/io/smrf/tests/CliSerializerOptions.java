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
package io.smrf.tests;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;

public class CliSerializerOptions {

    public String path;
    public boolean shouldBeCompressed;

    CliSerializerOptions (String[] args) {
        Options options = new Options();
        options.addRequiredOption("p", "path", true, "path to be written to / read from");
        options.addRequiredOption("c", "compressed", true, "compressed|uncompressed");
        CommandLineParser parser = new DefaultParser();
        CommandLine cmd = null;
        try {
            cmd = parser.parse(options, args);
        } catch (ParseException e) {
            System.err.println(e);
            System.exit(1);
            return;
        }
        path = cmd.getOptionValue("p");
        shouldBeCompressed = cmd.getOptionValue("c").equals("compressed");
    }
}
