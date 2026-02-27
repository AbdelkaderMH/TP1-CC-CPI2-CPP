# C++ Lab Repository Guide (TP)

This document defines the standard repository structure, CI/CD workflow, and contribution guidelines for the C++ Labs solutions. It is designed to support collaborative development via Pull Requests while enforcing strict compilation standards and theoretical analysis.

---

## 1. Repository Structure

Each lab (TP1, TP2, TP3, TP4, ...) gets its own repository. The internal structure is standardized to accommodate both single-file exercises and multi-file projects.

```text
TPx-Solutions/
├── .github/
│   ├── workflows/
│   │   └── ci.yml                 # CI pipeline (compile & test)
├── exercices/                     # Root folder for all exercises
│   ├── exercice1/
│   │   ├── src/                   # [MANDATORY] .cpp files (main.cpp, etc.)
│   │   ├── include/               # [OPTIONAL] .hpp files (if multi-file)
│   │   ├── data/                  # [OPTIONAL] Input files (csv, txt)
│   │   ├── Makefile               # [MANDATORY] Build script
│   │   └── README.md              # [MANDATORY] Statement + Theory Answers
│   ├── exercice2/
│   │   ├── src/
│   │   ├── Makefile
│   │   └── README.md
│   └── ...
├── README.md                      # Global progress table
└── CONTRIBUTING.md                # Collaboration rules
```

### Folder Rules
| Folder | Status | Usage |
| :--- | :--- | :--- |
| `src/` | **Mandatory** | Must contain `main.cpp` and any implementation `.cpp` files. |
| `include/` | Optional | Create only if the exercise requires header files (e.g., TP1 Ex7, TP3, TP4). |
| `data/` | Optional | Create only if the exercise requires input files (e.g., TP3 Ex7 `etudiants.csv`). |
| `Makefile` | **Mandatory** | Must compile code with strict flags. |
| `README.md` | **Mandatory** | Must contain theoretical answers and usage instructions. |

---

## 2. CI/CD Workflow (`.github/workflows/ci.yml`)

This workflow triggers on every push or pull request. It detects which exercises were modified and runs compilation tests **only for those exercises**.

```yaml
name: C++ Lab CI

on:
  push:
    branches: [main, master]
  pull_request:
    branches: [main, master]

jobs:
  detect-changes:
    runs-on: ubuntu-latest
    outputs:
      matrix: ${{ steps.set-matrix.outputs.matrix }}
    steps:
      - uses: actions/checkout@v4
        with:
          fetch-depth: 0
      
      - name: Detect changed exercises
        id: set-matrix
        run: |
          if [ "${{ github.event_name }}" == "pull_request" ]; then
            files=$(git diff --name-only origin/${{ github.base_ref }}...${{ github.head_ref }})
          else
            files=$(git diff --name-only HEAD~1 HEAD)
          fi
          
          # Extract unique exercise folders (e.g., exercices/exercice1)
          exercises=$(echo "$files" | grep "^exercices/exercice[0-9]*" | cut -d'/' -f1,2 | sort -u | jq -R -s -c 'split("\n")[:-1]')
          
          # If no exercise changed, run all (safety net)
          if [ "$exercises" == "[]" ]; then
            exercises=$(find exercices -maxdepth 1 -type d -name "exercice*" | sort | jq -R -s -c 'split("\n")[:-1]')
          fi
          
          echo "matrix=$exercises" >> $GITHUB_OUTPUT

  build-and-test:
    needs: detect-changes
    runs-on: ubuntu-latest
    strategy:
      fail-fast: false
      matrix:
        exercise: ${{ fromJson(needs.detect-changes.outputs.matrix) }}
    
    steps:
      - uses: actions/checkout@v4
      
      - name: Install Dependencies
        run: sudo apt-get update && sudo apt-get install -y g++ make
      
      - name: Validate Mandatory Structure
        run: |
          dir="${{ matrix.exercise }}"
          if [ ! -f "$dir/Makefile" ]; then echo "::error::Missing Makefile in $dir"; exit 1; fi
          if [ ! -d "$dir/src" ]; then echo "::error::Missing src/ directory in $dir"; exit 1; fi
          if [ ! -f "$dir/README.md" ]; then echo "::error::Missing README.md in $dir (Theoretical answers required)"; exit 1; fi
          # Optional folders (include/, data/) are NOT checked here
      
      - name: Compile (Strict Flags)
        run: |
          dir="${{ matrix.exercise }}"
          cd $dir
          make clean 2>/dev/null || true
          
          # TP4 requires -Werror flag, others use -Wall -Wextra -pedantic
          if [[ "$dir" == *"tp4"* ]] || [[ "$dir" == *"TP4"* ]]; then
            CXXFLAGS="-std=c++17 -Wall -Wextra -pedantic -Werror"
          else
            CXXFLAGS="-std=c++17 -Wall -Wextra -pedantic"
          fi
          
          make CXXFLAGS="$CXXFLAGS" 2>&1 | tee build.log
          
          if [ "${{ github.event_name }}" == "pull_request" ]; then
            if grep -i "warning" build.log; then
              echo "::error::Warnings treated as errors in PRs"; exit 1;
            fi
          fi
      
      - name: Run Tests (Dynamic Data Detection)
        run: |
          dir="${{ matrix.exercise }}"
          cd $dir
          
          # Priority 1: Use 'make test' if defined in Makefile
          if [ -f "Makefile" ] && grep -q "^test:" Makefile; then
            echo "✓ Found 'test' target in Makefile. Running custom test..."
            make test 2>&1 | tee output.txt
          
          # Priority 2: If data/ exists, pipe the first found file to stdin
          elif [ -d "data" ] && [ "$(ls -A data 2>/dev/null)" ]; then
            DATA_FILE=$(find data -type f | head -n 1)
            echo "✓ Data directory found. Piping $DATA_FILE to stdin..."
            timeout 5 ./main < "$DATA_FILE" > output.txt 2>&1 || echo "::warning::Execution timed out or failed"
          
          # Priority 3: Run without input (for exercises not requiring input)
          else
            echo "✓ No data found. Running without input..."
            timeout 5 ./main > output.txt 2>&1 || echo "::warning::Execution timed out or failed"
          fi
```

---

## 3. Flexible Makefile Template

Place this `Makefile` inside each `exerciceN/` folder. It automatically detects if an `include/` folder exists.

```makefile
# Flexible Makefile for C++ Labs
CXX = g++
# Default flags. TP4 should add -Werror via CXXFLAGS override
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGET = main

# Check if include directory exists before adding flag
ifneq ($(wildcard include/),)
    INCLUDES = -I./include
else
    INCLUDES = 
endif

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

#  Customize this target for specific data files when necessary or remove for interactive tests
test: $(TARGET)
	@if [ -d "data" ] && [ "$(ls -A data 2>/dev/null)" ]; then \
		DATA_FILE=$$(find data -type f | head -n 1); \
		echo "Running with $$DATA_FILE"; \
		./$(TARGET) < "$$DATA_FILE"; \
	else \
		./$(TARGET); \
	fi

clean:
	rm -f $(OBJS) $(TARGET) output.txt build.log
```

---

## 4. Lab-Specific Adaptations

Refer to this table when setting up your exercise folder.

| Lab | Exercise | Requirement | Folder Structure |
| :--- | :--- | :--- | :--- |
| **TP1** | **Ex 1-6** | Single file logic | `src/main.cpp`, `Makefile`, `README.md` |
| **TP1** | **Ex 7** | **Multi-file** (`include/`, `src/`) | `src/*.cpp`, `include/*.hpp`, `Makefile`, `README.md` |
| **TP2** | **Ex 1-8** | Mostly single file (Ex8 = `main.cpp`) | `src/main.cpp`, `Makefile`, `README.md` |
| **TP3** | **Ex 1** | Single file | `src/main.cpp`, `Makefile`, `README.md` |
| **TP3** | **Ex 2-6** | **Multi-file** (`.hpp` + `.cpp`) | `src/*.cpp`, `include/*.hpp`, `Makefile`, `README.md` |
| **TP3** | **Ex 7** | **CSV Data** (`etudiants.csv`) | `src/*.cpp`, `include/*.hpp`, `data/etudiants.csv`, `Makefile`, `README.md` |
| **TP4** | **Ex 1,3,4,5,7** | Single file logic (STL) | `src/main.cpp`, `Makefile`, `README.md` |
| **TP4** | **Ex 2,6,8,9** | **Multi-file** (Utils/Structs) | `src/*.cpp`, `include/*.hpp`, `Makefile`, `README.md` |

### Special Notes
*   **TP1 Ex3 & Ex4:** Theoretical questions about compilation flags **must** be answered in `README.md`.
*   **TP2 Ex4:** Manual calculation of macro expansion **must** be in `README.md`.
*   **TP3 Ex7:** Ensure `data/etudiants.csv` matches the PDF content exactly.
*   **TP4 Ex9:** Operator overloading (`operator<`, `operator<<`) must be defined in `include/` headers if used across files.
*   **TP4 Compilation:** TP4 requires `-errors` flag. Add this to your `Makefile` `CXXFLAGS` for TP4 exercises.

---

## 5. Student Contribution Workflow

### Step 1: Claim an Exercise
1.  Go to the **Issues** tab.
2.  Create a new issue using the **"Claim Exercise"** template.
3.  Title: `Solution for Exercice N`.
4.  Assign yourself to prevent duplicate work.

### Step 2: Develop
1.  Fork the repository.
2.  Create a branch: `git checkout -b feat/exercice-N`.
3.  Create the folder structure inside `exercices/` (see Table above).
4.  **Code:** Place all `.cpp` files in `src/`.
5.  **Theory:** Answer all "Question:", "Why?", "Explain" prompts in `exerciceN/README.md`.
6.  **Comments:** **Every line of code must be commented** (Lab Instruction: "Chaque ligne de code doit être comprise et justifiée.").

#### Commit Before Pulling (Important!)
```bash
# 1. Check what files have changed
git status

# 2. Add all your changes to staging
git add exercices/exercice1/

# 3. Commit with a descriptive message
git commit -m "feat(exercice1): complete solution with tests and README"

# 4. NOW you can safely pull remote changes
git pull origin main

# 5. If there are conflicts, resolve them, then:
git add exercices/exercice1/
git commit -m "merge: resolve conflicts for exercice1"

# 6. Push your branch
git push origin feat/exercice1
```
### Step 3: Submit Pull Request
1.  Push your branch and open a Pull Request to `main`.
2.  **CI Check:** The workflow will automatically compile your code.
    *   If it fails: Fix compilation errors or warnings.
    *   If it passes: Wait for review.
3.  **Review:** Instructors/Peers will check code quality and theoretical answers.

### Step 4: Merge
Once approved and CI passes, the PR is merged. The solution becomes part of the official repository.

---

## 6. README.md Template (Per Exercise)

Copy this template into each `exerciceN/README.md`.

```markdown
# Exercice N: [Title]

## 📝 Problem Statement
[Copy the exercise text from the PDF here]

## 💻 Solution Code
See `src/` and `include/` folders.

## 🧠 Theoretical Answers & Analysis
*(Required for exercises asking "Why?", "Explain", "Calculate manually")*

1. **Question:** [Copy question from PDF]
   - **Answer:** [Your answer]
   - **Justification:** [Reference C++ standard or behavior]

2. **Question:** [e.g., TP2 Ex4: Why are results different?]
   - **Answer:** Macro expansion vs function evaluation...

## ✅ Checklist
- [ ] Code compiles with `-Wall -Wextra -pedantic`
- [ ] Every line of code is commented
- [ ] Theoretical questions answered above
- [ ] Data files (if any) included in `data/`
```