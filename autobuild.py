#Script to autogenerate bazel BUILD file
import os
from jinja2 import Template

# Get list of files from a directory
def get_files_from_directory(directory):
    return [f[:f.index('.')] for f in os.listdir(directory) if os.path.isfile(os.path.join(directory, f))]

# Define the Jinja template
template_string = """cc_library(
    name = "source",
    srcs = [{% for file in src_files %}"src/{{ file }}.cpp"{% if not loop.last %}, {% endif %}{% endfor %}], 
    hdrs = [{% for file in hdr_files %}"inc/{{ file }}.h"{% if not loop.last %}, {% endif %}{% endfor %}], 
    includes = ["inc"]
)

{% for test_file in test_files %}
cc_test(
    name = "{{ test_file }}", 
    size = "small",
    srcs = ["tests/{{ test_file }}.cc"], 
    deps = [
        ":source",
        "@googletest//:gtest",
        "@googletest//:gtest_main"
    ]
)
{% endfor %}
"""

# Directories
src_directory = 'src'
inc_directory = 'inc'
tests_directory = 'tests'

# Get lists of files
src_files = get_files_from_directory(src_directory)
hdr_files = get_files_from_directory(inc_directory)
test_files = get_files_from_directory(tests_directory)

# Create a Jinja Template object and render the content
template = Template(template_string)
output = template.render(src_files=src_files, hdr_files=hdr_files, test_files=test_files)

# Write the generated content to a file
output_file = 'BUILD'  # You can change the filename as needed
with open(output_file, 'w') as f:
    f.write(output)

print(f"Generated file: {output_file}")
