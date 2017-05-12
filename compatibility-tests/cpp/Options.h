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

#ifndef OPTIONS_H
#define OPTIONS_H

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

#include <boost/program_options.hpp>
#include <gtest/gtest.h>

struct Options {
    std::string path;
    bool isCompressed;

    void parse(int argc, char* argv[])
    {

        namespace po = boost::program_options;
        po::options_description desc("Available options");
        std::string compressed;

        desc.add_options()("path,p", po::value(&path)->required(), "path")(
                "compressed,c", po::value(&compressed)->required(), "compressed|uncompressed");

        try {
            po::variables_map vm;
            po::store(po::parse_command_line(argc, argv, desc), vm);
            po::notify(vm);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }

        isCompressed = compressed == "compressed";
    }
};

#endif // OPTIONS_H
