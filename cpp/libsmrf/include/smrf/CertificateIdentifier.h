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

#ifndef SMRF_CERTIFICATEIDENTIFIER_H
#define SMRF_CERTIFICATEIDENTIFIER_H

#include <string>

namespace smrf
{

class CertificateIdentifier
{
public:
    /**
     * @brief constructs a CertificateIdentifier based on the given parameters
     * @param issuer string encoding of certificate issuer according to RFC2253
     * @param serialNumber hex string encoding of certificate's serial number
     */
    CertificateIdentifier(const std::string& issuer, const std::string& serialNumber);

    /**
     * @brief constructs a CertificateIdentifier based on the given parameters
     * @param issuer string encoding of certificate issuer according to RFC2253
     * @param serialNumber hex string encoding of certificate's serial number
     */
    CertificateIdentifier(std::string&& issuer, std::string&& serialNumber);

private:
    const std::string issuer;
    const std::string serialNumber;
};

} // namespace smrf

#endif // SMRF_CERTIFICATEIDENTIFIER_H
