#!/usr/bin/env bash
# Usage:
#   bash examples/chat.sh [MODEL_ID] [--precision INT4|INT8|FP16] [EXTRA_ARGS...]
#
# Prerequisites:
#   - cactus CLI on PATH  (brew install cactus-compute/cactus/cactus, or source ./setup)
#   - Internet access on first run to download model weights
#
# Examples:
#   bash examples/chat.sh
#   bash examples/chat.sh LiquidAI/LFM2.5-1.2B-Instruct
#   bash examples/chat.sh LiquidAI/LFM2.5-1.2B-Instruct --precision INT8
#   bash examples/chat.sh LiquidAI/LFM2.5-1.2B-Instruct --system "You are a pirate."

set -euo pipefail

if ! command -v cactus &>/dev/null; then
    echo "Error: 'cactus' not found on PATH." >&2
    echo "Install: brew install cactus-compute/cactus/cactus" >&2
    echo "   or:  source ./setup   (from the repo root)" >&2
    exit 1
fi

MODEL_ID="${1:-LiquidAI/LFM2.5-1.2B-Instruct}"
shift || true  # shift away the model arg if present; ignore if $# was 0

exec cactus run "$MODEL_ID" "$@"
