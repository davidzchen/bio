# Bazel C++ Project Template

This is a template repository for C++ projects built with Bazel. The repository
boilerplate provides the following:

* `.bazelrc` with C++20, clang-tidy, and sanitizer builds
* `.clang-format` to automatically format according to the Google C++ style
  guide.
* `.clang-tidy` with a full suite of clang-tidy checks.
* `.clangd` enforcing strict header includes
* `WORKSPACE` file pulling in external dependencies, including gTest, Abseil
  Protocol Buffers, LLVM (for clang-tidy), etc.
* Root `BUILD` file with target to generate `compile-commands.json`
* And more.
