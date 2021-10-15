# MECH423_Libraries

## Coding style
| |   |
|----------------------------------------------------------------------------|-------------------------|
| Variables                                                                  | `snake_case`            |
| Functions                                                                  | `camelCase`             |
| Structs/enums                                                              | `CapitalCamelCase`      |
| Struct methods                                                             | `StructName_methodName` |
| Nested structs/enums (e.g. if an enum is closely associated with a struct) | `StructName_NestedName` |
| Constants                                                                  | `ALL_CAPS`              |

Note that since most of these are modules, any free functions and global variables should be prefixed with the module name, e.g. `Uart_readData()`.

Suffix any internal variables (i.e. anything that library users shouldn't be touching, the equivalent of private variables in C#/other object oriented languages) with an underscore. E.g. the UART module is probably going to need circular buffers for received data, so an acceptable name for this buffer is `Uart_buffer_`, since code shouldn't interface directly with the buffer; instead using the provided functions.
