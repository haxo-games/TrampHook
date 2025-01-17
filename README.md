# TrampHook

A lightweight trampoline hook library for x86 and x64 Windows applications. TrampHook allows you to intercept function calls while preserving the ability to call the original function.

## Features

- Supports both x86 and x64 architectures
- Preserves original function calls through trampolines
- Minimal overhead

## Installation

1. Clone the repository:
```bash
git clone https://github.com/yourusername/TrampHook.git
```

2. Add the include directory to your project
3. Link against the library

## Usage

### Basic Example

```cpp
#include <TrampHook/TrampHook.h>

// Function pointer for the function to be hooked
using OriginalFunc = int (*__cdecl)(int a, int b);

// Our detour function
int DetourFunction(int a, int b) 
{
    // Do something before the original call
    printf("Before original call: %d, %d\n", a, b);
    
    // Call original function through the trampoline
    OriginalFunc original = (OriginalFunc)TrampHook::hook(originalFunction, DetourFunction);
    int result = original(a, b);
    
    // Do something after the original call
    printf("After original call. Result: %d\n", result);
    return result;
}

int main() 
{
    // Hook the function
    void* trampoline = TrampHook::hook(originalFunction, DetourFunction);

    if (!trampoline) 
    {
        printf("Failed to hook function\n");
        return 1;
    }

    // Call the function (will go through our detour)
    originalFunction(5, 3);

    // Unhook when done
    TrampHook::unhook(originalFunction);
    return 0;
}
```

### API Reference

```cpp
namespace TrampHook 
{
public:
    // Hook a function, returns trampoline pointer or nullptr on failure
    void* hook(void* target, void* detour);
    
    // Remove hook from a specific function
    void unhook(void* target);
    
    // Remove all hooks
    void unhookAll();
};
```

## How It Works

TrampHook uses a trampoline-based hooking approach:

1. **Analysis**: Disassembles enough instructions from the target function to make room for the hook
2. **Trampoline**: Copies the original instructions to a new executable memory location
3. **Jump Back**: Adds a jump from the trampoline back to the original function
4. **Hook Placement**: Places a jump to the detour function at the original location

```
Original Function:                      Trampoline:
┌─────────────────────────────┐         ┌─────────────────────────────┐
│ injected jump to detour     │         │ overwritten instruction1    │
│ dummy code for alignement   │──┐      │ overwritten instruction2    │
│ original instructions       │  │      │ ...                         │
│ ...                         │◄─│──────│ jump back                   │
└─────────────────────────────┘  │      └─────────────────────────────┘
                                 │        ▲     
                                 │        │
Detour Function:                 │        │
┌─────────────────────────────┐  │        │
│ pre-processing              │◄─┘        │
│ call trampoline             │───────────┘
│ post-processing             │
└─────────────────────────────┘
```

## Platform Support

- Windows x86 (32-bit)
- Windows x64 (64-bit)

## Limitations

- Requires at least 5 bytes (x86) or 12 bytes (x64) for hooking
- Some functions may not be hookable due to their instruction patterns (the library does not handle/resolve relative jumps in the trampoline)
- Does not handle thread-safety

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Security Considerations

- Ensure you have the necessary permissions to modify code memory
- Be aware that antivirus and anti-cheat software may flag hooking behavior
- Consider thread safety