#!/usr/bin/env bash
# Runs the same checks as CI (.github/workflows/ci.yml): format, cppcheck,
# arduino-lint and a compile check. See CONTRIBUTING.md for tool install steps.
set -u

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TOOLS_DIR="$ROOT_DIR/.tools"

ARDUINO_LINT="$TOOLS_DIR/arduino-lint"
ARDUINO_CLI="$TOOLS_DIR/arduino-cli"
[ -f "$ARDUINO_LINT.exe" ] && ARDUINO_LINT="$ARDUINO_LINT.exe"
[ -f "$ARDUINO_CLI.exe" ] && ARDUINO_CLI="$ARDUINO_CLI.exe"

FAILED=0
section() { printf '\n=== %s ===\n' "$1"; }

command -v clang-format >/dev/null 2>&1 || { echo "clang-format not found (see CONTRIBUTING.md)"; exit 1; }
command -v cppcheck >/dev/null 2>&1 || { echo "cppcheck not found (see CONTRIBUTING.md)"; exit 1; }
[ -x "$ARDUINO_LINT" ] || { echo "arduino-lint not found in $TOOLS_DIR (see CONTRIBUTING.md)"; exit 1; }
[ -x "$ARDUINO_CLI" ] || { echo "arduino-cli not found in $TOOLS_DIR (see CONTRIBUTING.md)"; exit 1; }

section "clang-format"
find "$ROOT_DIR" -path "$ROOT_DIR/.git" -prune -o \( -iname "*.h" -o -iname "*.cpp" -o -iname "*.ino" \) -print0 \
  | xargs -0 clang-format --dry-run --Werror \
  || FAILED=1

section "cppcheck"
cppcheck --enable=warning,style,performance,portability --inline-suppr --std=c++11 --platform=avr8 \
  --error-exitcode=1 -I "$ROOT_DIR/src" "$ROOT_DIR/src" "$ROOT_DIR/examples" \
  || FAILED=1

section "arduino-lint"
# --library-manager false (CI uses "submit"): submit mode rejects any .exe found
# anywhere under the project, which would always flag our own .tools/ binaries.
# Every other rule still runs the same either way.
"$ARDUINO_LINT" --compliance strict --library-manager false "$ROOT_DIR" \
  || FAILED=1

section "arduino-cli compile"
for dir in "$ROOT_DIR"/examples/*/; do
  "$ARDUINO_CLI" compile --fqbn arduino:avr:uno --library "$ROOT_DIR" "$dir" \
    || FAILED=1
done

if [ "$FAILED" -ne 0 ]; then
  printf '\nOne or more checks failed.\n'
  exit 1
fi

printf '\nAll checks passed.\n'
