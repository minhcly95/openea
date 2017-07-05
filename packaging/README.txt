How to package this library:

- Copy the template folder
- Change the version number of the new folder
- Run ./openea-prep <version_number> to copy the source code into the folder and create tarball
- Go in the new folder and run debuild
- For Ubuntu package, run debuild -S -sa

