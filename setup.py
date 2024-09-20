from setuptools import setup, Extension
import pybind11
import os

cdl_path = os.path.dirname(__file__)


cpp_files = [cdl_path + "/bind/bind.cpp",
             cdl_path + "/core/src/condorcet_domain.cpp",
             cdl_path + "/core/src/forbidden_permutation.cpp",
             cdl_path + "/core/src/utils.cpp",
             cdl_path + "/core/src/wrapper.cpp"]

# Define the C++ extension
ext_modules = [
    Extension(
        'cdl',  
        cpp_files,              
        include_dirs=[pybind11.get_include(), cdl_path + "/core/include"],  
        language='c++',          
    ),
]

# Call setup() to package the module
setup(
    name='condorcet-domain',
    version='1.0.1',
    author='Bei Zhou',
    description='',
    long_description="",
    ext_modules=ext_modules,
    zip_safe=False,
)

# python setup.py sdist
# twine upload dist/*