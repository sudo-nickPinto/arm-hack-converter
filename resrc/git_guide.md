# The Aspiring Engineer's Guide to Git and Version Control

## 1. Introduction: The "Time Machine" for Code

Imagine you are writing a massive essay. You save a version as `Final.doc`, then `Final_v2.doc`, then `Final_REAL_v3.doc`. It's a mess. Now imagine doing that with 10 other people editing the same file. That is chaos.

**Version Control Systems (VCS)** solve this. Git is the industry standard VCS. It records changes to a file or set of files over time so that you can recall specific versions later.

For a Software Engineer (SWE), Git is your safety net. It allows you to:
- **Revert** back to a working state if you break something.
- **Collaborate** without overwriting your teammates' work.
- **Experiment** in a safe, isolated environment (branches) without affecting the main project.

---

## 2. The `.gitignore` File: Keeping Your House Clean

Before we dive into commands, we must talk about one of the most important, yet often overlooked, files in a repository: `.gitignore`.

### What is it?
The `.gitignore` file is a simple text file that tells Git which files or folders to **intentionally ignore**. When a file is ignored, Git will not track changes to it, will not stage it, and will not commit it.

### Why is it critical? (Real-World SWE Examples)

1.  **Build Artifacts & Binaries**:
    *   **Scenario**: You are writing C++ code. When you compile `main.cpp`, the compiler generates `main.o` (object file) and `app.exe` (executable).
    *   **Problem**: These files are *generated* from your source code. If you change one line of C++, the entire binary changes. If you commit these, your repository size will balloon to gigabytes very quickly.
    *   **Solution**: You add `*.o` and `*.exe` to `.gitignore`. Git now only cares about your logic (`.cpp`), not the output.

2.  **Security & Secrets**:
    *   **Scenario**: You are connecting to a database and have a file named `.env` containing your password: `DB_PASSWORD=super_secret_123`.
    *   **Problem**: If you commit this file and push it to GitHub, **anyone** can see your password. Bots scan GitHub constantly for these secrets. This is a major security breach.
    *   **Solution**: You add `.env` to `.gitignore` immediately. The file stays on your local machine, but never goes to the cloud.

3.  **System Files**:
    *   **Scenario**: You use a Mac. macOS automatically creates hidden files called `.DS_Store` in folders to remember icon positions.
    *   **Problem**: Your teammate uses Windows. If you commit `.DS_Store`, they will download this useless file that does nothing for them. It's "pollution" in the codebase.
    *   **Solution**: Add `.DS_Store` to `.gitignore`.

### The Golden Rule
**"If it is generated (compiled), system-specific, or sensitive (passwords), IGNORE IT. If it is source code or configuration, COMMIT IT."**

---

## 3. Core Concepts

### The Repository (Repo)
The folder containing your project and the hidden `.git` folder. This is where Git stores all the snapshots and history.

### The Commit (The Snapshot)
A commit is a snapshot of your entire project at a specific moment.
*   **Bad Commit**: "Update code" (What changed? Why?)
*   **Good Commit**: "Fix null pointer exception in user login logic" (Contextual, specific).

### The Stage (The Shopping Cart)
Before you take a snapshot (commit), you must choose what goes into it. This is called "Staging".
*   `git add file.txt` puts the file in the "cart".
*   `git commit` buys the cart (saves the snapshot).

---

## 4. The "Happy Path" Workflow (Collaborative Edition)

This is the daily loop you will perform when working with a team. **Always assume your friend pushed code while you were sleeping.**

1.  **Pull (CRITICAL)**:
    Before you type a single line of code, get the latest updates.
    ```bash
    git pull origin main
    ```
    *If you skip this, you might build your house on an outdated foundation.*

2.  **Check Status**:
    ```bash
    git status
    ```
    *Always* run this. It tells you what files have changed and what is staged.

3.  **Work & Stage**:
    Write your code, then prepare it for the snapshot.
    ```bash
    git add filename.cpp
    # OR to add everything (use carefully!)
    git add .
    ```

4.  **Commit**:
    ```bash
    git commit -m "Implement function to translate ADD instruction"
    ```
    This saves your work **locally**. It is NOT on GitHub yet.

5.  **Push**:
    ```bash
    git push origin main
    ```
    This uploads your commits to GitHub.

---

## 5. Troubleshooting: "Diverged" and "Ahead/Behind"

This happens when you and your friend both worked at the same time, but didn't sync up.

### Scenario 1: "X Commits Ahead of Main"
*   **What it means**: You have worked on your laptop (committed locally), but you haven't uploaded (pushed) to GitHub yet.
*   **Analogy**: You wrote 3 pages of the essay, but haven't emailed it to the group.
*   **Fix**: Run `git push origin main`.

### Scenario 2: "Divergent Branches" (The "Updates were rejected" Error)
*   **What it means**: You tried to push, but GitHub said "Stop! Your friend pushed code that you stick don't have yet."
*   **Analogy**: You tried to save "Final_v4.doc", but the folder already has a "Final_v4.doc" from your friend that looks different.
*   **Fix (The Rebase Workflow)**:
    1.  **Pull the changes**: `git pull origin main`. 
        *   *Note: This might trigger a "Merge Conflict" if you both edited the EXACT same line.*
    2.  **Resolve Conflicts** (if any): Open the files with `<<<<` markers, fix the code, and save.
    3.  **Push again**: `git push origin main`.

---

## 6. Branching: The Multiverse of Code

This is what separates amateur coding from professional engineering.

Imagine you are working on a feature (e.g., "Dark Mode") while your teammate fixes a critical bug. If you both work on the same files in the same place, you will conflict.

**Branches** allow you to create a parallel universe of your code.
*   **`main` (or `master`)**: This is the "Production" code. It should always work.
*   **`feature/dark-mode`**: This is your playground. You can break things here, and `main` remains safe.

### The Workflow:
1.  **Create a branch**: `git checkout -b feature/add-sub-instruction`
2.  **Work & Commit**: Make changes and commit them to this branch.
3.  **Merge**: When you are done, you merge your branch back into `main`.

In a professional setting, you don't just merge. You open a **Pull Request (PR)**. This is a request to merge your code, where your teammates review your code, leave comments, and approve it before it touches the main codebase.

---

## 6. Summary Checklist for this Project

1.  **Initialize**: `git init` (Done!)
2.  **Ignore**: Setup `.gitignore` to block `.o` and `ArmToHack` executable. (Done!)
3.  **Work**: Edit `ArmToHack.cpp`.
4.  **Stage**: `git add ArmToHack.cpp`.
5.  **Commit**: `git commit -m "Implement MOV instruction logic"`.
6.  **Repeat**.
