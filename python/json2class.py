#!/usr/bin/env python3

import sys
import json
import class_writer

major_versions = {
  "JAVA_VERSION_5": 49,
  "JAVA_VERSION_6": 50,
  "JAVA_VERSION_7": 51,
  "JAVA_VERSION_8": 52
}

def get_major_version(value):
  if isinstance(value, int): return value

  if not value in major_versions:
    print("Warning: Unknown version name: " + value)
    return 0

  return major_versions[value]

class_access_flags = {
  "public":     0x0001,
  "final":      0x0010,
  "super":      0x0020,
  "interface":  0x0200,
  "abstract":   0x0400,
  "synthetic":  0x1000,
  "annotation": 0x2000,
  "enum":       0x4000
}

def get_class_access_flags(flags):
  value = 0

  for flag in flags:
    if not flag in class_access_flags:
      print("Error: Unknown class access flag " + flag)
      continue

    value |= class_access_flags[flag]

  return value

field_access_flags = {
  "public":    0x0001,
  "private":   0x0002,
  "protected": 0x0004,
  "static":    0x0008,
  "final":     0x0010,
  "volatile":  0x0040,
  "transient": 0x0080,
  "synthetic": 0x1000,
  "enum":      0x4000
}

def get_field_access_flags(flags):
  value = 0

  for flag in flags:
    if not flag in field_access_flags:
      print("Error: Unknown field access flag " + flag)
      continue

    value |= field_access_flags[flag]

  return value

method_access_flags = {
  "public":       0x0001,
  "private":      0x0002,
  "protected":    0x0004,
  "static":       0x0008,
  "final":        0x0010,
  "synchronized": 0x0020,
  "bridge":       0x0040,
  "varargs":      0x0080,
  "native":       0x0100,
  "abstract":     0x0400,
  "strict":       0x0800,
  "synthetic":    0x1000
}

def get_method_access_flags(flags):
  value = 0

  for flag in flags:
    if not flag in method_access_flags:
      print("Error: Unknown method access flag " + flag)
      continue

    value |= method_access_flags[flag]

  return value

# ----------------------- fold here ---------------------

if len(sys.argv) != 2:
  print("Usage: python3 json2class.py <file.json>")
  sys.exit(1)

with open(sys.argv[1], "r") as f:
  data = json.load(f)

test = class_writer.create()

if "class_name" in data:
  class_writer.set_class_name(test, data["class_name"])

if "super_class" in data:
  class_writer.set_super_class(test, data["super_class"])

if "major_version" in data:
  major_version = get_major_version(data["major_version"])
  class_writer.set_major_version(test, major_version)

if "minor_version" in data:
  class_writer.set_minor_version(test, data["minor_version"])

if "access_flags" in data:
  access_flags = get_class_access_flags(data["access_flags"])
  class_writer.set_access_flags(test, access_flags)

for field in data["fields"]:
  access_flags = get_field_access_flags(field["access_flags"])
  is_inherited = 0

  if "is_inherited" in field:
    if field["is_inherited"]: is_inherited = 1

  class_writer.add_field(test, field["name"], field["type"], access_flags, is_inherited)

for method in data["methods"]:
  access_flags = get_method_access_flags(method["access_flags"])
  code = bytes(method["code"])

  class_writer.add_method(test, field["name"], field["type"], access_flags, method["max_stack"], method["max_locals"], code)

class_writer.write(test)

