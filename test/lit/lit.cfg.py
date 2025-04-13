import lit.formats
import os

config.name = 'pyxx'
config.test_format = lit.formats.ShTest(True)
# config.suffixes = [".py"]

config.test_source_root = os.path.dirname(__file__)
config.test_exec_root = os.path.join(config.pyxx_obj_root, 'test')
# config.excludes = ["lit.cfg.py"]

config.substitutions.append(
  ('%pyxx', os.path.join(config.pyxx_obj_root, 'pyxx'))
  )