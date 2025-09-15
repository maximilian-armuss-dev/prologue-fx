#!/usr/bin/env bash
# ---------------------------------------------------------------------------
# build.sh – multi‑FX build helper for Korg logue‑SDK
# ---------------------------------------------------------------------------

set -euo pipefail

# ──────────────────────────── check requirements ───────────────────────── #
command -v jq >/dev/null || die "'jq' is required but not installed."

# ───────────────────────── load colourised logging ─────────────────────── #
ROOT_DIR="$(pwd)"
source "$ROOT_DIR/scripts/logging.sh"   # → info / warn / error / success / die

# ───────────────────────── project‑wide constants ──────────────────────── #
TOOLCHAIN_FILE="$ROOT_DIR/toolchain-arm-none-eabi.cmake"
FX_DIR="$ROOT_DIR/fx"
CONFIG_FILE="$ROOT_DIR/fx_config.json"
LOG_DIR="$ROOT_DIR/logs"
mkdir -p "$LOG_DIR"

# ─────────────────────── helper: read fx‑type from json ────────────────── #
get_fx_type() { jq -r --arg k "$1" '.[$k] // empty' "$CONFIG_FILE"; }

# ──────────────────────── helper: bump & patch manifest ────────────────── #
update_manifest() {
    local project_dir="$1"  fx_type="$2"
    local manifest="$project_dir/manifest.json"
    [[ -f "$manifest" ]] || { error "manifest.json missing in $project_dir"; return 1; }

    local proj_name; proj_name="$(basename "$project_dir")"

    # Step 1: Bump version
    local new_version
    if ! new_version="$(jq -r '
            .header.version
            | capture("(?<maj>\\d+)\\.(?<min>\\d+)-(\\d+)") as $v
            | ($v.maj|tonumber)      as $maj
            | ($v.min|tonumber + 1)  as $min
            | "\($maj).\($min)-0"
        ' "$manifest" 2>/dev/null)"; then
        error "Failed to parse header.version in $manifest"
        return 1
    fi

    # Step 2: Patch temporary manifest
    if ! jq --arg name   "$proj_name" \
            --arg module "${fx_type}fx" \
            --arg ver    "$new_version" \
            '.header.name=$name
             | .header.module=$module
             | .header.version=$ver' \
            "$manifest" >"$manifest.tmp"; then
        error "jq failed while updating manifest for $proj_name"
        rm -f "$manifest.tmp"
        return 1
    fi

    # Step 3: Overwrite manifest
    mv "$manifest.tmp" "$manifest" || {
        error "Could not overwrite $manifest"
        return 1
    }

    info "manifest updated → name=$proj_name  module=${fx_type}fx  version=$new_version"
}

# ───────────────────────── build one fx project ────────────────────────── #
build_one_fx() {
    local dir="$1"; local clean_flag="$2"
    local name="${dir%/}"; name="${name##*/}"
    local fx_type; fx_type="$(get_fx_type "$name")"

    [[ -n "$fx_type" ]] || { warn "Skipping '$name': not in fx_config.json"; return 0; }

    local build_dir="$dir/build"
    local log_file="$LOG_DIR/${name}.log"

    [[ "$clean_flag" == "--clean" && -d "$build_dir" ]] && {
        info "Cleaning previous build of '$name'"; rm -rf "$build_dir";
    }

    info "Building '$name' (FX_TYPE=$fx_type) → $log_file"
    echo "===== BUILD LOG [$name] =====" > "$log_file"
    date >> "$log_file"; echo >> "$log_file"

    # Step 1: Update manifest
    if ! update_manifest "$dir" "$fx_type" >> "$log_file" 2>&1; then
        error "Manifest update failed for $name – see $log_file"; return 1
    fi

    # Step 2: Configure CMake
    if ! cmake -S "$dir" \
               -B "$build_dir" \
               -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
               -DFX_TYPE="$fx_type" >> "$log_file" 2>&1; then
        error "CMake configure failed for $name – see $log_file"; return 1
    fi

    # Step 3: Build
    if ! cmake --build "$build_dir" --target all >> "$log_file" 2>&1; then
        error "Build failed for $name – see $log_file"; return 1
    fi

    success "$name built successfully"
    echo
}

# ─────────────────────────── argument parsing ──────────────────────────── #
TARGET=${1:-}      # <project> | --all
CLEAN=${2:-}       #   --clean | ""

[[ -z "$TARGET" ]] && die "Usage: build.sh <project>|--all [--clean]"

# ───────────────────── main loop (single or all) ───────────────────────── #
if [[ "$TARGET" == "--all" ]]; then
    info "Building all FX projects listed in $CONFIG_FILE"
    for dir in "$FX_DIR"/*/; do
        [[ -f "$dir/CMakeLists.txt" ]] || (info "Skipping $dir" && continue)
        build_one_fx "$dir" "$CLEAN" || exit 1
    done
else
    PROJECT_PATH="$FX_DIR/$TARGET"
    [[ -d "$PROJECT_PATH" && -f "$PROJECT_PATH/CMakeLists.txt" ]] \
        || die "Project '$TARGET' not found or missing CMakeLists.txt"

    build_one_fx "$PROJECT_PATH" "$CLEAN"
fi
