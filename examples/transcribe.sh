#!/usr/bin/env bash
# Usage:
#   bash examples/transcribe.sh [AUDIO_FILE] [MODEL_ID] [--language LANG] [EXTRA_ARGS...]
#
# Prerequisites:
#   - cactus CLI on PATH  (brew install cactus-compute/cactus/cactus, or source ./setup)
#   - Internet access on first run to download model weights
#   - Audio file must be WAV format; omit for live microphone input
#
# Examples:
#   bash examples/transcribe.sh                               # live microphone
#   bash examples/transcribe.sh tests/assets/test.wav        # transcribe a file
#   bash examples/transcribe.sh tests/assets/test.wav nvidia/parakeet-tdt-0.6b-v3 --language en

set -euo pipefail

if ! command -v cactus &>/dev/null; then
    echo "Error: 'cactus' not found on PATH." >&2
    echo "Install: brew install cactus-compute/cactus/cactus" >&2
    echo "   or:  source ./setup   (from the repo root)" >&2
    exit 1
fi

AUDIO_FILE="${1:-}"
MODEL_ID="${2:-nvidia/parakeet-tdt-0.6b-v3}"

# consume the two positional args; pass remainder through to cactus
shift 2 2>/dev/null || shift "$#"

if [ -n "$AUDIO_FILE" ]; then
    if [ ! -f "$AUDIO_FILE" ]; then
        echo "Error: audio file not found: $AUDIO_FILE" >&2
        exit 1
    fi
    exec cactus transcribe "$MODEL_ID" --file "$AUDIO_FILE" "$@"
else
    exec cactus transcribe "$MODEL_ID" "$@"
fi
