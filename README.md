# 🎛️ Prologue‑FX Framework

🚧 **Under Active Construction** – expect rapid changes, renamed folders, and the occasional exploding build.

## 🙄 The Problem with KORG's [logue-SDK](https://github.com/korginc/logue-sdk)...

A **LOT** of boilerplate code ─ their repository probably contains more duplicate lines of code than original ones.

Let's take a quick look into their [template for a modulation effect](https://github.com/korginc/logue-sdk/tree/master/platform/prologue/dummy-modfx):
```
dummy-modfx/
├── ld/                            
│   ├── main_api.syms
│   ├── rules.ld
│   └── usermodfx.ld
│ 
├── tpl/
│   └── _unit.c
│
├── Makefile
├── manifest.json
└── project.mk
```
➡️ **A lot of files, but yet no source code!**

## 🤩 The Solution ─ ✨ This Framework ✨

In contrast to the original logue-SDK, this framework offers a **framework-first approach** to effect development:

- Inherit from `FXBase`, implement your FX logic – you’re done.
- Build for **any** FX slot (mod/rev/del) on the prologue 
- Boilerplate-free development with reusable utilities
- One-command build to `.prlgunit`

➡️ **Look at how much cleaner this has become**:
```
dummy-modfx/
├── src/                           # Also, this folder is new ;)        
├── CMakeLists.txt         
└── manifest.json     
```

## 🧪 Project Structure

```
prologue-fx-suite/
├── fx/                            # FX unit folders (Clipper, Panner, etc.)
│   ├── <Effect>/
│   │   ├── src/                   # Source code (.cpp/.hpp)
│   │   ├── CMakeLists.txt         # Obvious :)
│   │   └── manifest.json          # Main information source for the prologue
│
├── logs/                          # Build logs per effect
├── out/                           # Generated .prlgunit files
├── scripts/                       # General utility shell scripts
├── shared/                        # Shared DSP wrappers, linker scripts, math utils
│   ├── wrappers/                  # FXBase, ModFXWrapper, etc.
│   ├── ld/                        # usermodfx.ld, userrevfx.ld, etc.
│   ├── tpl/                       # _unit.c template
│   └── inc/                       # Shared headers
│
├── CMSIS/                         # ARM CMSIS headers (submodule)
├── fx_config.json                 # Maps FX name → slot (mod/del/rev)
├── build.sh                       # Top-level build script
├── toolchain-arm-none-eabi.cmake  # Toolchain config
└── README.md
```

## 🛠️ Repository Setup

```bash
# Clone the repo and its submodules
git clone --recurse-submodules https://github.com/maximilian-armuss-dev/prologue-fx.git
cd prologue-fx
```

> 📦 The `CMSIS` module is included as a submodule (see `.gitmodules`).

## 🧩 Creating a New FX Plugin

1. Duplicate another FX unit (like `Clipper`) and rename it, e.g. `MyNewEffect`

2. Adapt the `manifest.json` (& `CMakeLists.txt` if needed)

3. Add your plugin name to `fx_config.json`, and specify the FX module, e.g.
```json
{
  "Clipper": "mod",
  "Panner": "del",
  "MyNewEffect": "rev"
}
```

4. Create your own source files in `fx/MyNewEffect/src`, e.g.
```c++
// MyNewEffect.hpp
#pragma once

#include "float_math.h"
#include "FXBase.hpp"


class MyNewEffectClass : public FXBase {
public:
    void set_speed(float speed) override;
    void set_depth(float depth) override;
    float process_main_L(float x) override;
    float process_main_R(float x) override;

private:
    float speed_ = 0.5f;  // I recommend naming this after the parameters you actually want to control, e.g. "gain_", "pan_", etc.
    float depth_ = 0.5f;
};
```
> **Note:** Your class **must** inherit from `FXBase` (and thus override these methods)!
```c++
// MyNewEffect.cpp
#include "MyNewEffect.hpp"


void MyNewEffectClass::set_speed(const float speed) { speed_ = speed; }

void MyNewEffectClass::set_depth(const float depth) { depth_ = 2 * depth; }

float MyNewEffectClass::process_main_L(const float x) {
    float processed_signal = x;         // TODO: Actual processing
    return clip1m1f(processed_signal);  // It's always a good idea to throw on a clipper at the end 
}

float MyNewEffectClass::process_main_R(const float x) {
    float processed_signal = x;         // TODO: Actual processing
    return clip1m1f(processed_signal);  // It's always a good idea to throw on a clipper at the end 
}
```

5. Set FXClass to the name of your class in `module_config.hpp`
```c++
#pragma once
#include "MyNewEffect.hpp"

using FXClass = MyNewEffectClass;
```
> **Note:** Although it may seem a bit ugly, it reduces the amount of boilerplate code significantly!
> Have a look at `shared/wrappers/FXWrapper` if you're interested in the inner workings! :)
6. Build :)

## ⚙️ Building
From the root directory of your repository, build using:

| Task                      | Command                            |
|---------------------------|------------------------------------|
| Build single effect       | `./build.sh <effect-name>`         |
| Clean & rebuild           | `./build.sh <effect-name> --clean` |
| Build all FX              | `./build.sh --all`                 |
| Build all FX from scratch | `./build.sh --all --clean`         |

The resulting `.prlgunit` files are saved to the `out/` directory.

> **Important:** Use either [logue-cli](https://github.com/korginc/logue-cli) or the [prologue sound librarian](https://www.korg.com/us/support/download/software/0/778/3995/) to upload FX to your synth!

---
## 📬 Contact

If you're curious, want to collaborate, or just geek out over music, feel free to reach out!
