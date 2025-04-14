#!/bin/bash

set -e

ROOT_DIR=$(pwd)
TOOLCHAIN_FILE="${ROOT_DIR}/toolchain-arm-none-eabi.cmake"
FX_DIR="${ROOT_DIR}/fx"
CONFIG_FILE="${ROOT_DIR}/fx_config.json"
LOG_DIR="${ROOT_DIR}/logs"

mkdir -p "$LOG_DIR"

TARGET=$1
CLEAN=$2

if ! command -v jq >/dev/null 2>&1; then
    echo "[ERROR] ❌ 'jq' is required but not installed."
    exit 1
fi

get_fx_type() {
    local fx_name="$1"
    jq -r --arg key "$fx_name" '.[$key] // empty' "$CONFIG_FILE"
}

build_one_fx() {
    local dir="$1"
    local name
    name=$(basename "$dir")
    local build_dir="$dir/build"
    local log_file="$LOG_DIR/${name}.log"

    FX_TYPE=$(get_fx_type "$name")
    if [[ -z "$FX_TYPE" ]]; then
        echo "[WARNING] ⚠️ Skipping '$name': Not found in $CONFIG_FILE"
        return
    fi

    if [[ "$CLEAN" == "--clean" && -d "$build_dir" ]]; then
        echo "[INFO] 🧹 Cleaning build for '$name'"
        rm -rf "$build_dir"
    fi

    echo "[INFO] 🔧 Building '$name' with FX=$FX_TYPE"
    echo "[INFO] 📄 Logging to: $log_file"

    {
        echo "===== BUILD LOG FOR $name ====="
        date
        echo
        cmake -B "$build_dir" \
              -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
              -DFX_TYPE="$FX_TYPE" \
              -S "$dir"

        cmake --build "$build_dir"
    } &> "$log_file"

    if [[ $? -eq 0 ]]; then
        echo "[SUCCESS] ✅ Built $name"
    else
        echo "[ERROR] ❌ Build failed for '$name'. Check log: $log_file"
    fi

    echo
}

if [[ "$TARGET" == "--all" ]]; then
    echo "[INFO] 🛠 Building all FX projects listed in $CONFIG_FILE"
    echo
    for dir in "$FX_DIR"/*/; do
        if [[ -f "$dir/CMakeLists.txt" ]]; then
            if ! build_one_fx "$dir"; then
                name=$(basename "$dir")
                echo "[ERROR] ❌ Build failed. See logs/${name}.log"
                exit 1
            fi
        fi
    done
elif [[ -z "$TARGET" ]]; then
    echo "[ERROR] ❌ Usage: build.sh <project-name> [--clean] OR build.sh --all [--clean]"
    exit 1
else
    PROJECT_PATH="$FX_DIR/$TARGET"
    if [[ ! -d "$PROJECT_PATH" || ! -f "$PROJECT_PATH/CMakeLists.txt" ]]; then
        echo "[ERROR] ❌ Project '$TARGET' not found in fx/ or missing CMakeLists.txt"
        exit 1
    fi
    if ! build_one_fx "$PROJECT_PATH"; then
        echo "[ERROR] ❌ Build failed for '$TARGET'. See logs/${TARGET}.log"
        exit 1
    fi
fi
