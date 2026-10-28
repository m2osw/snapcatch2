
* I'd like to have a tool which goes through my tests and verifies the
  names of things; the sections are expected to reuse the exact name
  of the test as in "\<test-name>: \<what this specific test verifies>".
  Also, we are supposed to use underscore, no dashes, no spaces in those
  names to make it easy to reference (no quotes needed). This should be
  a script which we can run at compile time and defined within
  CMakeLists.txt.

