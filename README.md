# 🎛️ The *PrologueFX* Framework

PrologueFX is a cleaner, more efficient way to build custom FX for KORG logue synths.
It removes boilerplate from the logue-SDK and lets you focus entirely on your DSP logic.

![Prologue 8](prologue-8.png)

## ⚔️ logue-SDK vs. PrologueFX 

| 🛑 Pain in KORG's [logue-SDK](https://github.com/korginc/logue-sdk)                                                                                               | ✅ How PrologueFX fixes it                                                                                                                                                                       |
|-------------------------------------------------------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| **Redundant boilerplate** – every new effect needs **7 near-identical files** (linker script, glue C, wrapper C++, etc.).                                         | **Zero boilerplate** – all shared scaffolding lives in `shared/`. A fresh effect folder only holds **`CMakeLists.txt` + `manifest.json` + your own source**. During the build process, a `main.cpp` is automatically generated, so you only need to focus on your source code. |
| Must implement **3 C-style callbacks** (`MODFX_INIT / PROCESS / PARAM`) **by hand**. To keep code tidy you end up writing an *extra* file and wiring it yourself. | Just inherit `FXBase`, override its virtual methods, done. The framework auto-generates the mandatory callbacks and hooks your class in.                                                        |
| Different effect slots (Mod, Rev, Del) use **different function signatures**, so you usually duplicate projects when you want the same DSP in another slot.       | One line in **`fx_config.json`** chooses the slot: `"Clipper": "rev"`. Change it to `"mod"` or `"del"` and rebuild – the wrapper adapts the signatures automatically.                           |
| Typical workflow relies on diving into the Docker container and running Make by hand.                                                                             | Top-level **`build.sh`** script, with clean build and build-all options. All finished `.prlgunit` files are collected in **`out/`**, so you never hunt through build folders.                   |

> **Bottom line:** PrologueFX keeps the power of the logue-SDK but strips away the busy-work. Focus on DSP, not on scaffolding.


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

1. Duplicate another FX unit (like `Clipper`) and rename it, e.g. `MyNewReverb`

2. Adapt the `manifest.json` (& `CMakeLists.txt` if needed)

3. Add your plugin name to `fx_config.json`, and specify the FX module, e.g.
```json
{
  "Clipper": "mod",
  "Panner": "del",
  "MyNewReverb": "rev"
}
```

4. Create your own source files in `fx/MyNewReverb/src`
> **Note:** The filenames of your source files must follow the convention `<EffectName>FX.cpp` and `<EffectName>FX.hpp` (e.g., `MyNewReverbFX.cpp` and `MyNewReverbFX.hpp`), where `<EffectName>` is the name of your parent folder.
```c++
// MyNewReverbFX.hpp
#pragma once

#include "float_math.h"
#include "FXBase.hpp"


class MyNewReverbFX : public FXBase {
public:
    void set_speed(float speed) override;
    void set_depth(float depth) override;
    float process_main_L(float x) override;
    float process_main_R(float x) override;

private:
    float speed_ = 0.5f;  // I recommend naming this after the parameters you actually want to control, e.g. "size_", "decay_", etc.
    float depth_ = 0.5f;
};
```
> **Note:** Your class **must** inherit from `FXBase` (and thus override these methods)!
```c++
// MyNewReverb.cpp
#include "MyNewReverbFX.hpp"


void MyNewReverbFX::set_speed(const float speed) { speed_ = speed; }

void MyNewReverbFX::set_depth(const float depth) { depth_ = 2 * depth; }

float MyNewReverbFX::process_main_L(const float x) {
    float processed_signal = x;         // TODO: Actual processing
    return clip1m1f(processed_signal);  // It's always a good idea to throw on a clipper at the end 
}

float MyNewReverbFX::process_main_R(const float x) {
    float processed_signal = x;         // TODO: Actual processing
    return clip1m1f(processed_signal);  // It's always a good idea to throw on a clipper at the end 
}
```

5. Build :)

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
