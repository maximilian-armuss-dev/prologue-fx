#!/bin/bash

# ────────────────────────────────────────────────
# Usage: ./update_manifest.sh mod|rev|del
# ────────────────────────────────────────────────

FX_TYPE="$1"

if [[ -z "$FX_TYPE" ]]; then
  echo "Usage: $0 mod|rev|del"
  exit 1
fi

# FX type string to inject into manifest
NEW_TYPE="${FX_TYPE}fx"

# Location of manifest.json (adjust if needed)
MANIFEST_FILE="manifest.json"

# Sanity check
if [[ ! -f "$MANIFEST_FILE" ]]; then
  echo "Error: ${MANIFEST_FILE} not found!"
  exit 1
fi

# Do in-place replacement using sed (Linux/WSL style)
sed -i '' "s/\"fx_type\": *\"[^\"]*\"/\"fx_type\": \"${NEW_TYPE}\"/" "$MANIFEST_FILE"
