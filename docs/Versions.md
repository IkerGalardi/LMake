# Versioning scheme

This project uses the semver versioning system. This versioning system can be found in its [website](https://semver.org/). But in summary, these are the points that this projects version follow: 

* The versioning system follows the X.Y.Z format: where X is the major version, which will specify the base for the public API (lmake functions and command line options); where Y is the minor version, which specifies minor and incremental changes from the base API; and where Z is the patch version, which just apply bugfixes (no functionality added).

* Excepting major version changes, all lmake versions are retrocompatible. If that is not the case, a bug has ocurred.

Apart from the final versions, development versions exist such as `ALPHA`. Major or minor release should pass these development versions. These are what those version mean:

* **DEV** : in this version the public API is missing some features that the final stable version would have. Using this stage of development can be triky because features can be added or removed frecuently.

* **PRE-ALPHA** : in this stage, the API is more or less stable. Features will not be removed but still aditional features can be added. 

* **ALPHA** : in this stage the API will not be changed unless there are deeply flawed (library that is used to implement that feature is broken, etc...). The only accepted patches are internal changes such as debugging and refactors. At this stage the software starts being usable for the final user.

* **BETA** : this stage is only used for bug fixing. No more feature or API changes are accepted. This is the best development version available. The next available version is the stable version.
