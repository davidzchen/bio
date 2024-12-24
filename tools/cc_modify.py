#!/usr/bin/env python3

"""
Converts relative paths in compile_commands.json to absolute paths.

From https://github.com/hedronvision/bazel-compile-commands-extractor/issues/178
"""

import re
import subprocess

def replace_pattern(file_name, search_pattern, replacement_pattern):
    # Read the contents of the file
    with open(file_name, 'r') as file:
        file_content = file.read()

    # Perform the replacement using regular expressions
    new_content = re.sub(search_pattern, replacement_pattern, file_content)

    # Write the updated content back to the file
    with open(file_name, 'w') as file:
        file.write(new_content)

# Define the file name
file_name = 'compile_commands.json'

# Read the contents of the file
with open(file_name, 'r') as file:
    file_content = file.read()

# Call the function to replace the first pattern
search_pattern1 = r'"(external/.*?)"'
output_base = subprocess.check_output(['bazel', 'info', 'output_base']).decode('utf-8').strip()
replacement_pattern1 = fr'"{output_base}/\1"'
file_content = re.sub(search_pattern1, replacement_pattern1, file_content)

# Call the function to replace the second pattern
search_pattern2 = r'"bazel-out/(.*?)"'
output_path = subprocess.check_output(['bazel', 'info', 'output_path']).decode('utf-8').strip()
replacement_pattern2 = fr'"{output_path}/\1"'
file_content = re.sub(search_pattern2, replacement_pattern2, file_content)

# Write the updated content back to the file
with open(file_name, 'w') as file:
    file.write(file_content)
