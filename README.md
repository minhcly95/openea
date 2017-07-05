# OpenEA Framework

Welcome to the Git repository of the OpenEA project. The OpenEA project is an open-source framework for Evolutionary Computing built in C++. The framework is general-purpose and can be expanded to fit any demand without sacrificing speed and reusability. Here are the main features of the framework:

- General interface for representations and fitness
- General interface for operators
- General interface for algorithms
- Two programming options: C++ code or EAML configuration file
- Powerful Command-Line Interface (CLI) allowing fast configuration
- Plugin loader which loads user code without recompiling
- Parallelism via MultiThreading or Distributed Computatation on Cluster
- Back-up to and Restore from file system
- Speed tracker (both generational and for each phase)

The OpenEA framework is distributed under the GNU General Public License version 3. The framework is written and maintained by Bùi Quang Minh (minhcly95@outlook.com).

For more information, please check the website of the project at https://openea.bqminh.com .
# Installation
Currently, the framework is only tested on Ubuntu 16.04 system. There are two ways to install the framework: from Ubuntu Launchpad (`apt-get`) and directly from GitHub.
## Ubuntu Launchpad
To install the framework using apt-get, first, add the following PPA to your system:

`sudo add-apt-repository ppa:minhcly95/openea`
`sudo apt-get update`

Then, install the package libopenea-cli which includes both the library and the CLI tool:

`sudo apt-get install libopenea-cli`

To test the installation, enter the command:

`openea`

And a list of available sub-commands should be printed on the terminal.

## GitHub
To install from GitHub, first, clone this repository to your computer:

`git clone https://github.com/minhcly95/openea.git`

Then, run the Makefile using the command:

`sudo make install`

This command will compile the source and create symbolic links to the binary and source files, so you can include and link them any where on your machine. To test the installation, enter the command:

`openea`

And a list of available sub-commands should be printed on the terminal.
# How to use
For new users, it is recommended to read the Tutorials page on the website of the project (https://openea.bqminh.com).

