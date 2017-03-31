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

#ifndef SMRF_COMPRESSION_H
#define SMRF_COMPRESSION_H

#include <ios> // for std::stream_size
#include <vector>

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include "smrf/ByteArrayView.h"
#include "smrf/ByteVector.h"
#include "smrf/exceptions.h"

namespace smrf
{

class ByteInsertDevice
{
public:
    using category = boost::iostreams::sink_tag;
    using char_type = char;
    ByteInsertDevice(ByteVector& byteVector) : byteVector(byteVector)
    {
    }
    std::streamsize write(const char* s, std::streamsize n)
    {
        using T = ByteVector::value_type;
        byteVector.insert(byteVector.end(), reinterpret_cast<const T*>(s), reinterpret_cast<const T*>(s) + n);
        return n;
    }

protected:
    ByteVector& byteVector;
};

namespace zlib
{

inline ByteVector compress(const ByteArrayView& view)
{
    boost::iostreams::filtering_ostreambuf outputStream;
    outputStream.push(boost::iostreams::zlib_compressor());

    ByteVector outputBuffer;
    ByteInsertDevice backInserter(outputBuffer);
    outputStream.push(backInserter);

    boost::iostreams::array_source source(reinterpret_cast<const char*>(view.data()), view.size());

    try {
        boost::iostreams::copy(source, outputStream);
    } catch (const boost::iostreams::zlib_error& e) {
        throw EncodingException(std::to_string(e.error()));
    }
    return outputBuffer;
}

inline ByteVector compress(const ByteVector& inputBuffer)
{
    return compress(ByteArrayView(inputBuffer));
}

inline ByteVector decompress(const ByteArrayView& view)
{
    boost::iostreams::stream_buffer<boost::iostreams::array_source> inputStreamBuffer(
            reinterpret_cast<const char*>(view.data()), view.size());

    boost::iostreams::filtering_istreambuf source;
    source.push(boost::iostreams::zlib_decompressor());
    source.push(inputStreamBuffer);

    ByteVector outputBuffer;
    ByteInsertDevice backInserter(outputBuffer);

    try {
        boost::iostreams::copy(source, backInserter);
    } catch (const boost::iostreams::zlib_error& e) {
        throw EncodingException(std::to_string(e.error()));
    }
    return outputBuffer;
}

inline ByteVector decompress(const ByteVector& inputBuffer)
{
    return decompress(ByteArrayView(inputBuffer));
}

} // namespace zlib
} // namespace smrf

#endif // SMRF_COMPRESSION_H
