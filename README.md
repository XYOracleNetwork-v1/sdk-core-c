[logo]: https://cdn.xy.company/img/brand/XY_Logo_GitHub.png

![logo]

# sdk-core-c

A library to preform all basic XYO Network functions.
This includes creating an origin chain, maintaining an origin chain, negotiations for talking to other nodes, and other basic functionality.
The library has heavily abstracted modules so that all operations will work with any crypto, storage, networking, etc.

## Building
You can add sdk-core-c to your existing app by cloning the project, building the library, and linking it into your project.

```
git clone git@github.com:XYOracleNetwork/sdk-core-c.git
cd sdk-core-c
mkdir build && cd build
cmake ..
make

This should compile a XYO library in the `build` folder.
```

To install the project run `sudo make install`.

#### Prerequisites
* C compiler
* Cmake

## License
This project is licensed under the MIT License - see the LICENSE.md file for details
