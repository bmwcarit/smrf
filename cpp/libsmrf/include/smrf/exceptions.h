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

#ifndef SMRF_EXCEPTIONS_H
#define SMRF_EXCEPTIONS_H

#include <stdexcept>

namespace smrf
{

class EncodingException : public std::runtime_error
{
    using runtime_error::runtime_error;
};
class SecurityException : public std::runtime_error
{
    using runtime_error::runtime_error;
};

} // namespace smrf

#endif // SMRF_EXCEPTIONS_H
