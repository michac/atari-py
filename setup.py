from distutils.core import setup
import os
import os.path, sys
import shutil

# make sure dll exists
ale_c_lib = 'atari_py\\ale_interface\\visual_studio\\bin\\x64\Release\\Arcade.Learning.Environment.Python.dll'
if not os.path.isfile(ale_c_lib):
    print('ERROR: Unable to find required dll, Please ensure you\'ve built ALE and the ale_python_interface projects')
    sys.exit()

# get dlls from dependencies
dll_files = os.listdir('atari_py\\ale_interface\\visual_studio\\dependencies\\dll_win_x64')
# filter to make sure list only has dlls
dll_files = list(filter(lambda x: x[-3:] == 'dll', dll_files))
# copy dependency dlls
for dll in dll_files:
    shutil.copy('atari_py\\ale_interface\\visual_studio\\dependencies\\dll_win_x64\\' + dll, 'atari_py\\ale_interface\\build')

# copy compiled dll
shutil.copy(ale_c_lib, 'atari_py\\ale_interface\\build')

setup(name='atari-py',
      version='0.1.1',
      description='Python bindings to Atari games',
      url='https://github.com/openai/atari-py',
      author='OpenAI',
      author_email='info@openai.com',
      license='',
      packages=['atari_py'],
      package_dir={'atari_py': 'atari_py\\ale_interface\\build'},
      package_data={'atari_py': ['*.dll']})
