import os
import re
import subprocess

# Find libraries
files = os.listdir('.')
libs = []
for f in files:
  if f.endswith('dylib'):
    if not os.path.islink(f):
      libs.append(f)

# Find links on each library
deps = {}
for l in libs:
  output = subprocess.check_output(['otool', '-L', l])
  odeps = output.split('\n')
  odeps = list(map(lambda x: x.strip(' :\r\n\t'), odeps))
  odeps = list(map(lambda x: re.sub(' \(comp.*', '', x), odeps))
  odeps = filter(lambda x: x != l and x != "", odeps)
  odeps = filter(lambda x: not re.match(".*{0}.*".format(l), x), odeps)
  deps[l] = odeps

# Look for any local matches for the same library
mappings = {}
for l in deps:
  print('Looking for mappings on {0}'.format(l))
  mapped = {}
  for dep in deps[l]:
    for lib in deps:
      if re.match(".*{0}.*".format(lib), dep):
        print('- Found mapping to match {0} to {1}'.format(dep, lib))
        mapped[dep] = lib
  mappings[l] = mapped

# Apply install_name_tool to map items
print('\nPatching:')
for l in mappings:
  print('- {0}'.format(l))
  for key in mappings[l]:
    command = ['install_name_tool', '-change', key, '@loader_path/{0}'.format(mappings[l][key]), l]
    subprocess.call(command)
