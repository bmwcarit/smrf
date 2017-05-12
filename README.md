The Secure Message wRapper Format (SMRF) aims at providing a unified messaging
format for communication between heterogenous endpoints. It fulfils the following requirements:
 - Non-encrypted meta-information (e.g. routing information)
 - Optional signing of message contents
 - Optional compression of message payload
 - Optional encryption of message payload

The format uses Google FlatBuffers and the Cryptographic Message Syntax (CMS) to
specify the wireformat. SMRF is available in Java, JavaScript and C++.
It was initially developed for usage in [joynr](https://joynr.io) to wrap messages
before sending them to a remote party.

[SMRF distribution channels](wiki/Distribution.md) -- Where and how to get SMRF.
[Release Notes](wiki/ReleaseNotes.md) -- All SMRF releases and API changes at a glance.
