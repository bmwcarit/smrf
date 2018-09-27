# Prerequisites for C++ and Java
SMRF relies on [flatbuffers](https://github.com/google/flatbuffers) and assumes this is available on the system upon starting
the build (`flatc` should be visibile in the PATH).
This is needed in order to generate Java/C++ sources out of the flatbuffers schema file.

# Java
Run `mvn clean install` in the root directory.

# C++
`cmake` will take care of finding `flatc` at configuration time.

# JavaScript
The JavaScript sources are not automatically generated at build time but are part
of the project.
