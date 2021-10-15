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

Have a banner at the top of each component, something like
```c
/*
 * Circular Buffer ===========================================
 * Circular Buffer ===========================================
 * Circular Buffer ===========================================
 */
```
so it can be easily spotted if you're searching for it, as well as having something to Ctrl-F for. Later, we can make an index of all of the libraries, complete with Ctrl-F terms.

Document all your functions. Ideally, follow a docgen convention, something like
```c
/*
@brief Description of the method
@param param_name Description of the argument
@param param_name2 Description of the argument
@return Return value, only if applicable

Feel free to add more detailed information at the bottom like so
*/
void finagleTheThingamabobs() {
    // ...
}
```
Also include plenty of comments describing what you're doing. Ideally, the comments will contain enough info that you don't have to go to the datasheet for most activities.
