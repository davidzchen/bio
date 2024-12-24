#!/bin/bash

bazel run //:refresh_compile_commands && \
    python3 tools/cc_modify.py compile_commands.json
