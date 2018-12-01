## Installation instructions for Cairo
This plotting requires cairo (actually used cairocffi instead as it seemed easier to install)
Installation instructions are below:


Linux:
sudo apt install libcairo2-dev
conda install -c anaconda cffi
pip install cairocffi


Windows:
Install GTK with cairo from here:
https://github.com/tschoonj/GTK-for-Windows-Runtime-Environment-Installer
From Anaconda Prompt:
conda install -c anaconda cffi
pip install pyhamcrest
pip install cairocffi

Had a problem with an mkl dll error I fixed with:
conda install mkl-2018.0.2

This was possibly from installing a conda-forge version of pyhamcrest:
conda install -c conda-forge pyhamcrest
