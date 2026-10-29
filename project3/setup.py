from setuptools import setup, Extension
setup(name='AnalysisModule', ext_modules=[Extension('analysis_module', sources=['analysis_module.c'])])
