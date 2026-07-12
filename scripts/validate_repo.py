from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]

REQUIRED = [
    "README.md",
    "LICENSE",
    "CHANGELOG.md",
    "CONTRIBUTING.md",
    "SECURITY.md",
    "CODE_OF_CONDUCT.md",
    "ROADMAP.md",
    "PROJECT_STRUCTURE.md",
    "LICENSES/README.md",
    "LICENSES/MIT.txt",
    "LICENSES/CERN-OHL-S-v2-NOTICE.txt",
    ".github/PULL_REQUEST_TEMPLATE.md",
    ".github/CODEOWNERS",
    ".github/workflows/validate.yml",
    ".github/ISSUE_TEMPLATE/bug_report.md",
    ".github/ISSUE_TEMPLATE/feature_request.md",
    ".github/ISSUE_TEMPLATE/hardware_issue.md",
]

missing = [p for p in REQUIRED if not (ROOT / p).exists()]

if missing:
    print("Missing files:")
    for item in missing:
        print(f" - {item}")
    sys.exit(1)

print("Repository scaffold looks good.")
