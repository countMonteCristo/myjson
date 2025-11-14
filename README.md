# myjson
Yet another JSON serialization/deserialization library in C++

This library was written moslty in educational purposes. Use it in your own risk.

## Getting started
### Dependencies
* gcc or clang or other c++ compiler
* cmake

### Testing dependencies
* cppunit
* python3

### Installation
1. Install dependencies:
```sh
$ sudo apt-get install gcc cmake libcppunit
```

2. Clone the repo:
```sh
git clone https://github.com/countMonteCristo/myjson.git
```

3. Build project
```sh
mkdir build && cd build
cmake ..
make -i myjson_static myjson_shared
```

4. Run unit tests (from **build** folder)
```sh
make myjson-unit-test
../bin/myjson-unit-test
```

5. Run complex test (from **build** folder)
```sh
make myjson-complex-test
```

6. Install library (from **build** folder)
```sh
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${INSTALL_FOLDER}" ..
make -j8 install
```

### Features

* library provides classes and functions for [de]serializing JSON objects
* ability to prettify and customize JSON serialization
* strict mode for parsing and serializing JSON entities (NaN, Infinity and -Infinity are not allowed in strict mode)

### Example
```cxx
#include <iostream>

#include "json.hpp"

int main()
{
    mj::JsonNode node{mj::JsonObject{
        std::make_pair("k01", 1),
        std::make_pair("k02", false),
        std::make_pair("k03", nullptr),
        std::make_pair("k04", mj::JsonArray{1, nullptr, "23", true}),
        std::make_pair("k05", mj::JsonObject{
            std::make_pair("k06", true),
            std::make_pair("k07", 123),
            std::make_pair("k08", "456"),
            std::make_pair("k09", nullptr),
        })
    }};
    node.PrintToStream(std::cout, mj::SerializeOptions{.pretty = true, .field_sep = ": ", .indent = "  "});

    return 0;
}
```

In this example we construct JsonNode out of JsonObject, which represents some JSON object, and serialize it into stdout, setting pretty to true and customizing some other options.

You can build this example as follows:
* linking with static library:
```sh
g++ -std=gnu++20 example.cpp -o example_static -I"${PATH_TO_REPO}/include" -L"${PATH_TO_REPO}/lib" -l:libmyjson.a
```
* linking with dynamic library:
```sh
g++ -std=gnu++20 example.cpp -o example_shared -I"${PATH_TO_REPO}/include" -L"${PATH_TO_REPO}/lib" -lmyjson -Wl,-rpath,./lib
```

Both static and shared versions will give the same output:
```
> ./example_shared
{
  "k05": {
    "k08": "456",
    "k07": 123,
    "k09": null,
    "k06": true
  },
  "k04": [
    1,
    null,
    "23",
    true
  ],
  "k03": null,
  "k02": false,
  "k01": 1
}
```


## Tests description
There are unit tests and single complex test needed for checking if library works as expected.

Complex test generates random JSON strings using python [json](https://docs.python.org/3/library/json.html) module. Then these random jsons are used as an input for **process_json** utility which reads strings from stdin, parses them into JsonNode and serializes back into strings. After that another python script is used to check whether processed JSON strings represent the same JSON entities as the randomly generated ones.

All complex test code contains in [run.sh](https://github.com/countMonteCristo/myjson/blob/main/tests/complex/run.sh)


## Contacts

Artem Shapovalov: artem_shapovalov@frtk.ru

Project Link: https://github.com/countMonteCristo/myjson
