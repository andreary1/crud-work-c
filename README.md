# Electoral Management System (C Project)

This project is a console-based **Electoral Management System**, developed in the C programming language as a **college assignment** during the **first semester** of the **Introduction to Programming** course. 
It uses concepts such as **file I/O**, **pointers to pointers**, **dynamic memory allocation**, **structs**, and **enums**

## 📋 Features

The system allows the user to manage and interact with electoral data through a menu-driven interface. Key functionalities include:

### 🗳️ Voters and Voting
- Register people with CPF, name, voter title, birth date, phone, and address.
- Cast votes with timestamp validation.
- Age check (16+ to vote and to run for office).

### 👤 Candidates
- Register a person as a candidate in a specific election.
- Prevent duplicate candidacies per year.
- Automatically remove associated votes and attendance when a candidate is removed.

### 🏛️ Elections
- Manage elections by year and state (UF).
- Assign descriptions and ensure uniqueness by UF and year.
- Cascade deletion of related candidates and votes when an election is removed.

### 📍 States (UFs)
- Add, edit, delete and display state data (code, name, and abbreviation).
- Cascade deletion of elections when a state is removed.

### 📂 Attendance and Voting Records
- Register voter attendance automatically when voting.
- Show attendance lists per election or state.
- Display and count votes per candidate.

### 📁 File Persistence
All data is stored in binary files:
- `uf.data`
- `pessoas.data`
- `eleicao.data`
- `candidatos.data`
- `votos.data`
- `comparecimentos.data`

## 📦 Structure

The codebase is organized into modular directories:
- `UF/` — State (UF) management
- `eleicao/` — Election management
- `candidato/` — Candidate handling
- `pessoa/` — Personal information
- `voto/` — Voting 
- `comparecimento/` — attendance
- `main.c` — Entry point and main menu logic

## 🧪 Example Interactions

- Insert a UF → Insert an Election in that UF → Add Candidates → Register People → Cast Votes
- Automatically validates all relationships and removes linked data if necessary (cascading deletion).

## 🎓 Academic Context

This project was developed as part of an **academic exercise** for learning structured programming, modular design, and file handling in C. It was completed during the **first semester** of the **Computer Engineering** course at IFCE.

### PS: The entire code is in Portuguese.
