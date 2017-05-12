# Java Artifacts
Java artifacts are deployed to Maven repositories.

## Snapshot Versions
Snapshot versions are available from the [Sonatype Nexus Snapshots]
(https://oss.sonatype.org/content/repositories/snapshots) repository.

To access snapshot versions through Maven add the following repository snippet to the
`<repositories>` section of your Maven POM:

```xml
<repository>
	<id>sonatype-nexus-snapshots</id>
	<name>Sonatype Nexus Snapshots</name>
	<url>https://oss.sonatype.org/content/repositories/snapshots</url>
	<releases>
		<enabled>false</enabled>
	</releases>
	<snapshots>
		<enabled>true</enabled>
	</snapshots>
</repository>
```

## Release Versions
Release versions are directly available from [Maven Central](http://maven.org) repository.

## PGP Signatures
All artifacts are signed and deployed with PGP signatures.

### SMRF PGP Public Key
Key: EEEDBA7F
Key fingerprint: D846 BD58 0EC3 6F82 3BFC 89C5 8856 691F EEED BA7F

```text
-----BEGIN PGP PUBLIC KEY BLOCK-----
Version: GnuPG v2

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
=4FkC
-----END PGP PUBLIC KEY BLOCK-----
```

# JavaScript artifacts
JavaScript artifacts are deployed on [npmjs.com](https://www.npmjs.com/package/smrf-native).

