#!/usr/bin/env bash
set -euo pipefail
EXPECTED_VERSION=17
MODE="${1:-format}"
command -v clang-format >/dev/null 2>&1 || { echo "Error: clang-format is not installed" >&2; exit 1; }
LOCAL_VERSION=$(clang-format --version | grep -oE '[0-9]+' | head -n1 || true)
[[ "$LOCAL_VERSION" == "$EXPECTED_VERSION" ]] || { echo "Error: clang-format $EXPECTED_VERSION is required, found ${LOCAL_VERSION:-unknown}" >&2; exit 1; }
case "$MODE" in
  format) FLAGS=(-i --verbose) ;;
  check|format-check) FLAGS=(--dry-run --Werror --verbose) ;;
  *) echo "Usage: $0 [format|format-check]" >&2; exit 1 ;;
esac
index=0
while IFS= read -r -d '' file; do
  index=$((index + 1))
  echo "Formatting [$index] $file"
  clang-format "${FLAGS[@]}" "$file"
done < <(find . \( -path './.git' -o -path './.git/*' -o -path './libs/brick' -o -path './libs/brick/*' -o -path './libs/brick-boards' -o -path './libs/brick-boards/*' -o -path '*/.pio' -o -path '*/.pio/*' -o -path '*/generated' -o -path '*/generated/*' \) -prune -o -type f \( -name '*.c' -o -name '*.cc' -o -name '*.cpp' -o -name '*.cxx' -o -name '*.h' \) ! \( -name 'generated_assets.h' -o -name 'generated_font*.h' \) -print0)

