# SMRF 0.3.0
[JS] Add smrf javascript implementation without native modules
[JS] smrf-native: Fixed usage of deprecated v8 API in node 10 by using Nan replacement

# SMRF 0.2.3
[Java] explicitly free resources when performing zlib (de-)compression

# SMRF 0.2.2
[JS] Updated dev dependency `microtime` to 2.1.6.
[JS] Fixed potential problem wrt. unhandled C++ exceptions.

# SMRF 0.2.1
[C++] Fixed potential integer overflow in deserializer.<br/>
[JS] Updated and cleaned up the project's dependencies.

# SMRF 0.2.0
[C++, Java] Fixed memory alignment issue, by making sure that MessagePrefix is 8 byte aligned.
This version is not backwards compatible.

# SMRF 0.1.4
Bugfix in the node module regarding its native part

# SMRF 0.1.3
Minor Sonatype bugfixes.

# SMRF 0.1.2
Minor Sonatype bugfixes.

# SMRF 0.1.1
Minor bug fixes release.

## Other changes
* Fix deploy to maven central.

# SMRF 0.1.0
This is the initial release of SMRF.

Major features:
* Support serialization/deserialization in C++, Java and JavaScript.
