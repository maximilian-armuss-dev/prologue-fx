# 🎛️ Prologue‑FX‑Suite

🚧 **Under Active Construction** – expect rapid changes, renamed folders, and the occasional exploding build.

## 📌 Overview

**Prologue‑FX‑Suite** is a **modern, simplified toolchain** for developing custom FX units using:

- A clean, CMake-based build system
- Shared modular C++ FX framework
- Easy-to-use CLI scripts
- Automatic FX slot configuration

It also features some of the FX I wrote for my prologue :)
## 🌟 Unique Selling Point (USP)

This suite offers a **framework-first approach** to effect development:

- Inherit from `FXBase`, implement your logic – you’re done.
- Fully abstracted build logic and slot setup
- Boilerplate-free development with reusable utilities
- One-command build to `.prlgunit`

## 🎯 Project Goals

| Status | Goal                                                       |
|--------|------------------------------------------------------------|
| ✅     | `FXBase` framework for minimal user code                   |
| ✅     | Build for **any** FX slot (mod/rev/del) on the prologue    |
| ✅     | Fully CMake-powered build setup (no Docker)                |
| ✅     | User-friendly bash build system + manifest auto-versioning |
| ⏳     | GitHub Actions CI to build all FX units                    |

> **Note:** We do not ship pre‑compiled binaries.  
> Build scripts generate the `*.prlgunit` files locally so you can audit and tweak the code yourself.

## 🛠️ Repository Setup

```bash
# 1. Clone the repo and its submodules (includes CMSIS + logue-SDK)
git clone --recurse-submodules https://github.com/maximilian-armuss-dev/prologue-fx-suite.git
cd prologue-fx-suite
```

> 📦 The `CMSIS` module is included as a submodule (see `.gitmodules`).

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

## ⚙️ Building Effects

| Task                      | Command                            |
|---------------------------|------------------------------------|
| Build single effect       | `./build.sh <effect-name>`         |
| Clean & rebuild           | `./build.sh <effect-name> --clean` |
| Build all FX              | `./build.sh --all`                 |
| Build all FX from scratch | `./build.sh --all --clean`         |

The resulting `.prlgunit` files are saved to the `out/` directory.

> **Important:** Use either [logue-cli](https://github.com/korginc/logue-cli) or the [prologue sound librarian](https://www.korg.com/us/support/download/software/0/778/3995/) to upload FX to your synth:  

## 🧩 Creating a New FX Plugin

1. Create a new folder in `fx/`, e.g. `fx/MyNewEffect`
2. Copy a `CMakeLists.txt` from another FX unit and adapt if needed
3. Add your plugin name to `fx_config.json`, e.g.

```json
{
  "Clipper": "mod",
  "Panner": "mod",
  "MyNewEffect": "rev"
}
```

4. Run:

```bash
./build.sh MyNewEffect
```

🎉 Your `.prlgunit` will appear in the `out/` folder.

---
## 📬 Contact

If you're curious, want to collaborate, or just geek out over music, feel free to reach out!
