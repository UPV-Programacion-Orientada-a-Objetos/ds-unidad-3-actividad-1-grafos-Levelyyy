from setuptools import setup, Extension
from Cython.Build import cythonize
import os

# Define the extension module
extensions = [
    Extension(
        name="neuronet",
        sources=["src/cython/neuronet.pyx", "src/cpp/GrafoDisperso.cpp"],
        include_dirs=["src/cpp"],
        language="c++",
        extra_compile_args=["-std=c++14"],
        extra_link_args=["-static-libgcc", "-static-libstdc++"],
    )
]

setup(
    name="neuronet",
    ext_modules=cythonize(extensions, language_level="3"),
)
