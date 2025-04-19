#!/usr/bin/env bash
# ───────────────────────  Monokai‑ish custom palette  ────────────────────── #
#   error   #EB1507  (red‑orange)     → rgb(235,21,7)
#   success #1CE60B  (vivid green)    → rgb(28,230,11)
#   warn    #FA8100  (bright orange)  → rgb(250,129,0)
#   info    #FFFFFF  (white)

supports_truecolor() { [[ $COLORTERM =~ (truecolor|24bit) ]]; }

if supports_truecolor; then
    C_ERROR='\033[38;2;235;21;7m'      # #EB1507
    C_SUCCESS='\033[38;2;28;230;11m'   # #1CE60B
    C_WARN='\033[38;2;250;129;0m'      # #FA8100
    C_INFO='\033[38;2;255;255;255m'    # white
elif [[ $TERM =~ 256color ]]; then
    # nearest xterm‑256 approximations
    #   #EB1507 ≈ 196   •  #1CE60B ≈ 118
    #   #FA8100 ≈ 208   •  white   = 15
    C_ERROR='\033[38;5;196m'
    C_SUCCESS='\033[38;5;118m'
    C_WARN='\033[38;5;208m'
    C_INFO='\033[38;5;15m'             # bright white
else
    # 8/16‑colour safe fallback
    C_ERROR='\033[1;31m'   # bright red
    C_SUCCESS='\033[1;32m' # bright green
    C_WARN='\033[0;33m'    # yellow / orange
    C_INFO='\033[1;37m'    # bright white
fi

C_RESET='\033[0m'

# ───────────────────────────── helper output ──────────────────────────── #
info()    { echo -e "${C_INFO}[INFO]${C_RESET}    $*"; }
warn()    { echo -e "${C_WARN}[WARNING]${C_RESET} $*"; }
error()   { echo -e "${C_ERROR}[ERROR]${C_RESET}   $*" >&2; }
success() { echo -e "${C_SUCCESS}[SUCCESS]${C_RESET} $*"; }
die()     { error "$*"; exit 1; }
