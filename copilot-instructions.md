# Copilot Instructions for OneButton Library

## Project Overview

**OneButton** is an Arduino library for detecting button press patterns on a single digital input pin, supporting:
- Single clicks
- Double clicks
- Long presses with start/stop events
- Multi-click detection
- Press events
- Debouncing with configurable timing

The library comes in two versions:
- **OneButton**: Full-featured implementation for boards with ample memory
- **OneButtonTiny**: Lightweight variant for memory-constrained devices (attiny84, etc.)

**License**: BSD-3-Clause
**Repository**: https://github.com/mathertel/OneButton

## Key Design Patterns

### 1. Deferred Initialization Pattern
Prefer declaring buttons without initialization and using `setup()` in Arduino's `setup()` function:

```cpp
// Declaration
OneButton btn;

// In setup()
btn.setup(BUTTON_PIN, INPUT_PULLUP, true);  // pin, mode, activeLow
```

This ensures proper hardware initialization order and maximum compatibility.

### 2. Event-Based Callback Architecture
OneButton uses callbacks (function pointers) for events. Two callback types are supported:
- **Simple callbacks**: `void callback(void)`
- **Parameterized callbacks**: `void callback(void *param)`

### 3. Finite State Machine
OneButton implements a simple FSM internally for state tracking. When generating code, ensure:
- `tick()` is called regularly in the main loop
- Button logic is non-blocking
- Only one event fires per button state transition

## API Usage Guidelines

### Setup and Configuration

```cpp
// Configuration in setup()
btn.setDebounceMs(50);      // Debouncing delay (default: 20ms)
btn.setClickMs(400);        // Time to recognize single click (default: 400ms)
btn.setPressMs(800);        // Time to trigger press event (default: 800ms)
btn.setLongPressIntervalMs(100);  // Interval for repeated long-press callbacks (default: 0)
btn.setIdleMs(3000);        // Auto-idle timeout (optional)
```

### Event Attachment

When generating code that attaches events:
```cpp
// Single click
btn.attachClick(handleClick);
btn.attachClick(handleClickWithParam, (void*)myData);

// Double click
btn.attachDoubleClick(handleDoubleClick);

// Press events
btn.attachPress(handlePress);

// Long press
btn.attachLongPressStart(handleLongPressStart);
btn.attachLongPressStop(handleLongPressStop);

// Multi-click (requires specific configuration)
btn.attachMultiClick(handleMultiClick);
```

### Main Loop Integration

The `tick()` method must be called regularly:
```cpp
void loop() {
  btn.tick();  // Usually called on every iteration
  // Other code...
}
```

### Getting Button State

```cpp
// Active state (requires getPressedMs() or related methods)
if (btn.isPressed()) { }        // Button currently held down
if (btn.isLongPressed()) { }    // Long press is active
```

## Code Style and Conventions

### Naming Conventions
- **Buttons/pins**: `btn1`, `btn2`, `myButton`, `BUTTON_PIN_1` (constants)
- **Callbacks**: `handleClick()`, `onButtonPress()`, `button1_click()`
- **Configuration variables**: Use meaningful names with `Ms` suffix for time values

### Variable Scope
- Global OneButton instances are acceptable for simplicity
- Prefer static arrays for multiple buttons in memory-constrained scenarios
- Use parameterized callbacks for passing context to handlers

### Callback Pattern
```cpp
// Simple callback (preferred for single-purpose buttons)
void handleClick() {
  // Handle event
}

// Parameterized callback (better for multiple buttons with shared handler)
void handleClick(void *param) {
  int buttonId = (int)param;
  // Handle event based on buttonId
}
```

## Common Implementation Patterns

### Single Button with Multiple Events
```cpp
OneButton btn;

void setup() {
  btn.setup(BUTTON_PIN, INPUT_PULLUP, true);
  btn.attachClick(handleClick);
  btn.attachDoubleClick(handleDoubleClick);
  btn.attachLongPressStart(handleLongStart);
  btn.attachLongPressStop(handleLongStop);
}

void loop() {
  btn.tick();
}

void handleClick() { /* single click logic */ }
void handleDoubleClick() { /* double click logic */ }
void handleLongStart() { /* long press started */ }
void handleLongStop() { /* long press ended */ }
```

### Multiple Buttons with Shared Handler
```cpp
#define NUM_BUTTONS 3
OneButton buttons[NUM_BUTTONS];
int btn_pins[NUM_BUTTONS] = {2, 3, 4};

void setup() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].setup(btn_pins[i], INPUT_PULLUP, true);
    buttons[i].attachClick(handleButtonClick, (void*)i);
  }
}

void loop() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].tick();
  }
}

void handleButtonClick(void *param) {
  int buttonId = (int)param;
  // Handle click for specific button
}
```

### OneButtonTiny Usage (Memory-Constrained Devices)
```cpp
#include <OneButtonTiny.h>

OneButtonTiny btn;

void setup() {
  btn.setup(BUTTON_PIN, INPUT_PULLUP, true);
  btn.attachClick(handleClick);        // ✓ Available
  btn.attachDoubleClick(handleDblClick); // ✓ Available
  btn.attachLongPressStart(handleLong);  // ✓ Available
  // Note: MultiClick, LongPressStop NOT available in Tiny version
}
```

## Event Handling Best Practices

### Debouncing
- Default debounce is 20ms, usually sufficient
- Increase `setDebounceMs()` if experiencing phantom clicks or noise

### Timing Configuration Priorities
```
debounce < click < press < longPress
Example: 20ms < 400ms < 800ms < varies
```

### Avoiding Common Issues
1. **Multiple event triggers**: Ensure `tick()` is called once per loop iteration
2. **Slow response**: Call `tick()` more frequently (no delays in loop)
3. **Button state changes not detected**: Verify PIN configuration and `activeLow` setting
4. **Long press interferes with click**: Adjust `setClickMs()` and `setPressMs()` values

## Testing Patterns

When generating test code:
```cpp
// Simulate button press duration (in mock/test environment)
void testSingleClick(OneButton &btn) {
  // Simulate 100ms press
  pressButton(btn);
  delay(100);
  releaseButton(btn);
  delay(500);  // Wait for single-click detection
}

void testDoubleClick(OneButton &btn) {
  pressButton(btn);
  delay(50);
  releaseButton(btn);
  delay(100);  // Brief interval between clicks
  pressButton(btn);
  delay(50);
  releaseButton(btn);
}
```

## Code Generation Tips for Copilot

When using Copilot to generate OneButton code, be specific about:

1. **Button behavior**: "Generate code for detecting single and double clicks"
2. **Timing requirements**: "Set debounce to 50ms and click detection to 500ms"
3. **Callback style**: Request either simple or parameterized callbacks
4. **Multi-button scenarios**: Specify number of buttons and how to differentiate them
5. **Memory constraints**: Mention if OneButtonTiny should be used instead of OneButton

### Example Prompts
- "Add a long-press handler to toggle an LED when held for 1 second"
- "Create a multi-button setup with 3 buttons, each calling a different function"
- "Generate a debounced button click counter that prints to Serial"

## Examples Location

The library includes comprehensive examples:
- `examples/SimpleOneButton` - Basic single button usage
- `examples/FunctionalButton` - Using C++ lambda callbacks
- `examples/TwoButtons` - Multiple button management
- `examples/LongPressEvents` - Long press detection
- `examples/SpecialInput` - Custom input sources
- `examples/InterruptOneButton` - Interrupt-based detection
- `examples/BlinkMachine` - State machine example

## Deprecated Methods to Avoid

When generating code, avoid these legacy method names:
- `setDebounceTicks()` → Use `setDebounceMs()` instead
- `setClickTicks()` → Use `setClickMs()` instead
- `setPressTicks()` → Use `setPressMs()` instead

## Library Version

Current version: **2.6.2**

## References

- **Homepage**: http://www.mathertel.de/Arduino/OneButtonLibrary.aspx
- **Repository**: https://github.com/mathertel/OneButton
- **License**: BSD-3-Clause (see LICENSE file)
- **Author**: Matthias Hertel (mathertel@hotmail.com)
